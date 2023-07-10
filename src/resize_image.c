#include <math.h>
#include "image.h"

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

float nn_interpolate(image im, float x, float y, int c)
{
    int closest_x = (int)roundf(x);
    int closest_y = (int)roundf(y);
    
    return get_pixel(im, closest_x, closest_y, c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    return make_image(1,1,1);
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    return 0;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    return make_image(1,1,1);
}

