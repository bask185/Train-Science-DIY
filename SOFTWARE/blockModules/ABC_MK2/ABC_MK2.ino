#include "src/io.h"
#include "src/date.h"
#include "src/version.h"
#include "src/macros.h"
#include "src/debounceClass.h"
#include "src/ServoSweep.h"
#include "src/transceiver.h"

Debounce bufferSensor( bufferSensorPin ) ;
Debounce stopSensor( stopSensorPin ) ;

bool iAmOccupied ;
bool trainIsBuffered ;
bool upLineHasTrain ;

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


void processInput()
{
    uint32 ledInterval = 500 ;

    uint8 state_A = bufferSensor.getState() == LOW ; // set the debounced sensors to the Sender for transmission downline
    sender.setMyOccupancy_A( state_A ) ;
    receiver.setMyOccupancy_A( state_A ) ;

    uint8 state_B = stopSensor.getState() == LOW ;
    sender.setMyOccupancy_B( state_B ) ;
    receiver.setMyOccupancy_B( state_B ) ;
    if( state_B ) ledInterval = 250 ;

    uint8 nextBlockOccupied = receiver.getHisOccupancy() ;
    digitalWrite( ABC, nextBlockOccupied && state_B ) ;    // next block is occupied AND train is buffered, engage ABC
    if( digitalRead( ABC ) ) ledInterval = 100 ;

    REPEAT_MS( ledInterval ) 
    {
        PORTB ^= 1<<5 ;
    }
    END_REPEAT
}

void setup()
{
    initIO() ;

   // sender.begin() ;
   // receiver.begin() ;

    // delay(100);
    // Serial.begin( 115200) ;
    // Serial.println("BOOTED") ;
}

void loop()
{


    // if( Serial.available() )
    // {
    //     byte b = Serial.read() ;

    //     if( b == 'd' ) sender.setDeparture( 1 ) ;
    //     if( b == 'D' ) sender.setDeparture( 0 ) ;

    //     if( b == 'h' ) receiver.setMyOccupancy_A( 1 ) ;
    //     if( b == 'l' ) receiver.setMyOccupancy_A( 0 ) ;

    //     if( b == 'H' ) receiver.setMyOccupancy_B( 1 ) ; // if his occupancy is set, I should send yellow
    //     if( b == 'L' ) receiver.setMyOccupancy_B( 0 ) ;
    // }

    sender.transmitt() ;
    receiver.receive() ;

    debounceInput() ;
    processInput() ;

    // if( digitalRead(stopSensorPin) == LOW )
    // {
    //     digitalWrite(ledPin, LOW);
    // }
    // else{
    //     digitalWrite(ledPin, HIGH);
    // }
    // if( digitalRead(bufferSensorPin) == LOW)
    // {
    //     digitalWrite(ABC,HIGH) ;
    // }
    // else{
    //     digitalWrite(ABC,LOW) ;
    // }
}