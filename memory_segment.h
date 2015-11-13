#ifndef _MEM_SEG_H_ 
#define _MEM_SEG_H_

#define GRAY_OFFSET 0x0
#define RESIZE_OFFSET 0x1320
#define SOB_OFFSET 0x1830
#define G_IMG_OFFSET  0x660
#define R_IMG_OFFSET  0x1B0
#define S_IMG_OFFSET  0x1B0
#define BUFF_LEN 3
#define CTRL_OFFSET 0x20


/*Gray Image(s)*/
#define GRAYIMAGE_DATA_1 SHARED_ONCHIP_BASE+GRAY_OFFSET+CTRL_OFFSET
#define GRAYIMAGE_CTRL_1 SHARED_ONCHIP_BASE+GRAY_OFFSET

#define GRAYIMAGE_DATA_2 GRAYIMAGE_DATA_1 + G_IMG_OFFSET
#define GRAYIMAGE_CTRL_2 GRAYIMAGE_CTRL_1 + G_IMG_OFFSET

#define GRAYIMAGE_DATA_3 GRAYIMAGE_DATA_2 + G_IMG_OFFSET
#define GRAYIMAGE_CTRL_3 GRAYIMAGE_CTRL_2 + G_IMG_OFFSET

/*Resize Image(s)*/
#define RESIZE_IMAGE_DATA_1 SHARED_ONCHIP_BASE+RESIZE_OFFSET+CTRL_OFFSET
#define RESIZE_IMAGE_CTRL_1 SHARED_ONCHIP_BASE+RESIZE_OFFSET

#define RESIZE_IMAGE_DATA_2 RESIZE_IMAGE_DATA_1 + R_IMG_OFFSET
#define RESIZE_IMAGE_CTRL_2 RESIZE_IMAGE_CTRL_1 + R_IMG_OFFSET

#define RESIZE_IMAGE_DATA_3 RESIZE_IMAGE_DATA_2 + R_IMG_OFFSET
#define RESIZE_IMAGE_CTRL_3 RESIZE_IMAGE_CTRL_2 + R_IMG_OFFSET

/*Sobel Image(s)*/
#define SOBEL_IMAGE_DATA_1 SHARED_ONCHIP_BASE + SOB_OFFSET + CTRL_OFFSET
#define SOBEL_IMAGE_CTRL_1 SHARED_ONCHIP_BASE + SOB_OFFSET

#define SOBEL_IMAGE_DATA_2 SOBEL_IMAGE_DATA_1 + S_IMG_OFFSET
#define SOBEL_IMAGE_CTRL_2 SOBEL_IMAGE_CTRL_1 + S_IMG_OFFSET

#define SOBEL_IMAGE_DATA_3 SOBEL_IMAGE_DATA_2 + S_IMG_OFFSET
#define SOBEL_IMAGE_CTRL_3 SOBEL_IMAGE_CTRL_2 + S_IMG_OFFSET

#endif
