#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

float get_pixel(image im, int x, int y, int c)
{
    //Using the CHW format
    //Assuming im.h gives image height , im.w gives image width and im.c gives total channels 
    //In this fn we only need to return the value of the pixel at column x , row y and channel c 
    //we know that im.data stores the value
    
    //checking bounds 
    if(((x<0||x>=im.w) || (y<0||y>=im.h))|| (c<0||c>=im.c))
    {
        return;
    }    
    
    float *value= im.data + (c * im.w * im.h)+(y * im.w)+ x;
    return *value;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    //So in this function we just need to set the pixel to the given value 
    
    //checking bounds 
    if(((x<0||x>=im.w) || (y<0||y>=im.h))|| (c<0||c>=im.c))
    {
        ;
    }
    else
    {
    //setting value
    im.data[(c * im.w * im.h)+(y * im.w)+ x]=v;
    }
}


void l1_normalize(image im)
{
    float sum = 0;
    for (int c = 0; c < im.c; c++) 
    {
        for (int h = 0; h < im.h; h++) 
        {
            for (int w = 0; w < im.w; w++) 
            {
                sum += get_pixel(im, w, h, c);
            }
        }
    }

    for (int c = 0; c < im.c; c++) 
    {
        for (int h = 0; h < im.h; h++) 
        {
            for (int w = 0; w < im.w; w++) 
            {
                float value = get_pixel(im, w, h, c) / sum;
                set_pixel(im, w, h, c, value);
            }
        }
    }
}

image make_box_filter(int w)
{
    image new_image = make_image(w, w, 1);

    for (int h = 0; h < w; ++h) 
    {
        for (int w = 0; w < w; ++w) 
        {
            set_pixel(new_image, w, h, 0, 1);
        }
    }

    l1_normalize(new_image);

    return new_image;
}

image convolve_image(image im, image filter, int preserve)
{
    assert(filter.c == im.c || filter.c == 1);

    int channels = preserve ? im.c : 1;
    image new_image = make_image(im.w, im.h, channels);

    for (int c = 0; c < channels; c++) 
    {
        for (int h = 0; h < im.h; h++) 
        {
            for (int w = 0; w < im.w; w++) 
            {

                
                float sum = 0;
                for (int fc = 0; fc < filter.c; fc++) 
                {
                    for (int fh = 0; fh < filter.h; fh++) 
                    {
                        for (int fw = 0; fw < filter.w; fw++) 
                        {
                            int im_c = preserve ? c : 0;
                            int im_h = h + fh - filter.h / 2;
                            int im_w = w + fw - filter.w / 2;
                            
                            float im_val = get_pixel(im, im_w, im_h, im_c);
                            float filter_val = get_pixel(filter, fw, fh, fc);
                            
                            sum = sum + (im_val * filter_val);
                        }
                    }
                }
                
                set_pixel(new_image, w, h, c, sum);

                
            }
        }
    }

    return new_image;
}

image make_highpass_filter()
{
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);

    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 4);
    set_pixel(filter, 2, 1, 0, -1);

    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);

    return filter;
}

image make_sharpen_filter()
{
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);

    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 5);
    set_pixel(filter, 2, 1, 0, -1);

    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);

    return filter;
}

image make_emboss_filter()
{
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, -2);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);

    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 1);
    set_pixel(filter, 2, 1, 0, 1);

    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, 1);
    set_pixel(filter, 2, 2, 0, 2);

    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    return make_image(1,1,1);
}

image add_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image sub_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image make_gx_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    return make_image(1,1,1);
}

void feature_normalize(image im)
{
    // TODO
}

image *sobel_image(image im)
{
    // TODO
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
