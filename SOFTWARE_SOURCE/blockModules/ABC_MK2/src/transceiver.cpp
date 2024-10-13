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

#include "transceiver.h"

/* TODO,
Occupancy 


*/

const int HOLD_TRAIN        =  5 ; // EXIT module flags to hold down train
const int DEPARTURE         = 10 ; // EXIT module flags departure
const int BLOCK_RED_A       = 15 ; // upline buffer sensor occupied
const int BLOCK_RED_B       = 20 ; // upline stop sensor occupied   B superseeds A
const int BLOCK_RED_BOTH    = 25 ;
const int BLOCK_YELLOW      = 30 ; // upline ABC flags that next block is occupied 
const int BLOCK_GREEN       = 35 ; // upline ABC flags track is free
const int RESPONS_OCCUPIED  = 40 ; // respond EXIT module I have a train
const int RESPONS_FREE      = 45 ; // respond EXIT module I have no train

Sender::Sender( uint8 _pin, uint8 _type )
{
    pin  =  _pin ;
    type = _type ;
}

Receiver::Receiver( uint8 _pin )
{
    pin = _pin ;
}

void Transceiver::begin()
{
    pinMode( pin, INPUT_PULLUP );
}

void Transceiver::setMyOccupancy_A( uint8 _state )
{
    myOccupancy_A = _state ;
}
void Transceiver::setMyOccupancy_B( uint8 _state ) // stop sensor
{
    myOccupancy_B = _state ;
}


void Transceiver::setHisOccupancy( uint8 _state )
{
    //hisOccupancy = _state ;
}

uint8 Transceiver::getMyOccupancy()
{
    return myOccupancy_A | myOccupancy_B ;
}

uint8 Transceiver::getHisOccupancy_A()
{
    return hisOccupancy_A  ;
}

uint8 Transceiver::getHisOccupancy_B()
{
    return hisOccupancy_B ; // 2 = stop sensor
}
// END parent class



// inherrited stuff
void Sender::transmitt()
{
    switch( state )
    {
    case startPulse:
        pinMode( pin, OUTPUT ) ;
        digitalWrite( pin, LOW ) ;

        Serial.print("\r\nSEND: ") ;
        
        if( type == ABC_MODULE )
        {
            if( myOccupancy_A && myOccupancy_B ) {
                                       pulseLength = BLOCK_RED_BOTH ; }
            else if( myOccupancy_B ) { pulseLength = BLOCK_RED_B ;  Serial.println("my STOP sensor is occupied") ; }
            else if( myOccupancy_A ) { pulseLength = BLOCK_RED_A ;  Serial.println("my BUFFER sensor is occupied") ; }
            else if(  hisOccupancy_A || hisOccupancy_B ) { 
                                       pulseLength = BLOCK_YELLOW ; Serial.println("next block is occupied") ; }
            else                     { pulseLength = BLOCK_GREEN ;  Serial.println("I and next blocks are all free") ; }
        }
        else // I am exit module
        {
            if( departure )
            {  
                pulseLength = DEPARTURE ;
                Serial.println("REQUESTING DEPARTURE") ;
            }
            else
            {
                pulseLength = HOLD_TRAIN ;
                Serial.println("SIGNALING HOLD DOWN TRAIN") ;
            }
        }

        pulseLength += 100 ; // unneeeded but doesn't hurt
        lastPulse = millis() ;  
        state = waitPulseSent ;
        break ;        

    case waitPulseSent:
        if( millis() - lastPulse >= pulseLength ) 
        {
            lastPulse = millis() ;
            
            pinMode( pin, INPUT_PULLUP ) ;

            if( type == ABC_MODULE ) state = waitNewCycle;
            else                     state = waitResponse ;
        }
        break ;

    case waitResponse:
        if( millis() - lastPulse >= 15 ) // after 15ms we will observe if the ABC is holding down the line
        {
            if( digitalRead( pin ) == LOW ) { /*hisOccupancy = 1 ;*/ Serial.println("SEND: DOWN LINE ABC REPORTS: OCCUPIED") ;}
            else                            { /*hisOccupancy = 0 ;*/ Serial.println("SEND: DOWN LINE ABC REPORTS: FREE") ;    }
            
            lastPulse = millis() ;
            state = waitNewCycle ;
        }
        break ;

    case waitNewCycle:
        if( millis() - lastPulse >= 1000 )
        {
            state = startPulse ; // restart cycle
        }
        break ;
    }
}

/*  We are receiving from upline module, this can be an ABC or EXIT module  */
void Receiver::receive()
{
    switch( state )
    {        
    case waitPulse:
        if( !digitalRead( pin ) ) // incomming pulse
        {
            lastPulse = millis() ;
            state = getPulseLength ;
        }
        break ;

    // case debouncePulse:

    case getPulseLength:
        if( digitalRead( pin ) )
        {
            uint32 pulseTime = millis() - lastPulse - 100  ;
            Serial.print("RECV: ") ;
            switch( pulseTime )
            {
                case HOLD_TRAIN:        Serial.println("HOLD_TRAIN") ;     break ;
                case DEPARTURE:         Serial.println("DEPARTURE") ;      break ;

                case BLOCK_RED_BOTH:  hisOccupancy_A = 1 ; hisOccupancy_B = 1 ; break ;
                case BLOCK_RED_A:     hisOccupancy_A = 1 ; hisOccupancy_B = 0 ; Serial.println("BLOCK_RED_BUFFER_SENSOR") ;      break ;
                case BLOCK_RED_B:     hisOccupancy_A = 0 ; hisOccupancy_B = 1 ; Serial.println("BLOCK_RED_STOP_SENSOR") ;      break ;
                case BLOCK_YELLOW:    hisOccupancy_A = 0 ; hisOccupancy_B = 0 ; Serial.println("BLOCK_YELLOW") ;   break ;
                case BLOCK_GREEN:     hisOccupancy_A = 0 ; hisOccupancy_B = 0 ; Serial.println("BLOCK_GREEN") ;    break ;
            } 

            // process pulseTime 
            if( pulseTime == DEPARTURE || pulseTime == HOLD_TRAIN )
            {
                state = sendResponse ; // signal back upline that I have a train 
            }
            else
            {
                state = waitPulse ;
            }

            //Serial.print("RCV: PULSE RECEIVED, TIME: ") ;
            //Serial.println( pulseTime ) ;
        }
        break ;

    case sendResponse:
        lastPulse = millis() ;
        
        Serial.print("RECV: TRANSMITTING RESPONSE: ") ;

        if( myOccupancy_A || myOccupancy_B )
        {
            pinMode( pin, OUTPUT ) ;
            digitalWrite( pin, LOW ) ;
            Serial.println("OCCUPIED") ;
        } else Serial.println("FREE") ;

        state = responseSent ;
        break ;

    case responseSent:
        if( millis() - lastPulse >= 30 )
        {
            pinMode( pin, INPUT_PULLUP ) ;
            state = waitPulse ;
        }
        break ;
    }
}

void Sender::setDeparture( uint8 state )
{
    departure = state ;
}