#include "src/macros.h"
#include "src/weistra.h"
#include "src/stateMachineClass.h"
#include "config.h"

// TODO HARDWARE
/*
    new version IDEAs
    levers vertical
    additional slave potentiometer socket (RJ12) for external throttle knob for kids

    2nd Hbridge. Second channel for DCC bus to control DCC accessories with. <- no bueno. should be separate module with extra levers.
    OR
    Just use push buttons with CDUs to flip switches.?

    either way record option would be nice. Use extra EEPROM on PWM throttle itself
*/
#define DEDICATED_CONTROLLER

int     setPoint ;
int     setPoint2b ;
int     prevSetPoint ;
uint8_t brakeSensor ;
uint8_t shutling ;
uint8_t lastDir ;
int8_t  entrySpeed ;
int8_t  currentSpeed ;

StateMachine sm ;
enum states
{
    runMode,
    braking,
    departing,
} ;

Weistra pwm( in1, in2, Fmin, Fmax ) ;

uint8_t     hasMaxSpeedPot ;
uint8_t     hasMinSpeedPot ;
uint8_t     hasAccelPot ;
uint8_t     hasWaitPot ;
uint8_t     hasShuttleSerice ;

void setup()
{
    pwm.begin() ;
    pwm.setCurrentSense( currentSensePin, currentLimit ) ;
    sm.setState(runMode);
    /*
    Rshunt          = 0.05R
    Imax            = lets take 2.5A
    Vmax            = 2.5 * 0.05 = 0.125V (shunt voltage max)
    1 step          = 5V / 1024 = 0,00488V/step
    so max steps    = 0.125V / 0,00488 = 25 steps
    so lets take 20 to be on the safe side.

    */

    Serial.begin(115200);
    Serial.println("booting");

// for following 3 pins, I enable pullups and I take a sample, if the sample is < then 1023, I know a potentiometer is attached
    pinMode( S1,                INPUT_PULLUP ) ;
    pinMode( S2,                INPUT_PULLUP ) ;
    pinMode( maxSpeedPin,       INPUT_PULLUP ) ;
    pinMode( minSpeedPin,       INPUT_PULLUP ) ;
    pinMode( accelFactorPin,    INPUT_PULLUP ) ;
    pinMode( waitTimePin,       INPUT_PULLUP ) ;
    pinMode( ledGreen,          OUTPUT ) ;
    pinMode( ledRed,            OUTPUT ) ;
    digitalWrite( ledGreen,       HIGH ) ;

#ifndef DEDICATED_CONTROLLER
    int sample ;
    sample = analogRead( maxSpeedPin ) ;
    if( sample < 1020 )
    {
        pinMode( maxSpeedPin, INPUT ) ;
        hasMaxSpeedPot = 1 ;
    } else Serial.println("no max speed pot");
    delay(1);
    sample = analogRead( minSpeedPin ) ;
    if( sample < 1020 )
    {
        pinMode( minSpeedPin, INPUT ) ;
        hasMinSpeedPot = 1 ;
    } else Serial.println("no min speed pot");
    delay(1);
    sample = analogRead( accelFactorPin ) ;
    if( sample < 1020 )
    {
        pinMode( accelFactorPin, INPUT ) ;
        hasAccelPot = 1 ;
    }else Serial.println("no accel/brake pot");
    delay(1);
    sample = analogRead( waitTimePin ) ; // used for adjustable time delay.
    if( sample < 1020 )
    {
        pinMode( waitTimePin, INPUT ) ;
        hasWaitPot = 1 ;
        hasShuttleSerice = 1 ;
    }  else Serial.println("no wait time pot");
#else
    hasShuttleSerice    = 1 ;
    hasAccelPot         = 1 ;
    hasMinSpeedPot      = 1 ;
    hasMaxSpeedPot      = 1 ;
#endif
}

void loop()
{
    pwm.update() ;
    control() ;

    digitalWrite(ledRed, pwm.getState() ) ; 

    // read main speed potentiometer
    
    REPEAT_MS( accelerationFactor )
    {

        setPoint2b = analogRead( speedPin ) ; // todo, we need to let this react to changes
        const int deadRange = 65 ;
        if(      setPoint2b <= (512-deadRange)){ setPoint2b = map( setPoint2b,      0, 512-deadRange, -maxSpeed, -minSpeed ) ; /*digitalWrite( ledRed, LOW ) ;*/ }// test me
        else if( setPoint2b >= (512+deadRange)){ setPoint2b = map( setPoint2b, 512+deadRange,   1023,  minSpeed,  maxSpeed ) ; /*digitalWrite( ledRed, LOW ) ;*/ }
        else                                   { setPoint2b = 0 ;                                                              /*digitalWrite( ledRed,HIGH ) ;*/ }

        if( setPoint2b != prevSetPoint )
        {  
            setPoint = prevSetPoint = setPoint2b ;
        }
        

    // sample    0      512-50   512   512+50      1023
    // speed  -100   <->   0      0       0   <->   100  // dead zone in middle
            // if potentiometer changes, abort shuttle service at once (should discard sensors)?

        if( setPoint != currentSpeed )
        {
            digitalWrite( ledRed, !digitalRead(ledRed));
            if( currentSpeed < setPoint ) currentSpeed ++ ;
            if( currentSpeed > setPoint ) currentSpeed -- ;

            if( currentSpeed > -minSpeed && currentSpeed < minSpeed )
            {
                pwm.setSpeed( 0 ) ; 
            }
            else
            {
                pwm.setSpeed( currentSpeed ) ;
                //analogWrite( ledGreen, 255-abs(currentSpeed) ) ;
            }
            
            //sm.nextState( runMode, 3000 ) ; // go to runmode and lockout sensor for 3 seconds   
        }
    }
    END_REPEAT

    REPEAT_MS( 500 ) // 1000ms sample rate suffices
    {
        int sample ;
        if( hasMaxSpeedPot )
        {
            sample = analogRead( maxSpeedPin ) ;
            maxSpeed = map( sample, 0, 1023, 100, 20 ) ;
        } 
        if( hasMinSpeedPot )
        {
            sample = analogRead( minSpeedPin ) ;
            minSpeed = map( sample, 0, 1023, 30, 0 ) ;
        } 
        if( hasAccelPot )
        {
            sample = analogRead( accelFactorPin ) ;
            accelerationFactor = map( sample, 0, 1023, 100, 10 ) ; // CHECK ME
        }
        if( hasWaitPot )
        {
            sample = analogRead( waitTimePin ) ;
            waitTime = map( sample, 0, 1023, 60000, 5000 ) ; // 5-60 seconds
        }
    }
    END_REPEAT
}

StateFunction( runMode )
{
    digitalWrite( ledGreen, HIGH) ;
    if( digitalRead( S1 ) == 0 || digitalRead( S2 ) == 0 ) return 1 ;
    
    return 0 ;
}

StateFunction( braking )
{
    if( sm.entryState() )
    {
        digitalWrite( ledGreen, LOW ) ;
        entrySpeed = pwm.getSpeed() ; // store with which speed we started to brake.
        setPoint = 0 ;
    }
    if( sm.onState() )
    {
        if( pwm.getSpeed() == 0 ) sm.exit() ; 
    }
    if( sm.exitState() )
    {
    }

    return sm.endState() ;
}

StateFunction( departing )
{
    if( sm.entryState() )
    {
        setPoint = -entrySpeed ;
    }
    if( sm.onState() )
    {
        if( pwm.getSpeed() == -entrySpeed ) sm.exit() ;   
    }

    if( sm.exitState() )
    {
    }

    return sm.endState() ;
}


uint8_t control()
{
    STATE_MACHINE_BEGIN( sm )
    {
        State( runMode ) {
            sm.nextState( braking, 0 ) ; }
    
        State( braking ) {
            sm.nextState( departing,/* waitTime*/2000 ) ; }
    
        State( departing ) {
            sm.nextState( runMode , 1000 ) ; } // 8 seconds lockout for sensors
    }
    STATE_MACHINE_END( sm )
}
