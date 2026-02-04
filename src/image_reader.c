#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <png.h>

/*
/*load a png and jpeg files contents and and return values
by Andres Eufrasio Tinajero
created 30/01/26

TODO:
add bmp file



/*Read a jpeg file and return back a rgb data and size


*/
typedef struct 
{
    int height;
    int width;
    int pixel_length;
    unsigned char * pixel_buff;
}raw_image_data;


/*Read and return contents of jpeg file in SDL_PIXELFORMAT_RGB24


*/
raw_image_data read_jpeg_file(char * imageName){
    raw_image_data data;
    data.pixel_length=3;
    FILE *pJpeg = fopen(imageName, "rb");
    if (pJpeg == NULL){
        printf("no image found");
        abort();
    }
    
    struct jpeg_decompress_struct cinfo;
    j_decompress_ptr pinfo = &cinfo;
    struct jpeg_error_mgr jerr;
    

    int stride;
    
    cinfo.err = jpeg_std_error(&jerr);	
    jpeg_create_decompress(pinfo);
    jpeg_stdio_src(pinfo,pJpeg);
    jpeg_read_header(pinfo, TRUE);
    jpeg_start_decompress(pinfo);
    
    
    
    int width = cinfo.image_width;
    int height= cinfo.image_height;

    stride = width * 3;


    
    unsigned char ** pimage_data = NULL;
    
    unsigned char * buffer = NULL;


    pimage_data = (unsigned char**)malloc(sizeof(unsigned char*)*height);
    if (pimage_data == NULL){
        printf("mem allocation error");
    }
    buffer = (unsigned char*)malloc(sizeof(unsigned char)*height*stride);
    if (buffer == NULL){
        printf("mem allocation error");
    }

    for (int y = 0; y < height ;y++){
        pimage_data[y]= buffer + (y * stride);
    }
    
    for (int y = 0; y <height; y++){
        jpeg_read_scanlines(pinfo , &pimage_data[y], 1);  
        
    }
    
    data.width=width;
    data.height=height;
    data.pixel_buff=buffer;


    free(pimage_data);
    jpeg_destroy_decompress(pinfo);
    fclose(pJpeg);


    return data;
}




//png reader that takes info form a png and converts it into 255RGBa format and returns it back
raw_image_data read_png_file(char *imageName) {
    raw_image_data data;
    data.pixel_length=4;
    FILE *pPng = fopen(imageName, "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png){
        printf("THAT IS NOT A PNG!");
        return data;
    }
    
    png_infop info = png_create_info_struct(png); 
    if (!info){
        return data;
    }

    if (setjmp(png_jmpbuf(png))) {
        return data;
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


    
    unsigned char ** pimage_data = NULL;
    unsigned char * buffer = NULL;
    int stride = png_get_rowbytes(png,info);

    // setup image structure
    pimage_data= (unsigned char**)malloc(sizeof(unsigned char*)*height);
    if (pimage_data == NULL){
        printf("mem allocation error");
        return data;
    }

    buffer = (unsigned char*)malloc(stride*height);
    if (buffer == NULL){
        printf("mem allocation error");
        return data;
    }



    for (int y = 0; y < height; y++) {
        pimage_data[y] = buffer + (y * stride);
    }

    // put details into that image structure
    png_read_image(png, pimage_data);
    
    
    
    
    data.height = height;
    data.width = width;
    data.pixel_buff = buffer;
    
    free(pimage_data);
    fclose(pPng);
    png_destroy_read_struct(&png, &info, NULL);
    return data;
};