#include <Arduino.h>
#include "macros.h"

const int       nSignalTypes    = 10 ;
const int       maxAspect       = 10 ;
const int       maxLeds         =  5 ;

const int       OFF             = 0b00 ;
const int        ON             = 0b01 ;
const int         X             = 0b10 ; // flash
typedef struct Aspects 
{
    const uint8_t nAspect ;
    const uint8_t nLeds ;
    uint8_t aspects[maxAspect][maxLeds] ;
} Aspect ;

extern Aspect aspect[] ;


uint8_t getAspectAmount() ;
uint8_t getOutputAmount() ;
class Signal {

public:
    Signal() ;
    void    begin(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t) ;
    void    setState( uint8_t ) ;
    void    setMode( uint8_t ) ;

private:
    uint8   pinA ;
    uint8   pinB ;
    uint8   pinC ;
    uint8   pinD ;
    uint8   state  ;
    uint8   aspect  ;
} ;