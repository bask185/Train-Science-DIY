#ifndef TIMERS_H
#define TIMERS_H

#include <Arduino.h>

class BaseTimer 
{
protected:
    uint32_t    presetTime ;
    uint32_t    startTime ;
    bool        startTrigger ;
    bool        endTrigger ;

public:
    bool Q ;
    BaseTimer() ;
    BaseTimer( uint32_t time ) ;
    virtual uint8_t update(uint8_t IN) = 0 ;
    void setTime( uint32_t presetTime ) ;
};

class T_ON : public BaseTimer 
{
public:
    T_ON() {}
    uint8_t update(uint8_t IN) override;
};

class T_OFF : public BaseTimer 
{
public:
    T_OFF() {}
    uint8_t update(uint8_t IN) override;
};



class BlinkTimer : public BaseTimer 
{
public:
    BlinkTimer() {}
    BlinkTimer( uint32_t ) ;
    uint8_t update(uint8_t IN) override;
};

#endif // TIMERS_H
