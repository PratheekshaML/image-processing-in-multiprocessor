The implementation on Image processing application in multiprocessor is done in such a way that 3 images can be processed at a time by dividing memory location into 3 parts. We have included a memory segment file, where Shared on chip memory are divided into various memory segments.
The implementation in multiprocessor is done in such a way that 3 images can be processed at a time by dividing memory location into 3 parts. We have included a memory segment file, where Shared on chip memory are divided into various memory segments.

Folders src0, src1, src2, src3 and src4 contain the codes for CPU0, CPU1, CPU2, CPU3 and CPU4 respectively.

Functions of each CPU is described as below:
CPU0 takes input of RGB image and converts image from RGB to Grayscale
CPU1 takes the Grayscale input and resizes the image.
CPU2 takes the resized image and displays the RGB values in the hexadecimal display.
CPU3 takes the resized image and converts into sobel image.
CPU4 takes the Sobel image and converts into ASCII image.
