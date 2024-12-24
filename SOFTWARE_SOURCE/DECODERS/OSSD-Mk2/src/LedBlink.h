#ifndef LEDBLINK_H
#define LEDBLINK_H

#include <Arduino.h>

const int OFF = 0 ;
const int  ON = 1 ;

class LedBlink
{
public:
    LedBlink(uint8_t pin);

    void begin();
    void setCycleTime(uint32_t ms);
    void setBleepOnTime(uint32_t ms);
    void bleep(uint8_t count);
    void turn( uint8_t );
    void blinkSlow();
    void blinkFast();

    void handleEvent(uint8_t eventBleeps);
    void clearEvent();

    void update();

private:
    enum Mode
    {
        MODE_OFF,
        MODE_CONTINUOUS_ON,
        MODE_BLEEPS,
        MODE_TOGGLE_SLOW,
        MODE_TOGGLE_FAST
    };

    uint8_t  _pin;
    uint8_t  _mode;
    uint8_t  _bleepCount;
    uint32_t _cycleTime;
    uint32_t _bleepOnTime;
    uint32_t _lastUpdate;
    uint32_t _cycleStart;
    bool     _ledState;

    // Event mode
    bool     _inEventMode;
    uint8_t  _eventBleeps;

    void updateNormalMode(uint32_t now, uint32_t cyclePos);
    void updateEventMode(uint32_t now, uint32_t cyclePos);
    void setLed(bool on);
};

#endif
