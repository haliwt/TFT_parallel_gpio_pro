#include "bsp_plasma.h"
#include "bsp.h"


void Plasma_On(void)
{
  PLASMA_SetHigh();

}
void Plasma_Off(void)
{
   PLASMA_SetLow();


}




