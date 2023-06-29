#include "src/macros.h"
#include "src/weistra.h"
#include "src/XpressNetMaster.h"
#include "config.h"


Weistra pwm( in1, in2, Fmin, Fmax ) ; // pin 10 and 11 for direction, Fmin = 50, Fmax = 100

#ifdef HAS_XPRESSNET
XpressNetMasterClass Xnet ;
#endif

void setup()
{
    pwm.begin() ;
    pwm.setCurrentSense( currentSensePin, currentLimit ) ;

#ifdef HAS_XPRESSNET
    Xnet.setup( Loco128, rs485dir ) ;
#endif
}

void loop()
{
    pwm.update() ;

#ifdef HAS_XPRESSNET
    Xnet.setup( Loco128, rs485dir ) ;                       // either use Xnet
#else

    REPEAT_MS( accelerationFactor )                         // or use potentiometer
    {
        int setPoint = analogRead( speedPin ) ;
        static int currentSpeed = 0 ;

        setPoint = map( setPoint, 0, 1023, -maxSpeed, maxSpeed ) ;

        if( setPoint > currentSpeed ) currentSpeed ++ ;
        if( setPoint < currentSpeed ) currentSpeed -- ;

        pwm.setSpeed( currentSpeed ) ;

    }
    END_REPEAT
#endif
}

