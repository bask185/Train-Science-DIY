/*
 * Copyright (C) 2024 Sebastiaan Knippels, Train-Science
 *
 * To the extent possible under law, the person who associated CC0 with this work
 * has waived all copyright and related or neighboring rights to this work.
 *
 * This work is published from: The Netherlands
 *
 * You can copy, modify, distribute and perform the work, even for commercial purposes,
 * all without asking permission. See the full license for details at:
 * https://creativecommons.org/publicdomain/zero/1.0/
 */

// HEADER FILES
#include <Arduino.h>
#include "railCrossing.h"
#include "src/stateMachineClass.h"
#include "src/Fader.h"
#include "config.h"
#include <Servo.h>

static StateMachine sm  ;

enum states
{
    waitCrossing ,
    closeCrossing ,
    openCrossing ,
}  ;

// VARIABLES
bool occupied  ;

uint8_t angle ;
#ifdef IS_AHOB
static Servo arm1 ;
static Servo arm2 ;
static Servo arm3 ;
static Servo arm4 ;
#endif

static Fader led1( blinkLed1 ) ;
static Fader led2( blinkLed2 ) ;
static Fader led3( blinkLed3 ) ; // on AHOB outer led on arm, AKI -> white led


// FUNCTIONS
extern void railCrossingInit(void)
{
    sm.nextState( waitCrossing, 0 )  ;

#ifdef IS_AHOB
    arm1.write(openPos) ;
    arm1.attach(servoPin1) ;
    arm2.write(openPos) ;
    arm2.attach(servoPin2) ;
    arm3.write(openPos) ;
    arm3.attach(servoPin3) ;
    arm4.write(openPos) ;
    arm4.attach(servoPin4) ;
#endif

    led1.begin() ;
    led2.begin() ;
    led3.begin() ;
}

void blinkLeds()
{
    REPEAT_MS( blinkTime ) 
    {
    #ifdef IS_AHOB
        if( occupied )              // AHOB blinking
        {
            led3.setState( 1 ) ;

            if( led1.getState() )
            {
                led1.setState( 0 ) ;
                led2.setState( 1 ) ;
            }
            else
            {
                led1.setState( 1 ) ;
                led2.setState( 0 ) ;
            }
        }
        else                        // OFF
        {
            led1.setState( 0 ) ;
            led2.setState( 0 ) ;
            led3.setState( 0 ) ;
        }
    #else
        if( occupied )              // AKI blinking
        {
            if( led1.getState() )
            {
                led1.setState( 0 ) ;
                led2.setState( 1 ) ;
                led3.setState( 0 ) ;  // led 3 (white) goes off
            }
            else
            {
                led1.setState( 1 ) ;
                led2.setState( 0 ) ;
                led3.setState( 0 ) ;  // led 3 (white) goes off
            }
        }
        else
        {
            led1.setState( 0 ) ;
            led2.setState( 0 ) ;

            if( led3.getState()) led3.setState( 0 ) ; // white LED toggles
            else                 led3.setState( 1 ) ;
        }
    #endif
    }
    END_REPEAT

    led1.update() ;
    led2.update() ;
    led3.update() ;     // if AKI, update led3 a swell
}

void moveArms()
{
    REPEAT_MS( armSpeed / 90 )
    {
        static byte prevAngle ;
        
        if( occupied ) if( angle < closePos ) angle ++ ; // close
        else           if( angle >  openPos ) angle -- ; // open

    #ifdef IS_AHOB                  // if no ahob, dont do servo
        arm1.write(angle) ;
        arm2.write(angle) ;
        arm3.write(angle) ;
        arm4.write(angle) ;
    #endif

    }
    END_REPEAT
}



// STATE FUNCTIONS
StateFunction( waitCrossing )
{
    if( sm.entryState() )
    {
        Serial.println("Waiting on train")  ;
    }

    if( occupied == 1 ) return 1 ;

    return 0 ;
}


StateFunction( closeCrossing )
{
    if( sm.entryState() )
    {
        Serial.println("closing crossing")  ;
    }

    if( occupied == 0 ) return 1 ;
    
    return 0 ;
}

StateFunction( openCrossing )
{
    if( sm.entryState() )
    {
        Serial.println("opening crossing")  ;
    }

    if( angle == openPos || occupied == true) return 1 ;
    
    return 0 ;
}




// STATE MACHINE
extern bool railCrossing( uint8_t occupiedTracks )        // occupied tracks is 1 if atleast 1 sensor sees a train
{
    blinkLeds() ;
    moveArms() ;

    occupied = occupiedTracks  ;

    STATE_MACHINE_BEGIN( sm )
    {
        State( waitCrossing ) {
            sm.nextState(closeCrossing, 0) ; }

        State( closeCrossing ) {
            sm.nextState( openCrossing, 0 ) ; }

        State( openCrossing ) {
            if(angle == 0)  sm.nextState(waitCrossing, 0) ;
            else            sm.nextState(closeCrossing, 0) ; }
    }
    STATE_MACHINE_END( sm )
}