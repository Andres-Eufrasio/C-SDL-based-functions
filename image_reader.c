#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "structs.c"
#include <png.h>

/*
/*load a png and jpeg files contents and and return values
by Andres Eufrasio Tinajero
created 30/01/26

TODO:
change malloc to have coninous memory
*/



/*Read a jpeg file and return back a rgb data and size


*/
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









//png reader that takes info form a png and converts it into 255RGBa format and returns it back
raw_image_data read_png_file(char *imageName) {
    raw_image_data data;
    FILE *pPng = fopen(imageName, "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png){
        printf("THAT IS NOT A PNG!");
        abort();
    }
    
    png_infop info = png_create_info_struct(png); 
    if (!info){
        abort();
    }

    if (setjmp(png_jmpbuf(png))) {
        abort();
    }
    png_init_io(png, pPng);
    png_read_info(png,info);




    int height = png_get_image_height(png, info);
    int width = png_get_image_width(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);
    png_byte color_type = png_get_color_type(png, info);
    //Convert to 8bit png
    
    if(bit_depth == 16) png_set_strip_16(png);
    if(color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if(png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);
    
    
    //re-read after adjusting to rgb 255
    png_read_update_info(png, info);


    
    png_bytep *pimage_data = NULL;

    // setup image structure
    pimage_data= (png_bytep*)malloc(sizeof(png_bytep)*height);
    for (int y = 0; y <height; y++){
        pimage_data[y] = (png_byte*)malloc(png_get_rowbytes(png,info));     
    }
    // put details into that image structure
    png_read_image(png, pimage_data);



    
    data.height = height;
    data.width = width;
    data.pixel_buff = pimage_data;
    

    fclose(pPng);
    png_destroy_read_struct(&png, &info, NULL);
    return data;
};