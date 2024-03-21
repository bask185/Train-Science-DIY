#include "track.h"
#include "config.h"
#include "src/stateMachineClass.h"

Track::Track() {;}

void Track::begin( uint8_t _sw1, uint8_t _sw2 )
{
	sw1.begin( _sw1 ) ;
	sw2.begin( _sw2 ) ;image.png
}	



bool Track::monitorSw1Sw2F()
StateFunction( monitorSw1Sw2F )
{
	if( sm.entryState() )
	{
		occupied = 0;
	}
	if( sm.onState() )
	{
		if( sw1.getState() == RISING )
		{
			priority = 1 ;
			sm.exit() ;
		}
		if( sw2.getState() == RISING )
		{
			priority = 2 ;
			sm.exit() ;
		}
	}
	if( sm.exitState() )
	{
		occupied = 1;
		if(priority == 1) Serial.println("    detector 1 made first");
		else			  Serial.println("    detector 2 made first");
	}

	return sm.endState() ;
}

bool Track::waitSw1F()
{
	if( sm.entryState() )
	{
		
	}
	if( sm.onState() )
	{
		if( sw1.getState() == RISING )
		{
			sm.exit() ;
		}
	}
	if( sm.exitState() )
	{
		Serial.println("    train reached detector 1");
	}

	return sm.endState() ;
}

bool Track::waitSw2F()
{
	if( sm.entryState() )
	{
		
	}
	if( sm.onState() )
	{
		if( sw2.getState() == RISING )
		{
			sm.exit() ;
		}
	}
	if( sm.exitState() )
	{
		Serial.println("    train reached detector 2");
	}

	return sm.endState() ;
}

bool Track::waitTrain2passSw1F()
{
	if( sm.entryState() )
	{
	}
	if( sm.onState() )
	{
		if( sw1.getState() == RISING )
		{
			sm.exit() ;
		}
	}
	if( sm.exitState() )
	{
		Serial.println("    train has passed detector 1");
	}

	return sm.endState() ;
}

bool Track::waitTrain2passSw2F()
{
	if( sm.entryState() )
	{

	}
	if( sm.onState() )
	{
		if( sw2.getState() == RISING )
		{
			sm.exit() ;
		}
	}
	if( sm.exitState() )
	{
		Serial.println("    train has passed detector 2");
	}

	return sm.endState() ;
}


// STATE MACHINE
uint8_t Track::readSensors() 
{
	if( millis() - prevTime >= debounceInterval )
	{   prevTime = millis() ;

		sw1.debounceInputs() ;
		sw2.debounceInputs() ;
	}

	STATE_MACHINE_BEGIN(sm)
	{
		State( monitorSw1Sw2 ) {
			if( priority == 2 )  sm.nextState( waitSw1, 2 );
			else				 sm.nextState( waitSw2, 2 ); }

		State( waitSw1 ) {
			sm.nextState( waitTrain2passSw1 , 0 ) ; }

		State( waitSw2 ) {
			sm.nextState( waitTrain2passSw2 , 0 ) ; }

		State( waitTrain2passSw1 ) {
			sm.nextState( monitorSw1Sw2 , 0 ) ; }

		State( waitTrain2passSw2 ) {
			sm.nextState( monitorSw1Sw2 , 0 ) ; }
	}
	STATE_MACHINE_END(sm)
}
