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
  
unsigned int SquareRoot(unsigned int n)  
{  
        unsigned int c = 0x8000;  
            unsigned int g = 0x8000;  
              
    while(1)
    {  
        if(g*g > n)
        {       
          g ^= c;  
        }
        c >>= 1;  
        if(c == 0)
     {       
       return g;  }       
       g |= c; 
             }  
}


void appSobelFilter(unsigned char *ImagePtr,unsigned int check)
{ 
  unsigned char x = 0, y = 0,n=0,m=0;
  volatile unsigned int xDim,yDim;
  unsigned int  image1[20][20];
  unsigned int  image2[20][20];
  unsigned char* shared;
  if(check=1)
    shared = (unsigned char*) SOBEL_IMAGE_DATA_1;
    else if(check=2)
    shared = (unsigned char*) SOBEL_IMAGE_DATA_2;
    else if(check=3)
    shared = (unsigned char*) SOBEL_IMAGE_DATA_3;
  
   int Gx = 0, Gy = 0, pixel=0;
   
   //alt_putstr("\n Entering Forloop12");
    xDim = *ImagePtr++ ;
    //alt_putstr("\n Entering Forloop13");
    yDim = *ImagePtr++ ;
    //alt_putstr("\n Entering Forloop14");

    //alt_printf("\n Entering Forloop[%x], %x,%x",++test,xDim,yDim);
 for (n = 0; n < yDim; n++)
    {
        for (m = 0; m <xDim; m++)
        {
            image1[n][m]=*ImagePtr++;
        }
    }

    for (y = 0; y < yDim;y++)
    {      
	for (x = 1; x < xDim; x++)
        {

            Gx = (-(image1[y - 1][x - 1])
                    + (image1[y - 1][x + 1])
                    - ((image1[y + 0][x - 1])<<1)
                    + (( image1[y + 0][x + 1])<<1)
                    - (image1[y + 1][x - 1])
                    + (image1[y + 1][x + 1]));

            Gy = (-(image1[y - 1][x - 1])
                    + (image1[y + 1][x - 1])
                    - ((image1[y - 1][x + 0])<<1)
                    + ((image1[y + 1][x + 0])<<1)
                    - (image1[y - 1][x + 1])
                    + (image1[y + 1][x + 1]));


             /*if ( Gx < 0) Gx= -Gx;
             if(Gy <0 ) Gy= -Gy;
             pixel= ((Gx+Gy)>>1);*/
            pixel = (unsigned int)(SquareRoot((unsigned int)((Gx * Gx) + (Gy * Gy))));
            if(pixel > 255) pixel= 255;
               image2[y][x] = pixel;
            // alt_printf(" %x", image2[y][x]);
              pixel=0; Gx=0; Gy=0;
           
        }

    }
   
*shared++ = xDim;
*shared++ = yDim;
    for (n = 1; n < yDim-1; n++)
    { 
        for (m = 1; m <xDim-1; m++)
        {
          *shared++ = image2[n][m];
        }
   }
}

int main()
{   
    unsigned int i ,k=0;
 
   for(i=0;i<6000;i++){;}	//delay, so that processor 1 can initialise

	//alt_printf("\n\r\t Hello : PROCESSOR 4 \n\r");
	
	alt_mutex_dev* mutex_D  = altera_avalon_mutex_open( "/dev/mutex_3" );

    while(1)
    { 
        if((IORD_8DIRECT(KEY_4,0)) == UNLOCK)
        {    k++;
            if (k>3) k=1;
            
            altera_avalon_mutex_lock(mutex_D,1);
            IOWR_8DIRECT(KEY_4,0,LOCK);
            if(k=1)
            appSobelFilter(RESIZE_IMAGE_DATA_1,k);
            else if (k=2)
            appSobelFilter(RESIZE_IMAGE_DATA_2,k);
            else if (k=3)
            appSobelFilter(RESIZE_IMAGE_DATA_3,k);
            IOWR_8DIRECT(KEY_5,0,UNLOCK);
            altera_avalon_mutex_unlock(mutex_D);
           //IOWR_8DIRECT(KEY_1,0,UNLOCK);
        }
    }
    return 0;
}
