#include <stdio.h>
#include "altera_avalon_performance_counter.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"
#include "io.h"
#include "images.h"
#include "../memory_segment.h"
#include <altera_avalon_mutex.h>

#define DEBUG 0
#define KEY_1   SHARED_ONCHIP_BASE + SHARED_ONCHIP_SIZE_VALUE - 1 
#define KEY_2	(KEY_1 - 1)
#define KEY_3	(KEY_2 - 1)
#define KEY_4	(KEY_3 - 1)
#define KEY_5	(KEY_4 - 1)
#define KEY_6	(KEY_5 - 1)
#define HEXCHECK (KEY_6 - 1)
#define HEX (HEXCHECK-9)

#define SECTION_1 1

#define UNLOCK		0
#define LOCK		1

static char b2sLUT[] = 
{~0x40, //-
    ~0x79, //1
    ~0x24, //2
    ~0x30, //3
    ~0x19, //4
    ~0x12, //5
    ~0x02, //6
    ~0x78, //7
    ~0x00, //8
    ~0x18, //9
    ~0x3F, //0
};

#define PACK(c0, c1, c2, c3) \
    ((unsigned int)(unsigned char)(c3) << 24) | \
((unsigned int)(unsigned char)(c2) << 16) | \
((unsigned int)(unsigned char)(c1) << 8) | \
((unsigned int)(unsigned char)(c0))

void hexDis( unsigned char *ImagePtr)
{
    unsigned char BandImg[8] = {0};
    unsigned char i;
    volatile unsigned char a=0;
    for(i=0; i<8;i++)
    { a= *ImagePtr++;
      //alt_printf(" %x", a);
        BandImg[i]= (a);
     //alt_printf("\n BandImg=[%x]",BandImg[i]);
    }
    IOWR_ALTERA_AVALON_PIO_DATA(HEX7_HEX4_BASE,PACK(b2sLUT[BandImg[3]],b2sLUT[BandImg[2]],b2sLUT[BandImg[1]],b2sLUT[BandImg[0]]));
    IOWR_ALTERA_AVALON_PIO_DATA(HEX3_HEX0_BASE,PACK(b2sLUT[BandImg[7]],b2sLUT[BandImg[6]],b2sLUT[BandImg[5]],b2sLUT[BandImg[4]]));
}

void grayScale( unsigned char* base,unsigned int check)
{
    unsigned int i,r=0,g=0,b=0,x=0,y=0,z=0,xDIM,yDIM,maxClr,size,red,blue,green;
    unsigned char* shared;
    unsigned int grayscale[1600];
    if(check=1)
        shared = (unsigned char*) GRAYIMAGE_DATA_1;
    else if(check=2)
        shared = (unsigned char*) GRAYIMAGE_DATA_2;
    else
        shared = (unsigned char*) GRAYIMAGE_DATA_3;

    xDIM = *base++;
    yDIM = *base++;
    size= xDIM*yDIM;
    maxClr = *base++;
    //printf("\n Xdim =[%d], yDIM=[%d], maxClr =[%d]\n",xDIM, yDIM,maxClr);
    *shared++ = xDIM;
    *shared++ = yDIM;

    for(i=0;i< size;i++)
    {
        r= *base;
        g= *(base+1);
        b= *(base+2);
        red=(r>>4)+(r>>2);
        green=(g>>4)+(g>>1);
        blue=(b>>3);
        base = base + 3;
#if 0
        grayscale[i]=(unsigned int)(red+green+blue);
#else
        *shared++ = (unsigned int)(red+green+blue);
#endif
        
    }
    

} 



int main()
{ unsigned int i=0;


    char current_image=0;	
#if DEBUG == 1
    /* Sequence of images for testing if the system functions properly */
    char number_of_images=10;
    unsigned char* img_array[10] = {img1_24_24, img1_32_22, img1_32_32, img1_40_28, img1_40_40, 
        img2_24_24, img2_32_22, img2_32_32, img2_40_28, img2_40_40 };
#else
    /* Sequence of images for measuring performance */
    char number_of_images=3;
    unsigned char* img_array[3] = {img1_32_32, img2_32_32, img3_32_32};
#endif
    IOWR_8DIRECT(KEY_1,0,UNLOCK);
    IOWR_8DIRECT(KEY_2,0,LOCK);
    IOWR_8DIRECT(KEY_3,0,LOCK);
    IOWR_8DIRECT(KEY_4,0,LOCK);
    IOWR_8DIRECT(KEY_5,0,LOCK);
    IOWR_8DIRECT(KEY_6,0,LOCK);
    IOWR_8DIRECT(HEXCHECK,0,LOCK);

    //printf("\n\r\t Hello : PROCESSOR 1 \n\r");

    alt_mutex_dev* mutex_A  = altera_avalon_mutex_open( "/dev/mutex_0" );

    while (1)
    { 


        if((IORD_8DIRECT(KEY_1,0)) == UNLOCK)
        {
            i++;
            if(i>3) i=1;
            altera_avalon_mutex_lock(mutex_A,1);

            IOWR_8DIRECT(KEY_1,0,LOCK);

          PERF_START_MEASURING (PERFORMANCE_COUNTER_0_BASE);
           PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, SECTION_1);
            grayScale(img_array[current_image],i); 

            IOWR_8DIRECT(KEY_2,0,UNLOCK);
            altera_avalon_mutex_unlock(mutex_A);

            if((IORD_8DIRECT(HEXCHECK,0))== UNLOCK)
                hexDis(HEX);

            if((IORD_8DIRECT(KEY_6,0)) == UNLOCK)

            {   

                PERF_END(PERFORMANCE_COUNTER_0_BASE, SECTION_1);
                /* Print report */
               perf_print_formatted_report
                    (PERFORMANCE_COUNTER_0_BASE,            
                     ALT_CPU_FREQ,        // defined in "system.h"
                     1,                   // How many sections to print
                     "throughput"        // Display-name of section(s).
                    );
                PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
              
            }
        }

        current_image=(current_image+1) % number_of_images;

    }

    return 0;
}
