#include <math.h>
#include "image.h"


float nn_interpolate(image im, float x, float y, int c)
{
    int closest_x = (int)roundf(x);
    int closest_y = (int)roundf(y);
    
    return get_pixel(im, closest_x, closest_y, c);
}

image nn_resize(image im, int w, int h)
{
    image new_image = make_image(w, h, im.c);

    float width_scale = (float)im.w / w;
    float height_scale = (float)im.h / h;

    for (int k = 0; k < im.c; k++) 
    {
        for (int j = 0; j < h; j++) 
        {
            for (int i = 0; i < w; i++) 
            {
                float x = (i + 0.5) * width_scale - 0.5;
                float y = (j + 0.5) * height_scale - 0.5;
                float value = nn_interpolate(im, x, y, k);
                set_pixel(new_image, i, j, k, value);
            }
        }
    }
    return new_image;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    int x1 = (int)floorf(x);
    int y1 = (int)floorf(y);
    int x2 = x1 + 1;
    int y2 = y1 + 1;

    float q11 = get_pixel(im, x1, y1, c);
    float q12 = get_pixel(im, x1, y2, c);
    float q21 = get_pixel(im, x2, y1, c);
    float q22 = get_pixel(im, x2, y2, c);

    float dx = x - x1;
    float dy = y - y1;

    float value = ((1 - dx) * (1 - dy) * q11 ) + ( dx * (1 - dy) * q21) + ((1 - dx) * dy * q12) + (dx * dy * q22);

    return value;
}

image bilinear_resize(image im, int w, int h)
{
    image new_image = make_image(w, h, im.c);

    float width_scale = (float)im.w / w;
    float height_scale = (float)im.h / h;

    for (int k = 0; k < im.c; k++) 
    {
        for (int j = 0; j < h; j++)
        {
            for (int i = 0; i < w; i++) 
            {
                float x = (i + 0.5) * width_scale - 0.5;
                float y = (j + 0.5) * height_scale - 0.5;
                float value = bilinear_interpolate(im, x, y, k);
                set_pixel(new_image, i, j, k, value);
            }
        }
    }

    return new_image;
}

