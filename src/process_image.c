#include <stdio.h>
#include <string.h>
#include <assert.h>
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

void set_pixel(image im, int x, int y, int c, float v)
{
    //So in this function we just need to set the pixel to the given value 
    
    //checking bounds 
    if(((x<0||x>=im.w) || (y<0||y>=im.h))|| (c<0||c>=im.c))
    {
        return;
    }
    
    //setting value
    im.data[(c * im.w * im.h)+(y * im.w)+ x]=v;
    
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    //okay, so we just need to fill the data attribute 
    //we can loop by array 
    
    for(int i=0;i<((im.c)*(im.w)*(im.h));i++)
    {
       copy.data[i]=im.data[i];       
    }
        
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    
     for(int i=0;i<((im.w)*(im.h));i++)
    {
       float luma_value= (0.299*im.data[i]) +  (0.587*im.data[i+((im.w)*(im.h))]) +. (0.114*im.data[i+2*((im.w)*(im.h))]);
       gray.data[i]=luma_value;       
    }
    
    return gray;
}

void shift_image(image im, int c, float v)
{
    // Okay add v to every pixel in channel c , got it 
    // we can check the validity 
    if(c<0 || c>=im.c)
    {
        return;
    }
    
    //Assuming CHW arrangement
    for(int i=0;i<((im.w)*(im.h));i++)
    {
        im.data[i+(c*((im.w)*(im.h))]+=v;
    }
}

void clamp_image(image im)
{
    //all we have to do is restrict the pixel data values between 0 and 1 , roger that 
    
    for(int i=0;i<((im.c)*(im.w)*(im.h));i++)
    {
       if(im.data[i] <= 0.0f)
       {
           im.data[i]=0.0f;
       }
       else if(im.data[i] >= 1.0f)
       {
           im.data[i]=1.0f;
       } 
       else
       {
           ;
       }    
        
    }
    
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    //Cool , this is interesting 
    
    for(int i=0;i<((im.w)*(im.h));i++)
    {
        // First we need to extract the rgb components 
        float red= im.data[i];
        float green= im.data[i+((im.w)*(im.h))];
        float blue= im.data[i+2*((im.w)*(im.h));
                            
        // Finding the value should be easy 
        float Value=three_way_max(red,green,blue);
        
        //Saturation
        float min=three_way_min(red,green,blue);
        float C=Value-min;
        float Saturation;
        if((red==0.0f && green==0.0f )&& blue==0.0f)
        {
            Saturation=0.0f;
        }                   
        else
        {
            Saturation= C/Value;
        }
                                                
        //HUE
        float Hue,Hue_dash;
        if (C == 0)
        {
            Hue = 0;
        }
        else if (Value == red)
        {
          Hue_dash = (green - blue) / C;
        }
        else if (Value == green)
        {
           Hue_dash = ((blue - red) / C) + 2;
        }       
        else if (Value == blue)
        {
          Hue_dash = ((red - green) / C) + 4;
        }
        else
        {
           ;
        } 
        
        //We got Hue_Dash
        if(C!=0)  
        {
        if(Hue_dash<0)
        {
            Hue= (Hue_dash/6) + 1 ;
        }
        else
        {
            Hue= (Hue_dash/6) ;
        }
        }                                                            
        
        //setting the values
        im.data[i]=Hue;
        im.data[i+((im.w)*(im.h))]=Saturation;
        im.data[i+2*((im.w)*(im.h))=Value;                    
                            
                            
    }
}

void hsv_to_rgb(image im)
{
    // First we need to get the HSV values 
    float h= im.data[i];
    float s= im.data[i+((im.w)*(im.h))];
    float v= im.data[i+2*((im.w)*(im.h));
    
    // As H is between 0 and 1 , We need to convert it to 0 and 360
    h=h*360;

    //Conversion parameters
    float c= v *s ;
    float x= c * (1- abs(((h/60)%2) - 1 ));
    float m= v- c;
    float r, g, b;

    if(h>=0 && h<60)
    {
        r=c;
        g=x; 
        b=0;
    }
    else if( h>=60 && h< 120)
    {
        r=x;
        g=c; 
        b=0;

    }
    else if( h>=120 && h< 180)
    {
        r=0;
        g=c; 
        b=x;
        
    }
    else if( h>=180 && h< 240)
    {
        r=0;
        g=x; 
        b=c;
        
    }
    else if( h>=240 && h< 300)
    {
        r=x;
        g=0; 
        b=c;
        
    }
    else 
    {
        r=c;
        g=0; 
        b=x;
        
    }
    r=r+m;
    g=g+m;
    b=b+m;


    //setting the values
    im.data[i]=r;
    im.data[i+((im.w)*(im.h))]=g;
    im.data[i+2*((im.w)*(im.h))=b;


}
