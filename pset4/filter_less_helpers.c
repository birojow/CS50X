#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            // calculate and applies average color
            int average = round((image[row][column].rgbtRed + image[row][column].rgbtGreen + image[row][column].rgbtBlue) / 3.0);
            image[row][column].rgbtRed = average;
            image[row][column].rgbtGreen = average;
            image[row][column].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width]) {
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            // calculate individual colors
            int sepiaRed = round(0.393 * image[row][column].rgbtRed + 0.769 * image[row][column].rgbtGreen + 0.189 *
                                 image[row][column].rgbtBlue);
            int sepiaGreen = round(0.349 * image[row][column].rgbtRed + 0.686 * image[row][column].rgbtGreen + 0.168 *
                                   image[row][column].rgbtBlue);
            int sepiaBlue = round(0.272 * image[row][column].rgbtRed + 0.534 * image[row][column].rgbtGreen + 0.131 *
                                  image[row][column].rgbtBlue);
            // set color to 255 if value is greater
            if (sepiaRed > 255) {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255) {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255) {
                sepiaBlue = 255;
            }
            // applies new colors
            image[row][column].rgbtRed = sepiaRed;
            image[row][column].rgbtGreen = sepiaGreen;
            image[row][column].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]) {
    // find the center of the image
    int center;
    if (width % 2 == 0) {
        center = width / 2;
    }
    else {
        center = floor(width / 2);
    }

    for (int row = 0; row < height; row++) {
        for (int column = 0; column < center; column++) {
            // swap pixels
            RGBTRIPLE temp = image[row][column];
            image[row][column] = image[row][width - column - 1];
            image[row][width - column - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]) {
    // create a copy to apply the changes
    RGBTRIPLE copyImage[height][width];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            copyImage[i][j] = image[i][j];
        }
    }
    
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            // stores the sum of all values and the number of pixels summed
            float pixels = 0.0;
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            for (int adjacentRows = row - 1; adjacentRows < row + 2; adjacentRows++) {
                if (adjacentRows < 0 || adjacentRows == height) {
                    // skips is out of bonds
                    continue;
                }
                else {
                    for (int adjacentColumns = column - 1; adjacentColumns < column + 2; adjacentColumns++) {
                        if (adjacentColumns < 0 || adjacentColumns == width) {
                            // skips is out of bonds
                            continue;
                        }
                        else {
                            sumRed += image[adjacentRows][adjacentColumns].rgbtRed;
                            sumGreen += image[adjacentRows][adjacentColumns].rgbtGreen;
                            sumBlue += image[adjacentRows][adjacentColumns].rgbtBlue;
                            pixels++;
                        }
                    }
                }
            }
            // calculate new colors
            int averageRed = round(sumRed / pixels);
            int averageGreen = round(sumGreen / pixels);
            int averageBlue = round(sumBlue / pixels);
            // applies to copy image
            copyImage[row][column].rgbtRed = averageRed;
            copyImage[row][column].rgbtGreen = averageGreen;
            copyImage[row][column].rgbtBlue = averageBlue;
        }
    }

    // update original image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = copyImage[i][j];
        }
    }
    return;
}
