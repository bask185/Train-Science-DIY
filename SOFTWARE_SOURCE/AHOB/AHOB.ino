#include "config.h"
#include "railCrossing.h"
#include "track.h"

const int trackPins[][2] =                                                      // pin numbers for the track objects
{
    { detector1, detector2 } ,                                                  // track 1
    { detector3, detector4 } ,                                                  // track 2
    { detector5, detector6 } ,                                                // track 3 add as many as you want
 } ;

 const int nTracks = sizeof(trackPins) / sizeof(trackPins[0]) ;

Track track[nTracks] ;

void setup()
{
    Serial.begin(115200);
    Serial.println(F("AHOB STARTED")) ;

    for( int i = 0 ; i < nTracks ; i ++ )
    {
        uint8_t pin1 = trackPins[i][0] ;                                        // get pins from array and pass them to all track objects
        uint8_t pin2 = trackPins[i][1] ;
        track[i].begin( pin1, pin2 ) ;
    }
    railCrossingInit();                                                         // initialize state machine for rail crossing
}

void loop()
{
    uint8_t occupiedTracks = 0 ;

    for( int i = 0 ; i < nTracks ; i ++ )
    {
        if( track[i].readSensors() != 0 ) occupiedTracks = 1 ;                  // if atleast one track detects a train, set this to 1
    }

    railCrossing( occupiedTracks );
}