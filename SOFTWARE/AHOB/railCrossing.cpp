// HEADER FILES
#include <Arduino.h>
#include "railCrossing.h"
#include "src/stateMachineClass.h"
#include "src/io.h"
#include "config.h"
#include <Servo.h>

static StateMachine sm ;

enum states
{
	waitCrossing ,
	closeCrossing ,
	openCrossing ,
} ;

#define DOWN 1
#define UP 2

// VARIABLES
bool occupied ;

uint8_t angle;
Servo arm1;
Servo arm2;

// FUNCTIONS
extern void railCrossingInit(void)
{ 
	sm.nextState( waitCrossing, 0 ) ; 
    arm1.attach(servoPin1);
    arm2.attach(servoPin2);
	arm1.write(openPos);
	arm2.write(openPos);
}

void blinkLeds(uint8_t blinkTime)
{
	REPEAT_MS( blinkTime ) 
	{
		if( digitalRead(blinkLed1) ) {
			digitalWrite(blinkLed1, LOW);
			digitalWrite(blinkLed2, HIGH);
		}
		else {
			digitalWrite(blinkLed1, HIGH);
			digitalWrite(blinkLed2, LOW);
		}
	} END_REPEAT
}

void moveArms(int8_t direction)
{
	REPEAT_MS( armSpeed / 90 )
	{
		static byte prevAngle;
		
		if(direction == DOWN ) { 	// close
			if(angle < closePos) angle++;

			arm1.write(angle);
			arm2.write(angle);
		}
		else {					// open
			if(angle > openPos) angle --;

			arm1.write(angle);
			arm2.write(angle);
		}
		if(prevAngle != angle) {
			//Serial.print("angle = ");Serial.println(angle);
			prevAngle = angle;
		}
	} END_REPEAT
}



// STATE FUNCTIONS
StateFunction( waitCrossing )
{
	if( sm.entryState() )
	{
		Serial.println("Waiting on train") ;
	}
	if( sm.onState() )
	{
		if( occupied == 1 )
		{
			sm.exit() ;
		}
	}
	if( sm.exitState() )
	{
		Serial.println("train arrived 2") ;
	}

	return sm.endState() ;
}


StateFunction( closeCrossing )
{
	if( sm.entryState() )
	{
		digitalWrite( led1, HIGH );
		Serial.println("closing crossing") ;
	}
	if( sm.onState() )
	{
		blinkLeds( BLINK_TIME );
		moveArms( DOWN );

		if( occupied == 0 )
		{
			sm.exit() ;
		}
	}
	if( sm.exitState() )
	{
		Serial.println("train left") ;
	}

	return sm.endState() ;
}

StateFunction( openCrossing )
{
	if( sm.entryState() )
	{
		Serial.println("opening crossing") ;
	}
	if( sm.onState() )
	{
		blinkLeds( BLINK_TIME );
		moveArms( UP );

		if( angle == openPos || occupied == true)
		{ 
			sm.exit() ;
		}
	}
	if( sm.exitState() )
	{
		digitalWrite( led1, LOW ) ;
		digitalWrite( blinkLed1, LOW ) ;
		digitalWrite( blinkLed2, LOW ) ;
	}

	return sm.endState() ;
}




// STATE MACHINE
extern bool railCrossing( uint8_t occupiedTracks )		// occupied tracks is 1 if atleast 1 sensor sees a train
{

	occupied = occupiedTracks ;

	STATE_MACHINE_BEGIN( sm )
	{
		State( waitCrossing ) {
			sm.nextState(closeCrossing, 0); }

		State( closeCrossing ) {
			sm.nextState( openCrossing, 0 ); }

		State( openCrossing ) {
			if(angle == 0)	sm.nextState(waitCrossing, 0);
			else			sm.nextState(closeCrossing, 0); }
	}
	STATE_MACHINE_END( sm )
}