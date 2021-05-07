#include "StdTypes.h"
#include "BitMath.h"
#include "Memory_Map.h"
#include "DIO_interface.h"
#include "_7SEG_interface.h"
#include "StopWatch.h"



/* Project1  StopWatch */
int main(void)
{
	/* Local Decelerations */

	u16 n=3311;	
	/* System Init */
	DIO_Init();
 
    while(1)
    {
		/* Super Loop */
		StopWatch();

    }
}



