#include <Arduino.h>
#include "type.h"

#ifndef CONFIG_H
#define CONFIG_H

const int GPIO[] =
{
     7,  6,  5, 10,
    11, 12,  4,  3,
     1,  0, A5, A4,
    A3, A3, A2, A0,
} ;

enum 
{
    SERVO_DECODER,
    COIL_DECODER,
    RELAY_DECODER,
    SIGNAL_DECODER,
    FEEDBACK_DECODER,
} ;

#if defined SERVO_W_RELAY
#include "src/ServoSweep.h"
    const int NGPIO = 6 ;           // default amount
    const int type = SERVO_DECODER ;
    #define CONSTRUCT ServoSweep
    #define IS_SERVO

#elif defined SERVO
#include "src/ServoSweep.h"
    const int NGPIO = 12 ;
    const int type = SERVO_DECODER ;
    #define CONSTRUCT ServoSweep
    #define IS_SERVO

#elif defined COILS
#include "src/CoilDrive.h"
    const uint8_t NGPIO = 8 ;
    const int type = COIL_DECODER ;
    #define CONSTRUCT CoilDrive
    
#elif defined RELAY
#include "src/Relay.h"
    const int NGPIO = 8 ;
    const int type = RELAY_DECODER ;
    #define CONSTRUCT Relay

#elif defined SIGNAL
#include "src/Signal.h"
    const int NGPIO = 5 ;
    const int type = SIGNAL_DECODER ;
    #define CONSTRUCT Signal

#elif defined OCCUPANCY || defined POINT_CONTROLLER
#include "src/Occupancy.h"
    const int NGPIO = 16 ;
    const int type  = FEEDBACK_DECODER ;
    #define CONSTRUCT Occupancy
    #define IS_INPUT

#else
    #error nothing defined


#endif

#endif // include guard

extern void loadEeprom() ;

typedef struct Settings
{
    uint16_t    myAddress ;
    uint8_t     servoMin[NGPIO] ;
    uint8_t     servoMax[NGPIO] ;
    uint8_t     aspectType[NGPIO] ;
    uint8_t     DCC_type ;              // normal or extended
    uint8_t     coilSwitchTime ;
    uint8_t     nGpio ;
    uint8_t     inputType ;             // must I send feedback or switch accessories.
    uint8_t     defaults ;
} ;

extern Settings settings ;

void send2config( uint16_t ) ;
void setF1( uint8_t ) ;
void setF2( uint8_t ) ;
void configure() ;
void setServoPos( uint8_t ) ;
void setIndex( uint8_t ) ;