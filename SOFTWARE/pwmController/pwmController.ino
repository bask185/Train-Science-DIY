#include "src/macros.h"
#include "src/weistra.h"
#include "src/stateMachineClass.h"
#include "config.h"

int     setPoint ;
int     prevSetPoint ;
uint8_t brakeSensor ;
uint8_t lastDir ;
int8_t  brakeSpeed ;
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

      Serial.begin(115200);
    Serial.println("booting");

// for following 3 pins, I enable pullups and I take a sample, if the sample is < then 1023, I know a potentiometer is attached
    pinMode( maxSpeedPin,       INPUT_PULLUP ) ;
    pinMode( minSpeedPin,       INPUT_PULLUP ) ;
    pinMode( accelFactorPin,    INPUT_PULLUP ) ;
    pinMode( waitTimePin,       INPUT_PULLUP ) ;
    delay(1);

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

    pinMode( S1, INPUT_PULLUP ) ;
    pinMode( S2, INPUT_PULLUP ) ;


}

void loop()
{
    pwm.update() ;
    control() ;

    // read main speed potentiometer
    REPEAT_MS( accelerationFactor )
    {
        setPoint = analogRead( speedPin ) ;

    // sample    0      512-50   512   512+50      1023
    // speed  -100   <->   0      0       0   <->   100  // dead zone in middle
        if(      setPoint <= (512-50)) setPoint = map( setPoint,      0, 512-50, -maxSpeed,  minSpeed ) ; // test me
        else if( setPoint >= (512+50)) setPoint = map( setPoint, 512+50,   1023,  minSpeed,  maxSpeed ) ;
        else                          setPoint = 0 ;

            // if potentiometer changes, abort shuttle service at once (should discard sensors)?
      if( setPoint != currentSpeed )
      {
          
          if( currentSpeed < setPoint ) currentSpeed ++ ;
          if( currentSpeed > setPoint ) currentSpeed -- ;

          if( currentSpeed > -minSpeed && currentSpeed < minSpeed )
          {
              pwm.setSpeed( 0 ) ; 
          }
          else
          {
              pwm.setSpeed( currentSpeed ) ;
          }
          
          sm.nextState( runMode, 3000 ) ; // go to runmode and lockout sensor for 3 seconds   
      }
    }
    END_REPEAT

    REPEAT_MS( 1000 ) // 1000ms sample rate suffices
    {
        int sample ;
        if( hasMaxSpeedPot )
        {
            sample = analogRead( maxSpeedPin ) ;
            maxSpeed = map( sample, 0, 1023, 20, 100 ) ;
        } 
        if( hasMinSpeedPot )
        {
            sample = analogRead( minSpeedPin ) ;
            minSpeed = map( sample, 0, 1023, 0, 30 ) ;
        } 
        if( hasAccelPot )
        {
            sample = analogRead( accelFactorPin ) ;
            accelerationFactor = map( sample, 0, 1023, 50, 5 ) ; // CHECK ME
        }
        if( hasWaitPot )
        {
            sample = analogRead( waitTimePin ) ;
            waitTime = map( sample, 0, 1023, 5000, 60000 ) ; // 5-60 seconds
        }
    }
    END_REPEAT
}

StateFunction( runMode )
{
    if( digitalRead( S1 ) == 0 || digitalRead( S2 ) == 0 ) return 1 ;
    
    return 0 ;
}

StateFunction( braking )
{
    if( sm.entryState() )
    {
        brakeSpeed = currentSpeed ;
        if( brakeSpeed < 0 ) lastDir = 1 ;
        else                 lastDir = 0 ;
    }
    if( sm.onState() )
    {
        if( sm.repeat( accelerationFactor ) )
        {
            if(      brakeSpeed < 0 ) brakeSpeed ++ ;
            else if( brakeSpeed > 0 ) brakeSpeed -- ;
            else         sm.exit() ;   // when speed is 0 exit

            pwm.setSpeed( brakeSpeed ) ;
        }
    }

    return sm.endState() ;
}

StateFunction( departing )
{
    if( sm.repeat( accelerationFactor ) )
    {
        if( lastDir ) brakeSpeed -- ;
        else          brakeSpeed ++ ;
        pwm.setSpeed( brakeSpeed ) ;

        if( brakeSpeed == -maxSpeed 
        ||  brakeSpeed ==  maxSpeed ) sm.exit() ;
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
            sm.nextState( departing, waitTime ) ; }
    
        State( departing ) {
            sm.nextState( runMode , 8000 ) ; } // 8 seconds lockout for sensors
    }
    STATE_MACHINE_END( sm )
}
