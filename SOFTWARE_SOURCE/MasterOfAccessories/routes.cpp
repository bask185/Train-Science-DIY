#include "routes.h"
#include "src/i2cEeprom.h"

I2cEeprom eeprom( 0x28 ) ; // I am wrong, fixme

const int     maxRoutes = 256 ; // may need to go to header
const int pointPerRoute =  16 ;

uint8   nRegister ;

static uint8     firstButton ;
static uint8    secondButton ;
static uint8   settingPoints ;
static uint8      pointIndex ;
static uint16     routeIndex ;
static uint16       oldIndex ;

struct ButtonIndex
{
    uint8  firstButton ;
    uint8 secondButton ;
    uint8        index ;
} ;

struct Route
{
    uint16 address[pointPerRoute] ; // with one array, is struct needed?
    // todo add DCC EXT
} ;

struct Accessory
{
    uint16 address : 12 ;
    uint16   state :  1 ;
    uint16   isExt :  1 ;
    
    // uint8      ext :  8 ; todo
} ;

Route route ; // route can have a dynimcally allocated eeprom address which can be found with above button index. There are initially 16 items per thing. And I just may add DCC extended things as well.


enum
{
    idle,
    addPoints2street,
    commitRoute,
    awaitIndex,
    awaitFirst,
    awaitSecond,
    processButtons,
    runMode,
    teachinMode,
    configMode,
    setIoModules,
} ;

struct StateMachine
{
    uint8 state ;
    StateMachine(uint8_t initialState) : state(initialState) {}
} ;

static StateMachine teach( idle ) ;
static StateMachine layingRoute( idle ) ;
static StateMachine button( awaitFirst ) ;
static StateMachine mainRoutine( runMode ) ;



const int ee_IOMODULES = 800 ;
// 256 x 3 bytes starting from 0 yields
// 0 -  767 byts used for array index

// 800- 1000 reserved special purpose
const int route_offset_address = 1000 ;


int16 findIndex( uint8 firstButton, uint8 secondButton ) // DONE
{
    for( int i = 0 ; i < maxRoutes ; i ++ )
    {
        ButtonIndex idx ;
        uint16 eeAddress = i * sizeof(idx) ;

        eeprom.get( eeAddress, idx ) ;

        if(( firstButton == idx.firstButton  && secondButton == idx.secondButton )
        || ( firstButton == idx.secondButton && secondButton == idx.firstButton ) ) // if true match found. First/second button is ambiguous
        {
            return i ;
        }
    }

    return -1 ;
}

int16 newIndex( uint8 firstButton, uint8 secondButton )  // DONE
{
    int16 index = findIndex( firstButton, secondButton ) ;

    if( index != -1 ) // buttons already used this index, so use this index again and overwrite old route
    {
        return index ;
    }

    else            // buttons not yet have an index so we use the first free one        
    {
        for( int i = 0 ; i < maxRoutes ; i ++ )
        {
            ButtonIndex idx ;
            uint16 eeAddress = i * sizeof(idx) ;
            eeprom.get( eeAddress, idx ) ;

            if( idx.firstButton == 255 && idx.secondButton == 255 ) // this means free
            {
                return i ;
            }
        }
    }

    return -1 ; // error all routes are in use
}



 // TODO. This code won't work as it is not called continously
 //   to fix this, sendButton should just pass the variables to globals
 //   an other function should be called continously to handle what to do

void sendButton( uint8 newButton, uint8 level ) 
{
    switch( button.state )
    {
    case awaitFirst:
        if( level == 0 ) // a route button is pressed
        {
            firstButton = newButton ; // store which
            button.state = awaitSecond ;
        }
        break ;

    case awaitSecond:
        if( level == 1 ) // a button is released, so we are working with just one button
        {
            secondButton = 0xFF ;
        }
        else
        {
            secondButton = newButton ;
        }
        //break ;

// depening on mode, we need to do separate things
// in  run mode, route buttons are used to set routes (obviously). Than we need need to check if there is a route linked to this one or both buttons
// in  teachin mode route buttons are use to teach in point. A supplementing function named teachin() is used to couple points to street
// if both 
    //case processButtons:
        if( mainRoutine.state == runMode )
        {
            routeIndex = findIndex( firstButton, secondButton ) ;

            if( routeIndex != -1 ) // route found
            {
                int eeAddress = route_offset_address + (routeIndex * sizeof(route)) ;
                eeprom.get( eeAddress, route ) ; 
            }
        }

        if( mainRoutine.state == teachinMode )
        {
            routeIndex = newIndex( firstButton, secondButton ) ;
        }

        if( mainRoutine.state == setIoModules )
        {
            if( newButton == 0 ) 
            {
                if( nRegister > 0 ) nRegister -- ;
                eeprom.put( ee_IOMODULES, nRegister ) ;
            }
            if( newButton == 1 )
            {
                if( nRegister < maxRegister ) nRegister ++ ;
                eeprom.put( ee_IOMODULES, nRegister ) ;
            }
        } 

        button.state = awaitFirst ;
        break ;
    }
}


void teachin() // to run continously
{
    switch( teach.state )
    {
    case idle:
        if( mainRoutine.state == runMode )
        {
            oldIndex   = 0xFF ;
            routeIndex = 0xFF ;

            teach.state = awaitIndex ;
        }
        break ;
    
    case awaitIndex:
        if( routeIndex != oldIndex )
        {     oldIndex  = routeIndex ;

            teach.state = addPoints2street ;
            pointIndex  = 0 ;
        }
        break ;

    case addPoints2street:
        if( mainRoutine.state != teachinMode  // if the mode is turned off, the current street also needs to be committed
        ||  routeIndex != oldIndex )   // this means that 1 or 2 new route buttons are pressed. A new route is selected, so current street must be commited
        {
            
            teach.state = commitRoute ;
        }

        break ;

    case commitRoute:
        // int eeAddress = oldIndex * whatever'
        // eeprom.put( eeAddress, street ) ;
        oldIndex  = routeIndex ;
        if( mainRoutine.state != teachinMode ) teach.state = idle ;
        else
        {
            // reset point index and such
            teach.state = addPoints2street ; 
        }
        break ;
    }
}

void settingAccessories()
{
    if( routeIndex == 0xFF ) return ;
    
    REPEAT_MS( 333 )
    {
        Accessory accessory ;

        uint16 content = route.address[pointIndex++] ;
        accessory.address =  content & 0x3FFF ;
        accessory.state   = (content >> 15) & 1 ; // check if correct bit is used, 
        // accessory.isExt   = (content >> 14) & 1 ; // TODO
        if( notifyRoutePointSet ) notifyRoutePointSet( accessory.address, accessory.state ) ;
    }
    END_REPEAT
    //state
}
