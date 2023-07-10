
#include "src/macros.h"
#include "src/NmraDcc.h"
#include "src/ServoSweep.h"

NmraDcc dcc ;

void setup()
{
    dcc.init( MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 ) ;
}

void loop()
{
    dcc.process() ;
}