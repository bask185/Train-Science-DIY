#include "src/DCC_Decoder.h"
#include "src/weistra.h"

DCC_Decoder dcc ;
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
    dcc.SetupDecoder(1,1,2) ;
    DCC.SetBasicAccessoryDecoderPacketHandler(BasicAccDecoderPacket_Handler, true);
    pwm.begin() ;
}

void loop()
{
    dcc.loop() ;
    pwm.update() ;
}