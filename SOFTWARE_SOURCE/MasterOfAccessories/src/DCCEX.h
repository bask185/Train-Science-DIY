#ifndef DCCEX_H
#define DCCEX_H

#include <Arduino.h>

class DCCEX {
public:
    DCCEX(Stream& serial);

    void begin();
    void process();
    void setPower(uint8_t state);
    uint8_t getPower();

    // Set turnout state
    void setPoint(uint16_t address, bool state);
    void sendStatusResponse();

private:
    Stream& _serial;
    bool power;

    // Method for parsing the incoming message
    void parseMessage(char* message);
    
    // Methods for sending responses
    void sendPowerResponse(bool powerState);
    void sendOccupancyStatusResponse(uint16_t address, bool occupied);
};

    // Callbacks for turnout and occupancy
extern void NotifyDccExPointSet(uint16_t, uint8_t ) __attribute__((weak)) ;
extern void _occupancyCallback(uint16_t, uint8_t )  __attribute__((weak));

#endif
