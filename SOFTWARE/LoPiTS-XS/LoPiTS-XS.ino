#include "src/NmraDcc.h"
#include "src/weistra.h"

NmraDcc dcc ;
Weistra pwm( 1, 50, 100 ) ;

void BasicAccDecoderPacket_Handler( int address, boolean activate, byte data ) 
{
    address -= 1;
    address *= 4;
    address += 1;
    address += (data & 0x06) >> 1;
}

void setup()
{
    dcc.init( MAN_ID_DIY, 10, 0, 0 ) ;
    pwm.begin() ;
}

void loop()
{
    dcc.process() ;
    pwm.update() ;
}

void notifyDccSpeed( uint16_t Addr, DCC_ADDR_TYPE AddrType, uint8_t Speed, DCC_DIRECTION Dir, DCC_SPEED_STEPS SpeedSteps )
{
  #ifdef DEBUG_SPEED
  Serial.print("notifyDccSpeed: Addr: ");
  Serial.print(Addr,DEC);
  Serial.print( (AddrType == DCC_ADDR_SHORT) ? "-S" : "-L" );
  Serial.print(" Speed: ");
  Serial.print(Speed,DEC);
  Serial.print(" Steps: ");
  Serial.print(SpeedSteps,DEC);
  Serial.print(" Dir: ");
  Serial.println( (Dir == DCC_DIR_FWD) ? "Forward" : "Reverse" );
  #endif

//   newDirection = Dir;
//   newSpeed = Speed;
//   numSpeedSteps = SpeedSteps;
};

// This call-back function is called whenever we receive a DCC Function packet for our address 
void notifyDccFunc(uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState)
{
  #ifdef DEBUG_FUNCTIONS
  Serial.print("notifyDccFunc: Addr: ");
  Serial.print(Addr,DEC);
  Serial.print( (AddrType == DCC_ADDR_SHORT) ? 'S' : 'L' );
  Serial.print("  Function Group: ");
  Serial.print(FuncGrp,DEC);
  #endif

  if(FuncGrp == FN_0_4)
  {
    //newLedState = (FuncState & FN_BIT_00) ? 1 : 0;
    #ifdef DEBUG_FUNCTIONS
    Serial.print(" FN 0: ");
    Serial.print(newLedState);
    #endif
  }
  #ifdef DEBUG_FUNCTIONS
  Serial.println();
  #endif
}