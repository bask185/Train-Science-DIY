#include <Arduino.h>
#include "src/stateMachineClass.h"
#include "src/debounceClass.h"

class Track
{
public:
	Track() ;

	void begin( uint8_t, uint8_t ) ;
	uint8_t readSensors() ;

	bool monitorSw1Sw2F() ;
	bool waitSw1F() ;
	bool waitSw2F() ;
	bool waitTrain2passSw1F() ;
	bool waitTrain2passSw2F() ;

private:
	StateMachine sm ;

	Debounce sw1 ;
	Debounce sw2 ;
	
	uint8_t  occupied ;
	uint8_t  priority ;

	uint32_t prevTime ;
	const long debounceInterval = 20 ;

	enum states 
	{
		monitorSw1Sw2,
		waitSw1,
		waitSw2,
		waitTrain2passSw1,
		waitTrain2passSw2,
	} ;
} ;