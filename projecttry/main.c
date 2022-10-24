#include <math.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

/**
 * Create a new 1-dimensional array with the given size
 * @param[in] _size the size of the array
 * @param[out] _ empty 1-dimensional array filled with 0
 */

unsigned char *uc_arrayNew_1d(int _size)
{
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}

unsigned char *mask_image(unsigned char *image1, unsigned char *image2, unsigned char *image3, int width, int height, int channel)
{
    unsigned char *temp_array = uc_arrayNew_1d(width * height * channel);
    int a;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < channel; k+=3)
            {
                a = i * width * channel + j * channel + k;
                if(image2[a+1] > 180)
                // middle value is the G in RGB, choose a value close to 232 (original green screen value)
                {
                    temp_array[a] = image3[a];
                    temp_array[a+1] = image3[a+1];
                    temp_array[a+2] = image3[a+2];
                }
                else{
                    temp_array[a] = image2[a];
                    temp_array[a+1] = image2[a+1];
                    temp_array[a+2] = image2[a+2];
                }
            }
        }
    }
    return temp_array;
}

int main()
{
    // declare variables
    int width1, height1, channel1, width2, height2, channel2;
    int width3, height3, channel3;
    char path_img1[] = "./images/background.png";
    char path_img2[] = "./images/foreground.png";
    char path_img3[] = "./images/weather.png";
    char save_path[] = "./images/10-New.png";

    // read image data
    unsigned char *image1 = stbi_load(path_img1, &width1, &height1, &channel1, 0);
    if (image1 == NULL)
    {
        printf("\nError in loading the 1st image\n");
        exit(1);
    }

    printf("1.Width = %d\nHeight = %d\nChannel = %d\n\n\n", width1, height1, channel1);

    unsigned char *image2 = stbi_load(path_img2, &width2, &height2, &channel2, 0);
    if (image2 == NULL)
    {
        printf("\nError in loading the 2nd image\n");
        exit(1);
    }

    printf("2.Width = %d\nHeight = %d\nChannel = %d\n\n\n", width2, height2, channel2);

    if (width1 != width2     ||   height1 != height2    ||      channel1 != channel2)
    {
        printf("\nImages incompatible\n");
        exit(1);
    }

    unsigned char *image3 = stbi_load(path_img3, &width3, &height3, &channel3, 0);
    if (image3 == NULL)
    {
        printf("\nError in loading the 3rd image\n");
        exit(1);
    }

    printf("3.Width = %d\nHeight = %d\nChannel = %d\n\n\n", width3, height3, channel3);

    unsigned char *mimage = mask_image(image1, image2, image3, width1, height1, channel1);
    stbi_write_png(save_path, width1, height1, channel1, mimage, width1 * channel1);
    printf("New image saved to %s\n", save_path);
}
