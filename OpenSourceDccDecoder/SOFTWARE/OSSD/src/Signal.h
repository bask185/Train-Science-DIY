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

extern Aspect aspects[] ;


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

/* humanizing:
we hebben tot max 8 seinen. Elk sein heeft een type, hiermee kies ik uit de array.
Uit de array haal ik aantal leds en het aantal mogelijke seinbeelden (aspects)

tevens heeft elk sein naast een type ook een begin pin. Het is natuurlijk afhankelijk van de volgorde en welke type seinen in welke volgorde staan.
Tijdens run time, set je de variabele aspect om het goede beeld te tonen. De methode update() moet de seinen daadwerkelijk aansturen

Dat betekent dat elk signal object minstens deze 3 private variabelen heeft. Type, huidige seinbeeld(aspect) en begin pin.
Voor het knipperen is een prevTime ook nodig en misschien komt er nog een interval in de structs.

Om het leven makkelijker te maken, krijgt elk sein object ook het aantal leds. Anders moet ik dat blijven opvragen uit de struct en het is makkelijker zo

Tijdens configuraties, worden de types en de begin pinnen uitgedeeld aan de objecten. 

De nodige methods zijn:
 begin/init
 update
 get led count

 */