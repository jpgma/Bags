void main()
{
	// FILE *f;

 //    f = fopen("nsvgrast.bmp", "w+");
 //    if(f)
 //    {
 //        r32 scale = 1.0f;
 //        u32 width = image->width*scale;
 //        u32 height = image->height*scale;
        
 //        u32 pixel_data_size = width*height*BITMAP_FORMAT_RGBA32;
 //        u32 file_size = sizeof(BMPFileHeader) + sizeof(BMPImageHeader) + pixel_data_size;
 //        u8 *file_data = (u8*)calloc(1,file_size);
 //        u8 *pixels = file_data + (sizeof(BMPFileHeader) + sizeof(BMPImageHeader));
        
 //        NSVGrasterizer *rast = nsvgCreateRasterizer();
 //        nsvgRasterize(rast, image, 0,0,scale, pixels, width, height, width*4);
 //        nsvgDeleteRasterizer(rast);
 //        nsvgDelete(image);
        
 //        for (u32 y = 0; y < height/2; ++y)
 //        {
 //            for (u32 x = 0; x < width; ++x)
 //            {
 //                u32 i = ((x+(y*width))*4);
 //                u32 j = ((x+((height-1-y)*width))*4);
                
 //                u8 ri = pixels[i+0];
 //                u8 gi = pixels[i+1];
 //                u8 bi = pixels[i+2];
 //                u8 ai = pixels[i+3];
                
 //                u8 rj = pixels[j+0];
 //                u8 gj = pixels[j+1];
 //                u8 bj = pixels[j+2];
 //                u8 aj = pixels[j+3];
                
 //                if(aj)
 //                {
 //                    pixels[i+0] = bj;
 //                    pixels[i+1] = bj;
 //                    pixels[i+2] = bj;
 //                    pixels[i+3] = bj;
 //                }
 //                else
 //                {
 //                    pixels[i+0] = 128;
 //                    pixels[i+1] = 128;
 //                    pixels[i+2] = 128;
 //                    pixels[i+3] = 128;
 //                }

 //                if(ai)
 //                {
 //                    pixels[j+0] = bi;
 //                    pixels[j+1] = bi;
 //                    pixels[j+2] = bi;
 //                    pixels[j+3] = bi;
 //                }
 //                else
 //                {
 //                    pixels[j+0] = 128;
 //                    pixels[j+1] = 128;
 //                    pixels[j+2] = 128;
 //                    pixels[j+3] = 128;
 //                }

 //            }
 //        }

 //        MakeBMP (file_data, width, height, BITMAP_FORMAT_RGBA32, pixels);
 //        fwrite ((void*)file_data, file_size, 1, f); 
 //        fflush(f);
 //        fclose(f);
        
 //        free(file_data);
 //    }

 //    f = fopen("bagsrast.bmp", "w+");
 //    if(f)
 //    {
 //        r32 scale = 1.0f;
 //        u32 width = vector_image.width*scale;
 //        u32 height = vector_image.height*scale;
        
 //        u32 pixel_data_size = width*height*BITMAP_FORMAT_RGBA32;
 //        u32 file_size = sizeof(BMPFileHeader) + sizeof(BMPImageHeader) + pixel_data_size;
 //        u8 *file_data = (u8*)calloc(1,file_size);
 //        u8 *pixels = file_data + (sizeof(BMPFileHeader) + sizeof(BMPImageHeader));

 //        RasterizeVectorImage(vector_image, width,height,scale, pixels);

 //        MakeBMP (file_data, width, height, BITMAP_FORMAT_RGBA32, pixels);
 //        fwrite ((void*)file_data, file_size, 1, f);
 //        fflush(f);
 //        fclose(f);
        
 //        free(file_data);
 //    }
}