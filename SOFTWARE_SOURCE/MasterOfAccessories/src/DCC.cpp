/*
TODO

*/

// HEADER FILES
#include "DCC.h"
#include "macros.h"
#include "io.h" 
#include "stateMachineClass.h"

bool ACCES = 0; // DELETEME
bool DEBUG_LIVE = 0 ;
bool packetCounter ;



// #define TIMER1 NOTE TIMER 1 has mistake
#define TIMER2

#if defined TIMER1
#define TCCR_A      TCCR1A
#define TCCR_B      TCCR1B
#define TCNT        TCNT1
#define CS_0        CS10
#define CS_1        CS11
#define CS_2        CS12
#define TIMSK       TIMSK1
#define ENABLE_BIT  OCIE1A
#define OCRA        OCR1A
#define TIMER_COMPA_vect   TIMER1_COMPA_vect // ISR number 11
#define WGM_1       WGM11

const int   DCC_ZERO_BIT =  1855 ; // 116us prescaler
const int   DCC_ONE_BIT  =   927 ; // 58us 

#elif defined TIMER2
#define TCCR_A      TCCR2A
#define TCCR_B      TCCR2B
#define TCNT        TCNT2
#define CS_0        CS20
#define CS_1        CS21
#define CS_2        CS22
#define TIMSK       TIMSK2
#define OCRA        OCR2A
#define ENABLE_BIT  OCIE2A
#define TIMER_COMPA_vect   TIMER2_COMPA_vect // ISR number 7
#define WGM_1       WGM21

const int   DCC_ZERO_BIT = 200; // 1855/16;//8 ; // 116us 8x prescaler
const int   DCC_ONE_BIT  = 116; // 927/16;//8 ; // 58us 

#else
#error NO TIMER DEFINED
#endif



uint32 beginTime ;

StateMachine sm ;
// CONSTANTS
const int   maxPreAmble         =   21 ; // seems to be adjusted in RCN210 or 211..
const int   nAccessoryRepeats   =    6 ;
const int   nAccessories        =   50 ; // size of accessory buffer

enum packetTypes 
{
    idlePacket,
    accessoryPacket ,
    accessoryExtendedPacket ,
} ;

enum states
{
    selectNewPacketType ,
    assemblePacket ,
    awaitPacketSent ,
} ;

enum ISRstates
{
    preAmble,
    sendZero,
    sendByte,
    sendOne,
    theEnd,
} ;

// VARIABLES

struct
{
    uint8 data[10] ;
    uint8 length ;                    // these variable keeps track of ammount of bytes in FIFO buffer
} buffer[2] ;

static uint8   *ptr ;
static uint8   packetType = idlePacket ; // for DCC packets
static uint8   toSend ;
static uint8   toFill ;
static uint8   repeatCounter ;
static uint8   repeatCount ;

struct accessories          // small buffer to store accessory commands in
{                           // if central receives more than 1 accessory instuction before the previous one is
    uint16  address : 15 ;  // clocked out, the instruction would otherwise be discarded.
    uint8   ext     : 1 ;
    uint8   state ;
} accessory[nAccessories];

static uint8   accessoryIn  ;
static uint8   accessoryOut ;
static uint8   accessoryNow ; // retains address information during repeats


// CONTROL FUNCTIONS
enum instructions
{
    speedInstruction,
    setF1F4,
    clrF1F4,
    setF5F8,
    clrF5F8,
    headlightInstruction,
} ;


DCC *DCC::dccObject = 0 ;	//Static

DCC::DCC()
{
}

// CONTROL FUNCTIONS TO CONTROLL DCC CENTRAL WITH
void DCC::setAccessoryExt( uint16 address, uint8 state, uint8 ext )
{
    accessory[accessoryIn].address = address - 1 ;
    accessory[accessoryIn].state   =   state ;
    accessory[accessoryIn].ext     =     ext ;

    if( ++ accessoryIn == nAccessories ) accessoryIn = 0 ; // handle overflow of index
}

void DCC::setAccessoryExt( uint16 address, uint8 state ) // call above function and flag EXT
{
    setAccessoryExt( address, state, 1 ) ;
}

void DCC::setAccessory( uint16 address, uint8 state )    // call above function and flag NO EXT
{
    setAccessoryExt( address, state, 0 ) ;
}



/******** STATE FUNCTIONS ********/
// in order of priotiry we check for:
// if an instructions is to be/being repeated
// if an accessory is set
// if a loco has has new instructions
// and otherwise we go do cyclic repeat instructions for locos

#define StateFunction( x ) uint8_t DCC::x##F()

StateFunction( selectNewPacketType )
{
    uint8 i = 0 ;
    uint8 j = 0 ;

    if( repeatCounter )
    {   repeatCounter -- ;
        
        return 1 ;
    }

    if( accessoryIn != accessoryOut )       // an accessory has to be set..
    {
        if( accessory[accessoryOut].ext == 1 ) { packetType = accessoryExtendedPacket ; }
        else                                   { packetType = accessoryPacket ;         }

        accessoryNow  = accessoryOut ;
        repeatCounter = nAccessoryRepeats - 1 ;         // set the amount of repeats THIS SEEMS TO WORK

        if( ++ accessoryOut == nAccessories )  accessoryOut = 0 ;    // handle overflow of index
        return 1 ;
    }

    // no accessory due, load idle packets
    packetType = idlePacket ;
    return 1 ;
}


StateFunction( assemblePacket )
{
    const int   speed = 0 ; // array indices
    uint8       index = 0 ;
            
    toFill ^= 1 ; // toggle to the other buffer to fill

    switch(packetType)
    {
    case idlePacket:
        buffer[toFill].data[index++] =  0xFF ; //  3130us
        buffer[toFill].data[index++] =  0x00 ;
        buffer[toFill].length        = index ;
        break ;

    case accessoryPacket:
    case accessoryExtendedPacket: 
        uint16 address  = accessory[accessoryNow].address + 4 ;
        uint8 state     = accessory[accessoryNow].state ;
        uint8 ext       = accessory[accessoryNow].ext ;
       
        uint8 lowNibble ;
        uint8 highNibble ;

        if( ext == 0 ) 
        {
            lowNibble  = ( (address & 0x0003) << 1 | state | 0b1000) & 0xF ;    // sets A0 and A1 direction and output = on, also picks EXT or conventional
            highNibble = (~((address >> 8) & 0x03)) & 0xF ;                     // sets and invert A8-A10 and the first bit
        }
        else
        {
            lowNibble  = ( (address & 0x0003) << 1 | 0b0001 ) & 0xF ;            // sets A0 and A1 direction and output = on, also picks EXT or conventional
            highNibble = (~((address >> 8) & 0x03)) & 0x7 ;                      // sets and invert A8-A10. First bit remains 0
        }

        uint8 byte1 =  0x80 | (address & 0x00FF) >> 2 ;                     // sets a7 <-> A2
        uint8 byte2 =  highNibble << 4 | lowNibble ;

        buffer[toFill].data[index++] = byte1 ;
        buffer[toFill].data[index++] = byte2 ;

        if( ext == 1 ) // EXTENDED ACCESSORY
        {
            buffer[toFill].data[index++] = state ;
        }
        
        break ;
    }

    /***** CHECKSUM *******/
    
    uint8 checksum = 0 ;
    for( int i = 0 ; i < index ; i ++ )
    {
        checksum ^= buffer[toFill].data[i] ;
    }

    buffer[toFill].data[index++] = checksum ;
    buffer[toFill].length = index  ;

    return true ;
}


    
StateFunction( awaitPacketSent )
{
    if( bitRead(TIMSK,ENABLE_BIT) ) return 0 ;             // if previous packet is being transmitted, return false ;

    toSend = toFill  ;                                                      // set buffer transmitt index correctly
    TCNT  = 0 ;                                                             // initialize timer 
    OCRA = DCC_ONE_BIT ;
    TIMSK |= (1 << ENABLE_BIT);                                             // and fire up the ISR..

    return 1 ;
}



// STATE MACHINE
void DCC::generatePackets()
{
    STATE_MACHINE_BEGIN( sm )
    {
        State( selectNewPacketType ) {
            sm.nextState( assemblePacket, 0 ) ; }

        State( assemblePacket ) {
            sm.nextState( awaitPacketSent, 0 ) ;  }
        
        State( awaitPacketSent ) {
            sm.nextState( selectNewPacketType, 0 ) ;  }
    } 
    STATE_MACHINE_END( sm )
}

/***** INTERRUPT SERVICE ROUTINE *****/
ISR(TIMER_COMPA_vect)
{
    DCC::timerISR() ;
}

inline void DCC::timerISR()
{
    static uint8 state = preAmble ;
    static uint8 preAmbleCounter ;
    static uint8 *ptr ;
    static uint8 bitMask ;
    static uint8 ISR_state = 0 ; 

    if( dccISR ) dccISR() ; // call this to toggle IO

    ISR_state ^= 1 ;
    if(ISR_state) return ;

    switch( state )
    {
    case preAmble:
        OCRA = DCC_ONE_BIT ;
        if( oneBit )  oneBit() ; // weak declared debug function
        if( preAmbleCounter++ == maxPreAmble )
        {
            preAmbleCounter = 0 ;
            state = sendZero ;
            ptr = &buffer[toSend].data[0]-1 ; // load the pointer to the first byte
        }
        break ;

    case sendZero:
        OCRA = DCC_ZERO_BIT ; 

        if( zeroBit2 ) zeroBit2() ; // weak declared debug function

        state = sendByte ; 
        ptr++ ;
        bitMask = 0x80 ;
        buffer[toSend].length -- ;
        break ;

    case sendByte:
        if( *ptr & bitMask ){ OCRA =  DCC_ONE_BIT ; }
        else                { OCRA = DCC_ZERO_BIT ; }
        bitMask >>= 1 ;

        if( OCRA ==  DCC_ONE_BIT &&  oneBit )  oneBit() ; // weak declared debug function
        if( OCRA == DCC_ZERO_BIT && zeroBit ) zeroBit() ; // weak declared debug function
        
        if( bitMask == 0 )
        {
            state = sendZero ;
            if( buffer[toSend].length == 0 )
            {
                state = sendOne ;
            }
        }
        break ;

    case sendOne:                   // send final 1 bit to flag end of dcc packet
        if( endBit ) endBit() ; // weak declared debug function
        OCRA = DCC_ONE_BIT ;
        state = theEnd ;
        break ;

    case theEnd:
        bitClear(TIMSK,ENABLE_BIT) ; // kill ISR and flag main routine that packet is transmitted.
        state = preAmble ;
        break ;
    }
} // toggle pin 8 and 9 for direction*/





void DCC::begin()                     // initialize the timer
{
    cli() ; 

    TCCR_A = 0 ; // set entire TCCR_A register to 0
    TCCR_B = 0 ;

    TCCR_A |= (1 << WGM_1 ) ;

#ifdef TIMER2
    TCCR_B |= (0 << CS_2) | (1 << CS_1) | (0 << CS_0); // set prescaler to 8
#endif
   
    sei() ;
    sm.setState( selectNewPacketType ) ;
    dccObject = this;
}
