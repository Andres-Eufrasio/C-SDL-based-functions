#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "structs.c"

/*Read a jpeg file and return back a rgb data and size


*/





typedef struct my_error_mgr * my_error_ptr;

raw_image_data read_jpeg_file(char * imageName){
    raw_image_data data;
    printf("1");
    FILE *pJpeg = fopen(imageName, "rb");
    if (pJpeg == NULL){
        printf("no image found");
        abort();
    }
    
    struct jpeg_decompress_struct cinfo;
    j_decompress_ptr pinfo = &cinfo;
    struct jpeg_error_mgr jerr;
    

    unsigned long bmp_size;
    JSAMPARRAY  bmp_buffer;
    int row_stride;
    printf("2");
    cinfo.err = jpeg_std_error(&jerr);	
    jpeg_create_decompress(pinfo);
    jpeg_stdio_src(pinfo,pJpeg);
    jpeg_read_header(pinfo, TRUE);
    jpeg_start_decompress(pinfo);
    printf("3");
    
    
    int width = cinfo.image_width;
    int height= cinfo.image_height;

    row_stride = width * 3;
    printf("4");
    bmp_buffer = (cinfo.mem->alloc_sarray)((j_common_ptr) pinfo, JPOOL_IMAGE, row_stride, height);
    printf("5");

    
    for (int y = 0; y <height; y++){
        jpeg_read_scanlines(pinfo , &bmp_buffer[y], 1);  
        
    }

    for (int y = 0; y <height; y++){
        for (int x = 0; x <row_stride; x++){
            
        }
    }
    
    data.width=width;
    data.height=height;
    data.pixel_buff=bmp_buffer;


    
    jpeg_destroy_decompress(pinfo);
    fclose(pJpeg);


    return data;
}


