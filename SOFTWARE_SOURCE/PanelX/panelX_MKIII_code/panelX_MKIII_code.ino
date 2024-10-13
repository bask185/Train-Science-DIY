#include "src/macros.h"
#include "src/debounceClass.h"
#include "src/XpressNetMaster.h"

const int C =  0x0000 ;
const int S =  0x8000 ;
const int LAST = 0x7FFF ;
const int nPointsPerStreet = 12 ;

const uint16_t accessories[][nPointsPerStreet+3]
{
//  knoppen     wissels+standen          laatste
    {  1,   7,  6|S,  LAST },
    {  1,   8,  6|C, 7|C,   LAST },
    {  1,  20,  6|C, 7|C,  4|S,  3|S,  LAST },
    {  1,  21,  6|S, 5|S,  2|S,  1|S,  LAST }, 
    {  1,  22,  6|S, 5|S,  2|C,  1|S,  LAST }, 

    {  2,   8,  7|S,  LAST },
    {  2,  20,  7|S, 4|S, 3|S,  LAST },
    {  2,  21,  7|S, 4|C, 3|S, 2|S, 1|C,  LAST },
    {  2,  22,  7|S, 4|C, 3|S, 2|C, 1|C,  LAST },

    {  3,  21,  5|C, 2|S, 1|S,  LAST },
    {  3,  22,  5|C, 2|C, 1|S,  LAST },

    {  6,  18,  8|S, 9|S,  LAST },
    {  6,  19,  8|S, 9|C,  LAST },
    {  6,  20,  8|C, 4|S, 3|C,  LAST },
    {  6,  21,  8|C, 4|C, 3|C, 1|C, 2|S,  LAST },
    {  6,  22,  8|S, 4|C, 3|S, 1|C, 2|S,  LAST },

    {  7,  21,  5|S, 2|S,  1|S,  LAST }, 
    {  7,  22,  5|S, 2|C,  1|S,  LAST }, 

    {  8,  20,  4|S, 3|S,  LAST },
    {  8,  21,  4|C, 3|S, 2|S, 1|C,  LAST },
    {  8,  22,  4|C, 3|S, 2|C, 1|C,  LAST },
} ;

//variables
uint8_t     secondButton ;
uint8_t     firstButton ;
uint8_t     btnState ;
uint8_t     currentButton ;
uint8_t     pointIndex ;
uint8_t     streetIndex ;


Debounce button[] =
{
    Debounce(3) ,
    Debounce(4) ,
    Debounce(5) ,
    Debounce(6) ,
    Debounce(7) ,
    Debounce(8) ,
    Debounce(9) ,
    Debounce(10),
    Debounce(11),
    Debounce(12),
    Debounce(13),
    Debounce(A0),
    Debounce(A1),
    Debounce(A2),
    Debounce(A3),
    Debounce(A4),
    Debounce(A5),
} ;

const int  nRoutes = sizeof( accessories ) / sizeof( accessories[0] ) ; // caluclate the amount of routes in above table
const int nButtons = sizeof( button )      / sizeof( button[0] ) ;

XpressNetMasterClass Xnet ;

void readButtons()
{
    REPEAT_MS( 20 )
    {
        for( int i = 0 ; i < nButtons ; i ++ )
        {
            button[i].debounce() ;
        }
    }
    END_REPEAT

    for( int i = 0 ; i < nButtons ; i ++ )
    {
        btnState = button[i].getState() ;
        if( btnState == FALLING || btnState == RISING )
        {
            currentButton = button[i].getPin() ;
            return ;
        }
    }
    
}

enum states
{
    getFirstButton ,
    getSecondButton ,
    getIndex ,
    setRoute ,
    waitDeparture ,
    waitArrival ,
} stateMachine = getFirstButton ;

void NX()
{
    switch( stateMachine )
    {
    case getFirstButton:
        if( btnState == FALLING )
        {
            firstButton = currentButton ;
            stateMachine = getSecondButton ;
        }
        break ;

    case getSecondButton:
        if( btnState == FALLING )
        {
            secondButton = currentButton ;
            stateMachine = getIndex ;
        }
        if( btnState == RISING )
        {
            stateMachine = getFirstButton ;
            firstButton  = 0xFF ;
            secondButton = 0xFF ;
        }
        break ;

    case getIndex:
        {
        for( int i = 0 ; i < nRoutes ; i ++ )
        {
            if( (accessories[i][0] == firstButton && accessories[i][1] == secondButton)
            ||  (accessories[i][1] == firstButton && accessories[i][0] == secondButton) )
            {
                stateMachine = setRoute ;
                streetIndex = i ;
                pointIndex  = 2 ;
                return ;
            }
        }
        stateMachine = getFirstButton ;
        firstButton  = 0xFF ;
        secondButton = 0xFF ;
        }
        break ;

    case setRoute:
        REPEAT_MS( 300 )
        {
            uint8_t    state = accessories[streetIndex][pointIndex] >> 15 ;
            uint16_t address = accessories[streetIndex][pointIndex++] & 0x7FFF ;
            if( address == LAST )
            {
                stateMachine = getFirstButton ;
                firstButton  = 0xFF ;
                secondButton = 0xFF ;
            }
            else
            {
                Xnet.SetTrntPos(address, state, 1) ;
                delay(20);
                Xnet.SetTrntPos(address, state, 1) ;
            }
        }
        END_REPEAT
        break ;
    }
}

void setup()
{
    Xnet.setup( Loco128, 2 ) ;
}

byte test = 0 ;
void loop()
{
    Xnet.update() ;
    readButtons() ;
    NX() ;
}


