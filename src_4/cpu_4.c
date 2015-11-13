#include <stdio.h>
#include "system.h"
#include <altera_avalon_mutex.h>
#include "io.h"
#include "../memory_segment.h"

#define KEY_1   SHARED_ONCHIP_BASE + SHARED_ONCHIP_SIZE_VALUE - 1 
#define KEY_2	(KEY_1 - 1)
#define KEY_3	(KEY_2 - 1)
#define KEY_4	(KEY_3 - 1)
#define KEY_5	(KEY_4 - 1)
#define KEY_6	(KEY_5 - 1)
#define HEXCHECK (KEY_6 - 1)

#define UNLOCK		0
#define LOCK		1

void ascii(unsigned char *ImagePtr, unsigned int check)
{
 unsigned char symbols[16] = { '@', '%','8','&', '#', '*','o','a','h', '?','+', '=', '-', ':', '.', ' ' };
unsigned char n,m, i,j,saturationLevel = 0;
char  symbol =0;
unsigned int  image2[20][20];
volatile unsigned int xDim,yDim;
xDim = *ImagePtr++ ;
    //alt_putstr("\n Entering Forloop13");
yDim = *ImagePtr++ ;
  printf("\n ");
for (n = 1; n < yDim-1; n++)
    {
        for (m = 1; m <xDim-1; m++)
        {
            image2[n][m]=*ImagePtr++;
           
        }
    }
for (i = 1; i < yDim-1; i++)
        { 
            for (j = 1; j < xDim-1; j++)
            {
		saturationLevel = (unsigned char)((image2[i][j])>>4);
                symbol = symbols[saturationLevel];
	alt_printf("%c",symbol);
            }
        alt_printf("\n");
        }
}

int main()
{   
    unsigned int i,k=0;
 
   for(i=0;i<3000;i++){;}	//delay, so that processor 1 can initialise

	//alt_printf("\n\r\t Hello : PROCESSOR 4 \n\r");
	
	alt_mutex_dev* mutex_E  = altera_avalon_mutex_open( "/dev/mutex_4" );

    while(1)
    {  
        if((IORD_8DIRECT(KEY_5,0)) == UNLOCK)
        {   
            k++;
            if (k>3) k=1;
            altera_avalon_mutex_lock(mutex_E,1);
            IOWR_8DIRECT(KEY_5,0,LOCK);
            if(k=1)
            ascii(SOBEL_IMAGE_DATA_1,k);
            else if (k=2)
            ascii(SOBEL_IMAGE_DATA_2,k);
            else if (k=3)
            ascii(SOBEL_IMAGE_DATA_3,k);
            
            IOWR_8DIRECT(KEY_6,0,UNLOCK);
            altera_avalon_mutex_unlock(mutex_E);
           
        }
    }
    return 0;
}
