#include <Arduino.h>
#include "src/macros.h"

const int       nSignalTypes    = 20 ;
const int       maxAspect       = 20 ;
const int       maxLeds         =  7 ;

const int       OFF             = 0b00 ;
const int        ON             = 0b01 ;
const int         X             = 0b10 ; // flash
const int         C             = 0b11 ; // coil

typedef struct Aspects 
{
    uint8 nAspect ;
    uint8 nLeds ;
    uint8 aspects[maxAspect][maxLeds] ;
} Aspect ;

//extern Aspect aspects[] ;


uint8 getAspectAmount() ;
uint8 getOutputAmount() ;

//Aspect getAspect( uint8 index ) ;

class Signal {

public:
    Signal() ;
    void    begin( uint8, uint8, uint8 ) ; // used during boot
    uint8   update() ;

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
    uint8   getType() ;

private:
    uint8   beginPin ;
    uint8   currentAspect ;
    uint8   aspectPrev ;
    uint8   nAspects ;
    uint8   nAddresses ;
    uint8   type ;
    uint8   ledCount ;
    uint16  myAddress ;
    uint32  prevTime ;

    const int interval = 1000 ; // may need to be variable in struct..

    uint8    updateCoils() ;
} ;