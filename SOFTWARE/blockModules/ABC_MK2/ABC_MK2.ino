#include "src/io.h"
#include "src/date.h"
#include "src/version.h"
#include "src/macros.h"
#include "src/debounceClass.h"
#include "src/ServoSweep.h"
#include "src/transceiver.h"
#include "src/Trigger.h"
#include <Servo.h>

/* humanize problem

    if the next block is freeded up. departGO must become true.
    when the next block receives the train in the stop section, deportGo must become false

    go signal = falling flank on digitalRead( ABC ) ;
    stop signal = rising flank on upline stop detector !!! this info is not present

    stop signal, RISING flank on both my own sensors. If both sensors are freed up, all is well
*/

Servo distArm ; uint8 distPos = 90 ; 
Servo stopArm ; uint8 stopPos = 90 ;

Debounce bufferSensor( bufferSensorPin ) ;
Debounce stopSensor(     stopSensorPin ) ;


bool  iAmOccupied ;
bool  trainIsBuffered ;
bool  upLineHasTrain ;
bool  SP_DIST ;
bool  SP_STOP ;
bool  departure ;
uint8 semaphore ;
uint8 stopState ;
uint8 nextBlock_A;
uint8 nextBlock_B;
uint8 bufferState ;
uint32 ledInterval ;

enum 
{
    green,
    red,
    yellow,
} ;

Sender   sender( downlinePin, ABC_MODULE ) ;
Receiver receiver( uplinePin ) ;


void debounceInput()
{
    REPEAT_MS( 100 )
    {
        bufferSensor.debounce() ;
        stopSensor.debounce() ;
    }
    END_REPEAT
}


void processTransceiver()
{
    sender.transmitt() ;
    receiver.receive() ;

    ledInterval = 500 ;

    bufferState = bufferSensor.getState() ;
    uint8 state_A     = bufferState == LOW ; // set the debounced sensors to the Sender for transmission downline

    sender.setMyOccupancy_A( state_A ) ;
    receiver.setMyOccupancy_A( state_A ) ;

    stopState  = stopSensor.getState() ; 
    uint8 state_B    = stopState == LOW ;

    sender.setMyOccupancy_B(   state_B ) ;
    receiver.setMyOccupancy_B( state_B ) ;

    if( state_A ) ledInterval = 120 ;
    if( state_B ) ledInterval = 250 ;

    nextBlock_A = receiver.getHisOccupancy_A() ;
    nextBlock_B = receiver.getHisOccupancy_B() ;

    SP_STOP = nextBlock_A ; // test code
    SP_DIST = nextBlock_B ;
}

void trackLogic()
{
    
    static F_trigger nextBlockFreed ;
    static R_trigger myBlockCleared ;

// wanneer moet sein op rood?
//     als van het volgende blok een melder op rood staat.
    if( nextBlock_A || nextBlock_B ) semaphore = red ;

// wanneer moet sein op groen?
//     Als het volgende sein groen of geel is het blok is niet bezet
    if( !nextBlock_A && !nextBlock_B ) semaphore = green ;

// wanneer moet ABC aan?
//     Een neergaande flank van de stopmelder, EN de trein vertrekt niet EN het volgende blok is BEZET
    if( stopState == FALLING && (nextBlock_A || nextBlock_B) && departure == false )
    {
        digitalWrite( ABC, HIGH ) ;
    }
// wanneer vertrekt de tein?
//     Als het volgende blok is vrij gekomen
    if( nextBlockFreed.trigger( nextBlock_A || nextBlock_B ) ) // falling trigger
    {
        departure =  true ; // if both sensors of next block become low, departure is allowed
        digitalWrite( ABC, LOW ) ; // this part seems to work
    }
// wanneer vertrekt de trein niet meer?
//     Als de stopmelder van het volgende blok opkomt
    if( myBlockCleared.trigger( nextBlock_B ) )
    {
        departure = false ; 
    }
}

void blinkLed()
{
    if( digitalRead( ABC ) ) ledInterval = 100 ;
    REPEAT_MS( ledInterval ) 
    {
        PORTB ^= 1<<5 ;
    }
    END_REPEAT
}

void signals()
{
    // switch( semaphore )
    // {
    // case  green: digitalWrite( greenPin, HIGH ) ; digitalWrite( yellowPin,  LOW ) ; digitalWrite( redPin,  LOW ) ; SP_DIST = 0 ; SP_STOP = 0 ; break ; 
    // case yellow: digitalWrite( greenPin,  LOW ) ; digitalWrite( yellowPin, HIGH ) ; digitalWrite( redPin,  LOW ) ; SP_DIST = 0 ; SP_STOP = 0 ;break ; 
    // case    red: digitalWrite( greenPin,  LOW ) ; digitalWrite( yellowPin,  LOW ) ; digitalWrite( redPin, HIGH ) ; SP_DIST = 1 ; SP_STOP = 1 ;break ;
    // }

}

void servos() 
{
    REPEAT_MS( 25 ) 
    {
        uint8 setPoint ;
        if( SP_STOP ) setPoint = 135 ;
        else          setPoint =  90 ;
        if( setPoint > stopPos ) stopPos ++ ;
        if( setPoint < stopPos ) stopPos -- ;
        stopArm.write( stopPos ) ;

        if( SP_DIST ) setPoint = 135 ;
        else          setPoint =  90 ;
        if( setPoint > distPos ) distPos ++ ;
        if( setPoint < distPos ) distPos -- ;
        distArm.write( distPos ) ;
    }
    END_REPEAT
}

void setup()
{
    initIO() ;

    sender.begin() ;
    receiver.begin() ;

    distArm.write( 90 ) ; // 90 degree must be SAFE, 135 is UNSAFE
    stopArm.write( 90 ) ;

    distArm.attach( distantServoPin ) ;
    stopArm.attach( mainServoPin ) ;

    // delay(100);
    // Serial.begin( 115200) ;
    // Serial.println("BOOTED") ;
}

void loop()
{  
    debounceInput() ;
    processTransceiver() ;
    trackLogic() ;
    servos() ;
    signals() ;
    blinkLed() ;
}

/*
On in state machine variant

Ik kan zijn:
IDLE. Het volgende blok is vrij en ik ben vrij

I_got_train: ik heb een trein maar het volgende blok is vrij dus ik hoef eigenlijk niks


situation 1, Train passes sections with last car re-tripping the stop sensor
situation 2. Train arrives while next block is already occupied.

*/