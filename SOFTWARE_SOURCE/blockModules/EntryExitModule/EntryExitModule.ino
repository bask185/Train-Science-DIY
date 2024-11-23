#include "src/io.h"
#include "src/date.h"
#include "src/version.h"
#include "src/macros.h"
#include "src/transceiver.h"

/* TODO
- module must be able to work both as entry or exit module
- fix IO settings
- add statemachine for controlling logic and such.
- add loconet to the code
- devise way to make streets teachin-able
*/
// in order to attempt compatibility for both entry and exit modules, I make 2 arrays of both... The last element is ment for the opposide side


const int ENTRY = 1 ;
const int  EXIT = 2 ;

enum
{
    IDLE,
    SETTING_POINTS,
    SENDING_TRAIN,
} ;

uint8   state ;
uint8   whoami ;
uint8   laneToGo ;
uint8   pointIndex ;
uint32  timeStamp ;

struct Point
{
    uint16_t address : 14 ;
    uint16_t   state :  1 ;
};

struct Street
{
    Point point[7] ;
};

Street street[8] ;  // I have more than enough RAM to have this in RAM..



Sender transmitter[9] =
{
    Sender(  2, EXIT_MODULE ), 
    Sender(  3, EXIT_MODULE ), 
    Sender(  4, EXIT_MODULE ), 
    Sender(  5, EXIT_MODULE ), 
    Sender(  6, EXIT_MODULE ), 
    Sender(  7, EXIT_MODULE ), 
    Sender( 11, EXIT_MODULE ), 
    Sender( 10, EXIT_MODULE ),

    Sender( 12, EXIT_MODULE ), 
} ;

Receiver receiver[9] =
{
    Receiver(  2 ), 
    Receiver(  3 ), 
    Receiver(  4 ), 
    Receiver(  5 ), 
    Receiver(  6 ), 
    Receiver(  7 ), 
    Receiver( 11 ), 
    Receiver( 10 ),

    Receiver( 12 ), 
} ;

void setup()
{
    initIO() ;
    Serial.begin( 115200 ) ;
    Serial.println( version ) ;
    Serial.println( date ) ;

    for(int i = 8 ; i < 8 ; i ++ )
    {
        if( whoami == ENTRY ) // entry module has 1 receiver (which may be unconnected) and 8 transmitters
        {
            receiver[i].begin() ;
            transmitter[8].begin() ;
        }
        else
        {
            transmitter[i].begin() ;
            transmitter[i].setDeparture( 0 ) ; // flag all previous ABC modules to hold down trains
            receiver[8].begin() ;
        }
    }
}

void loop()
{
    if( whoami == ENTRY ) { entryLogic() ; }
    else                  {  exitLogic() ; }
}

// connects to 8 ABC downline (receivers) and merely determen whether their tracks are free or not. If a track is free, points must be set.
// also signal downline should be transmitted that ABC is allowed to dispatch a train. THis could be an exit module, an ABC or nothing.
void entryLogic()
{
    static uint8_t freeLanes[8] ;

    for( int i = 0 ; i < 8 ; i ++ )
    {
        receiver[i].receive() ;
    }
    transmitter[8].transmitt() ;

    switch( state )
    {
    case IDLE:
        {
            for( int i = 0 ; i < 8 ; i ++ ) freeLanes[i] = 0 ; // mark all lanes as not free 

            uint8_t laneAvailable = 0 ; // we start with no available lane        

            for( int i = 0 ; i < 8 ; i ++ )                    // check who are full
            {
                if( receiver[i].getHisOccupancy_A() == 0 
                 && receiver[i].getHisOccupancy_B() == 0 )
                {
                    freeLanes[i] = 1 ; // note a not connected one is free
                    laneAvailable = 1 ; 
                }
            }

            // select a random lane and check if that one is also free, keep doing this until a random found lane is free.
            if( laneAvailable )
            {
                while( laneAvailable == 1 )
                {
                    int randomLane = random( 8 ) ; // yields 0-7

                    if( freeLanes[randomLane] == 1 ) // if this random lane is a free one
                    {
                        pointIndex    = 0 ;                     // reset this
                        laneToGo      = randomLane ;            // select the random lane
                        state         = SETTING_POINTS ;        // go to next state
                        laneAvailable = 0 ;                     // escape for-loop
                    }
                }
            }
        }
        break ;

    case SETTING_POINTS: // set street. a street can have 7 points max, there can be 8 streets
        REPEAT_MS( 250 )
        {
            uint16_t address = street[laneToGo].point[pointIndex].address ;
            uint16_t   state = street[laneToGo].point[pointIndex].state   ;

            setTurnout( address, state ) ;

            if( ++ pointIndex == 7 || address == 0x3FFF ) state = SENDING_TRAIN ; // if all points are set -> go on
        }
        END_REPEAT
        break ;

    case SENDING_TRAIN:
        if( receiver[laneToGo].getHisOccupancy_B() == 1 ) // if stop sensor of selected ABC sees a train, the train is free of the points.
        {
            state = IDLE ;
        }
        break ;

    // TODO, signal to previous module than I am free now.
    }
}

// exit module is connected to 8 ABC downline.
// it is to monitor if atleast 1 ABC has a train, the module must  respond to all ABC to hold down their trains untill said otherwise
// it is also to monitor if an ABC or entry module signals that upline is clear
// if there is a train present and upline is clear, points must be set, train must be sent (there must be waited on the train) than system may reboot

// so IDLE (monitor all modules)
//    SETTING_POINTS
//    SENDING_TRAIN 
void exitLogic()
{
    static uint8_t occupiedLanes[8] ;

    for( int i = 0 ; i < 8 ; i ++ )
    {
        transmitter[i].transmitt() ;
    }
    receiver[8].receive() ;

    switch( state )
    {
    case IDLE:
        {
            if( receiver[8].getHisOccupancy_B || receiver[8].getHisOccupancy_A ) break ; // upline module signals it has train so we just have to wait...
            // TODO, check if no module is connected behind me, that I can just request a train unconditonally.
            // TODO, build in library (if possible) that a switch connected outgoing signal line, can yield in holding down a train

            for( int i = 0 ; i < 8 ; i ++ )
            {
                occupiedLanes[i] = 0 ; // mark all lanes as not free 
            }
            uint8_t laneHasTrain = 0 ;

            for( int i = 0 ; i < 8 ; i ++ )
            {
                occupiedLanes[i] = transmitter[i].getHisOccupancy() ;
                if( occupiedLanes[i] == 1 ) 
                {
                    laneHasTrain = 1 ;
                }
            }

            if( laneHasTrain )
            {
                while( laneAvailable == 1 )
                {
                    int randomLane = random( 8 ) ; // yields 0-7

                    if( occupiedLanes[randomLane] == 1 )
                    {
                        pointIndex    = 0 ;                     // reset this
                        laneToGo      = randomLane ;            // select the random lane
                        state         = SETTING_POINTS ;        // go to next state
                        laneAvailable = 0 ;                     // escape for-loop
                    }
                }
            }
        }
        break;

    case SETTING_POINTS: // CHECK ME
        REPEAT_MS( 250 )
        {
            uint16_t address = street[laneToGo].point[pointIndex].address ;
            uint16_t   state = street[laneToGo].point[pointIndex].state   ;

            setTurnout( address, state ) ;

            if( ++ pointIndex == 7 || address == 0x3FFF ) 
            {
                state = SENDING_TRAIN ; // if all points are set -> go on
                transmitter[laneToGo].setDeparture( 1 ) ; // flag to this 1 ABC module, to depart his train

                timeStamp = millis() ;
            }
        }
        END_REPEAT
        break;

    case SENDING_TRAIN:                     // Note, this time may be configurable in software..
        if( millis() - timeStamp >=  8000 )
        {
            transmitter[laneToGo].setDeparture( 0 ) ; // after around 8 seconds, flag ABC module to hold the next train.
        }
        if( millis() - timeStamp >= 45000 ) // we will wait atleast 45 seconds before a train is allowed to depart. 
        {
            state = IDLE ;
        }
        break;
    }
}

void setTurnout( uint16_t address, uint8_t state )
{
    // do loconet message for turnout
}