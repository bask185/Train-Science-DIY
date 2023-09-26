#include "src/NmraDcc.h"
#include "src/weistra.h"
#include "src/macros.h"

/* 
if mode = MOTOR. both outputs must make use of speed settings, functions must be ignored
else 
    outputs can be coupled to functions of choise.
    outputs can be direction or not





*/

struct // both outputs
{
    uint8   Fx              : 5 ;   // Function number to listen to
    uint8   dir_dependend   : 1 ;   // must I change when direction changes
    uint8   inverted        : 1 ;   // must I invert my output
    uint8   state           : 1 ;   // Am I turned on or off?
    uint8   set             : 1 ;   // used in update function as local variable
    uint8   trig            : 1 ;   // I am used during a pulse
    uint8   pulseTime       : 8 ;   // length of my pulse in 0,1s
    uint8   prevTime        : 8 ;   // used to handle pulse length
} A, B ;

const int shortAddresses = 0 ;
const int longAddresses  = 1 ;

const int AM_FUNCTION_DECODER = 0x01 ;

struct
{
    // standard DCC (according to NMRA settings, may need to look into RCN settings
    uint8   addressPrimary ;    // CV1 address primary
    uint8   speedMin ;          // CV2
    uint8   accelTime ;         // CV3
    uint8   decelTime ;         // CV4
    uint8   maxSpeed ;          // CV5
    
    uint8   manufactorVersion ; // CV7 // hard coded
    uint8   manufactorID ;      // CV8

    uint8   addrLow ;           // CV17
    uint8   addrHigh ;          // CV18

    uint8   modeControl ;       // CV29 default 6, 
//  bit 0 = reverse direction IN USE
//  bit 1 = speed steps, not in use decoder may accept both 28 and 128
//  bit 2 = analog mode?? DONT CARE
//  bit 3 = railcom,      DONT CARE
//  bit 4 = speed table.  DONT CARE
//  bit 5 = extended address  IN USE
//  bit 6 = DONT CARE
//  bit 7 = accessory decoder not in USE.

    uint8   kickStartTime ;     // CV65 motor pulse length in MS when train is stationairy ans must move.

// TS unique settings
    uint8   mode ;              // CV112 // am I motor driver (=0) or function decoder (=1)

    //F1 settings
    uint8   F1dir ;             // CV113 
//  bit 0 = dir dependend 
//  bit 1 = dir reversed
//  bit 2 = pulse on  rising flank
//  bit 3 = pulse on falling flank
    uint8   F1time ;            // CV114 (pulse time) // if not 0, handles pulse time of function. 1 Step = 0.1S so 25.5S max
    uint8   F1pseudo ;          // CV115 coupling to other function number, So you can switch F1 with F12 orso

    //F2 settings
    uint8   F2dir ;             // CV116
    uint8   F2time ;            // CV117
    uint8   F2pseudo ;          // CV118

} my ;

uint8   speed ;
uint8   dir ;
int8    setPoint ;
int8    currentSpeed ;
uint32  newFunc = 0x0000;
uint32  oldFunc = 0x0000 ;

NmraDcc dcc;
Weistra pwm(1, 2, 50, 100);


void followSpeed()
{
    if( currentSpeed < setPoint ) currentSpeed ++ ;
    if( currentSpeed > setPoint ) currentSpeed -- ;
    pwm.setSpeed( currentSpeed ) ;
}

enum states
{
    stationairy,
    kickStart,
    cruising,
    accelerating,
    decelerating,
};

void throttle()
{
    static uint8 state = stationairy ;

    switch( state )
    {
    case stationairy:
        if( setPoint > 0 ) state = kickStart ;
        break ;

    case kickStart:
        if( pwm.kickStart( my.kickStartTime ) ) state = accelerating ; // sets kick start, returns true when pulse has passed
        break ;

    case cruising:
        REPEAT_MS( 50 )
        {
            followSpeed() ;

            if( setPoint > currentSpeed ) state = accelerating ;
            if( setPoint < currentSpeed ) state = decelerating ;
        }
        END_REPEAT
        break ;

    case accelerating:
        REPEAT_MS( my.accelTime )
        {
            followSpeed() ;

            if( currentSpeed == setPoint ) state = cruising ;
        }
        END_REPEAT
        break ;

    case decelerating:
        REPEAT_MS( my.decelTime )
        {
            followSpeed() ;

            if(      currentSpeed ==        0 ) state = stationairy ;
            else if( currentSpeed == setPoint ) state = cruising ;
        }
        END_REPEAT
        break ;
    }
} ;

void updateFunc()
{
    // conditions:
    // State must be true
    // AND dir depened is OFF OR direction is OK
    
    // fetch live from EEPROM?

    A.set = A.state 
    && ( !A.dir_dependend | (dir ^ A.inverted) ) ;

    B.set = B.state 
    && ( !B.dir_dependend | (dir ^ B.inverted) ) ;


    if( A.pulseTime > 0 ) // if pulse time is enabled.
    {
        if( A.set == 0 ) A.trig = 1 ; // reset trigger when output is turned off

        if( A.trig == 1 )
        {   A.trig  = 0 ;

            A.prevTime = millis() ;
            digitalWrite( 2, HIGH ) ;
        }

        if( millis() - ((uint32)100*A.pulseTime) >= ((uint32)100*A.prevTime)) 
        {
            digitalWrite( 2,  LOW ) ;
        }
    }
    else
    {
        digitalWrite( 2, A.set ) ;
    }

    if( B.pulseTime > 0 ) // if pulse time is enabled.
    {
        if( B.set == 0 ) B.trig = 1 ; // reset trigger when output is turned off

        if( B.trig == 1 )
        {   B.trig  = 0 ;

            B.prevTime = millis() ;
            digitalWrite( 2, HIGH ) ;
        }

        if( millis() - ((uint32)100*B.pulseTime) >= ((uint32)100*B.prevTime)) 
        {
            digitalWrite( 2,  LOW ) ;
        }
    }
    else
    {
        digitalWrite( 2, B.set ) ;
    }
}


void loadCV()
{
    if( dcc.getCV( 511 ) != 0xCC )   // if this CV is mutilated...
    {   dcc.setCV( 511,     0xCC ) ; // factory reset to all CV

        dcc.setCV(   1,   3 ) ;  // addressPrimary 

        dcc.setCV(   2,   1 ) ; // speedMin    1
        dcc.setCV(   3,  10 ) ; // accelTime  10
        dcc.setCV(   4,  10 ) ; // decelTime  10
        dcc.setCV(   5, 100 ) ; // maxSpeed  100

        dcc.setCV(  17,   0 ) ; // addrLow 
        dcc.setCV(  18,   3 ) ; // addrHigh addr 3

        dcc.setCV(  29,   2 ) ; // modeControl  128 steps on, rest dont care
        dcc.setCV(  65,   5 ) ; // kickStartTime 5ms 
        
        dcc.setCV( 112,   0 ) ; // mode -> Lopi

        dcc.setCV( 113,   4 ) ; // F1dir 
        dcc.setCV( 114,   0 ) ; // F1time 
        dcc.setCV( 115,   1 ) ; // F1pseudo 
        
        dcc.setCV( 116,   4 ) ; // F2dir 
        dcc.setCV( 117,   0 ) ; // F2time 
        dcc.setCV( 118,   2 ) ; // F2pseudo 
    }

    my.addressPrimary       = dcc.getCV(   1 ) ;
    my.speedMin             = dcc.getCV(   2 ) ;
    my.accelTime            = dcc.getCV(   3 ) ;
    my.decelTime            = dcc.getCV(   4 ) ;
    my.maxSpeed             = dcc.getCV(   5 ) ;
    my.manufactorVersion    =   1 ;               // dcc.getCV(   7 ) ;
    my.manufactorID         = 123 ;               // dcc.getCV(   8 ) ; // I should get me a valid number registered somewhere..?
    my.addrLow              = dcc.getCV(  17 ) ;
    my.addrHigh             = dcc.getCV(  18 ) ;
    my.modeControl          = dcc.getCV(  29 ) ;
    my.kickStartTime        = dcc.getCV(  65 ) ;
    my.mode                 = dcc.getCV( 112 ) ;
    my.F1dir                = dcc.getCV( 113 ) ;
    my.F1time               = dcc.getCV( 114 ) ;
    my.F1pseudo             = dcc.getCV( 115 ) ;
    my.F2dir                = dcc.getCV( 116 ) ;
    my.F2time               = dcc.getCV( 117 ) ;
    my.F2pseudo             = dcc.getCV( 118 ) ;
}

void setup()
{
    dcc.init(MAN_ID_DIY|FLAGS_MY_ADDRESS_ONLY, 10, 0, 0);
    pwm.begin();

    loadCV() ;
}

void loop()
{
    dcc.process();

    if( (my.mode & AM_FUNCTION_DECODER) == false ) throttle() ;
    else                                           updateFunc() ;
}


void notifyDccSpeed(uint16_t Addr, DCC_ADDR_TYPE AddrType, uint8_t Speed, DCC_DIRECTION Dir, DCC_SPEED_STEPS SpeedSteps)
{
    setPoint    = map( Speed, 1, SpeedSteps, 0, 100 ) ;   // remap DCC speed to pwm controller speed levels
    dir         = Dir ^ (my.modeControl & 0x01) ;                   // XOR received dir with my own reverse direction bit
    if( dir )   setPoint =-setPoint ;
}

void setFunc( uint8 num, uint8 state )
{
    if( num == A.Fx ) A.state = state ;
    if( num == B.Fx ) B.state = state ;
}

void notifyCVAck()
{
    digitalWrite(1,  LOW ) ;
    digitalWrite(2, HIGH ) ;
    delay(6);
    digitalWrite(2,  LOW ) ;

    loadCV() ; // after a write, reload CV.
}

// This call-back function is called whenever we receive a DCC Function packet for our address
void notifyDccFunc(uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState)
{
    uint8   beginF ;
    uint8   endF ;
    uint8   F0 ;
    uint8   i ;
    uint8   Fx ;

    switch( FuncGrp )
    {
    case FN_0_4:    F0 = bitRead( FuncState, 4 ) ;          // put F0 in front
                    FuncState = (FuncState << 1) | F0 ;     // and shift the others
                    beginF =  0 ; endF =  4 ; goto setFunc ;
    case FN_5_8:    beginF =  5 ; endF =  8 ; goto setFunc ;
    case FN_9_12:   beginF =  9 ; endF = 12 ; goto setFunc ;
    case FN_13_20:  beginF = 13 ; endF = 20 ; goto setFunc ;
    case FN_21_28:  beginF = 21 ; endF = 28 ; goto setFunc ;
    
    setFunc:
        for( Fx =  beginF, i = 0 ; 
             Fx <= endF ; 
             Fx ++ ,       i ++ )
        {
            uint8 state = FuncState & (1<<i) ;
            setFunc( Fx, state ) ; 
        }
        break ;
    }
}


void notifyCVResetFactoryDefault()
{
    dcc.setCV(511, 123 ) ; // ensure that new CV will be loaded
    loadCV() ;
}