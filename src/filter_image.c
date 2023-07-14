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
// Answer: As explained in the markdown file , We should set preserve to 1 in box filters as it is used to remove noises and 
//deals with each channel indivisually.
//         In contradiction , highpass sharpen and emboss filters deals with edges and not indivisual values of channels we can choose
//not to preserve these as the values of filter will remain same no matter what.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: I don't think so box filter requires any post processing as the values remain above 0. But in other filters 
//    such as highpass, sharpen and emboss values can go -ve so we can do scaling accordingly.

image make_gaussian_filter(float sigma)
{
    int size = (int)ceilf(6 * sigma);
    
    if (size % 2 == 0)
    {
        size++; 
    }

    image filter = make_image(size, size, 1);

    float sum = 0;
    int half = size / 2;
    
    for (int h = -half; h <= half; h++) 
    {
        for (int w = -half; w <= half; w++) 
        {
            float x = w;
            float y = h;
            
            float value = expf(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * 3.141 * sigma * sigma);
            
            sum += value;
            
            set_pixel(filter, w + half, h + half, 0, value);
        }
    }

    l1_normalize(filter);

    return filter;
}

image add_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image new_image = make_image(a.w, a.h, a.c);

    for (int c = 0; c < a.c; c++) 
    {
        for (int h = 0; h < a.h; h++) 
        {
            for (int w = 0; w < a.w; w++) 
            {
                float value_1 = get_pixel(a, w, h, c);
                float value_2 = get_pixel(b, w, h, c);
                set_pixel(new_image, w, h, c, value_1 + value_2);
            }
        }
    }

    return new_image;
}

image sub_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image new_image = make_image(a.w, a.h, a.c);

    for (int c = 0; c < a.c; c++) 
    {
        for (int h = 0; h < a.h; h++) 
        {
            for (int w = 0; w < a.w; w++) 
            {
                float value_1 = get_pixel(a, w, h, c);
                float value_2 = get_pixel(b, w, h, c);
                set_pixel(new_image, w, h, c, value_1 - value_2);
            }
        }
    }

    return new_image;
}

image make_gx_filter()
{
    image filter = make_image(3, 3, 1);
    
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, 0);
    set_pixel(filter, 2, 0, 0, 1);
    
    set_pixel(filter, 0, 1, 0, -2);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 2);
    
    set_pixel(filter, 0, 2, 0, -1);
    set_pixel(filter, 1, 2, 0, 0);
    set_pixel(filter, 2, 2, 0, 1);
    
    return filter;
}

image make_gy_filter()
{
    image filter = make_image(3, 3, 1);
    
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, -2);
    set_pixel(filter, 2, 0, 0, -1);
    
    set_pixel(filter, 0, 1, 0, 0);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 0);
    
    set_pixel(filter, 0, 2, 0, 1);
    set_pixel(filter, 1, 2, 0, 2);
    set_pixel(filter, 2, 2, 0, 1);
    
    return filter;
}

void feature_normalize(image im)
{
    float min_val = INFINITY;
    float max_val = -INFINITY;

    // Find the minimum and maximum values in the image
    for (int c = 0; c < im.c; c++) 
    {
        for (int h = 0; h < im.h; h++) 
        {
            for (int w = 0; w < im.w; w++) 
            {
                float val = get_pixel(im, w, h, c);
                
                if (val < min_val)
                {
                    min_val = val;
                }
                
                if (val > max_val)
                {
                    max_val = val;
                }
            }
        }
    }

    float range = max_val - min_val;

    // If range is zero, set the whole image to 0
    if (range == 0) 
    {
        for (int c = 0; c < im.c; c++) 
        {
        for (int h = 0; h < im.h; h++) 
        {
            for (int w = 0; w < im.w; w++) 
            {
                set_pixel(im, w, h, c, 0);
            }
        }
        }
    }
    else
    {
    // Normalize the values in the image to the range 0-1
    for (int c = 0; c < im.c; c++) 
    {
        for (int h = 0; h < im.h; h++) 
        {
            for (int w = 0; w < im.w; w++) 
            {
                float val = get_pixel(im, w, h, c);
                
                val = (val - min_val) / range;
                
                set_pixel(im, w, h, c, val);
            }
        }
    }
    }
}

image *sobel_image(image im)
{
    image *sobel = calloc(2, sizeof(image));

    image gx_filter = make_gx_filter();
    image gy_filter = make_gy_filter();

    sobel[0] = convolve_image(im, gx_filter, 0);
    sobel[1] = convolve_image(im, gy_filter, 0);

    free_image(gx_filter);
    free_image(gy_filter);

    return sobel;
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
