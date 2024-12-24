#include "LedBlink.h"

LedBlink::LedBlink(uint8_t pin) :
    _pin(pin),
    _mode(MODE_OFF),
    _bleepCount(0),
    _cycleTime(2000),
    _bleepOnTime(100),
    _lastUpdate(0),
    _cycleStart(0),
    _ledState(false),
    _inEventMode(false),
    _eventBleeps(0)
{
}

void LedBlink::begin()
{
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    _cycleStart = millis();
}

void LedBlink::setCycleTime(uint32_t ms)
{
    _cycleTime = ms;
    _cycleStart = millis();
}

void LedBlink::setBleepOnTime(uint32_t ms)
{
    _bleepOnTime = ms;
}

void LedBlink::bleep(uint8_t count)
{
    _mode = MODE_BLEEPS;
    _bleepCount = count;
    _inEventMode = false;
}

void LedBlink::turn( uint8_t _state )
{
    _inEventMode = false;
    if( _state == OFF ) _mode = MODE_OFF;
    else                _mode = MODE_CONTINUOUS_ON;
}

void LedBlink::blinkSlow()
{
    _mode = MODE_TOGGLE_SLOW;
    _inEventMode = false;
}

void LedBlink::blinkFast()
{
    _mode = MODE_TOGGLE_FAST;
    _inEventMode = false;
}

void LedBlink::handleEvent(uint8_t eventBleeps)
{
    _inEventMode = true;
    _eventBleeps = eventBleeps;
    _cycleStart = millis(); // start event cycle now
}

void LedBlink::clearEvent()
{
    _inEventMode = false;
    _eventBleeps = 0;
    _cycleStart = millis();
}

void LedBlink::update()
{
    uint32_t now = millis();
    if(now - _lastUpdate < 50) return;
    _lastUpdate = now;

    uint32_t cyclePos = (now - _cycleStart) % _cycleTime;

    if(_inEventMode)
    {
        updateEventMode(now, cyclePos);
    }
    else
    {
        updateNormalMode(now, cyclePos);
    }
}

void LedBlink::updateNormalMode(uint32_t now, uint32_t cyclePos)
{
    switch(_mode)
    {
        case MODE_OFF:
            setLed(false);
            break;

        case MODE_CONTINUOUS_ON:
            setLed(true);
            break;

        case MODE_BLEEPS:
            if(_bleepCount > 0)
            {
                uint32_t interval = _cycleTime / _bleepCount;
                bool on = false;
                for(uint8_t i=0; i<_bleepCount; i++)
                {
                    uint32_t start = i * interval;
                    if(cyclePos >= start && cyclePos < start + _bleepOnTime)
                    {
                        on = true;
                        break;
                    }
                }
                setLed(on);
            }
            else
            {
                setLed(false);
            }
            break;

        case MODE_TOGGLE_SLOW:
        {
            // Slow toggle: bv 5 toggles per cycle
            uint8_t toggles = 5;
            uint32_t toggleInterval = _cycleTime / toggles;
            uint32_t index = cyclePos / toggleInterval;
            // Om de interval aan/uit
            bool on = (index % 2 == 0);
            setLed(on);
        }
        break;

        case MODE_TOGGLE_FAST:
        {
            // Fast toggle: bv 20 toggles per cycle
            uint8_t toggles = 20;
            uint32_t toggleInterval = _cycleTime / toggles;
            uint32_t index = cyclePos / toggleInterval;
            bool on = (index % 2 == 0);
            setLed(on);
        }
        break;
    }
}

void LedBlink::updateEventMode(uint32_t now, uint32_t cyclePos)
{
    // Event mode: we tonen eventBleeps net als bleep mode,
    // maar ignoreren huidige mode/preset.
    if(_eventBleeps == 0)
    {
        // Geen event bleeps ingesteld -> uit
        setLed(false);
        return;
    }

    uint32_t interval = _cycleTime / _eventBleeps;
    bool on = false;
    for(uint8_t i=0; i<_eventBleeps; i++)
    {
        uint32_t start = i * interval;
        if(cyclePos >= start && cyclePos < start + _bleepOnTime)
        {
            on = true;
            break;
        }
    }
    setLed(on);

    // Als je wilt dat event mode maar 1 cycle duurt:
    // kan je hier checken of cyclePos bijna weer 0 is en dan clearEvent()
    // Voor nu laten we het doorlopen tot expliciet clearEvent() wordt aangeroepen.
}

void LedBlink::setLed(bool on)
{
    if(on != _ledState)
    {
        _ledState = on;
        digitalWrite(_pin, on ? HIGH : LOW);
    }
}
