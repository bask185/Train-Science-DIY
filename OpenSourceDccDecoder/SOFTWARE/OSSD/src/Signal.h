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

//extern Aspect aspects[] ;


uint8_t getAspectAmount() ;
uint8_t getOutputAmount() ;
Aspect getAspect( uint8_t index ) ;

class Signal {

public:
    Signal() ;
    void    begin( uint8, uint8, uint8 ) ; // used during boot
    void    update() ;

    uint8   getLedCount() ;
    void    setType( uint8 ) ;
    void    setFirstIO( uint8 ) ;
    void    setAddress( uint16 ) ;
    uint8   setAspectExt( uint16, uint8 ) ;
    uint8   setAspect(    uint16, uint8 ) ;
    uint8   getAspectAmount() ;
    uint8   get1stPin() ;
    uint16  getAddress() ;
    uint16  getAddressAmount() ;

private:
    uint8   beginPin ;
    uint8   aspect ;
    uint8   nAspects ;
    uint8   nAddresses ;
    uint8   type ;
    uint8   ledCount ;
    uint16  myAddress ;
    uint32  prevTime ;
} ;