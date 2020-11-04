#include "helpers.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

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
                    // skips if out of bonds
                    continue;
                }
                else {
                    for (int adjacentColumns = column - 1; adjacentColumns < column + 2; adjacentColumns++) {
                        if (adjacentColumns < 0 || adjacentColumns == width) {
                            // skips if out of bonds
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]) {
    // create a copy to apply the changes
    RGBTRIPLE copyImage[height][width];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            copyImage[i][j] = image[i][j];
        }
    }

    // iterate through every pixel calculating gx and gy
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            double gx_red[3][3];
            double gx_green[3][3];
            double gx_blue[3][3];
            double gy_red[3][3];
            double gy_green[3][3];
            double gy_blue[3][3];
            int pixel = 1;
            while (true) {
                switch (pixel) {
                    case 1:
                        // calculate upper left pixel
                        if (row == 0 || column == 0) {
                            // black if beyond the edge
                            gx_red[0][0] = 0;
                            gx_green[0][0] = 0;
                            gx_blue[0][0] = 0;
                            gy_red[0][0] = 0;
                            gy_green[0][0] = 0;
                            gy_blue[0][0] = 0;
                        }
                        else {
                            // calculate gx
                            gx_red[0][0] = image[row - 1][column - 1].rgbtRed * (-1);
                            gx_green[0][0] = image[row - 1][column - 1].rgbtGreen * (-1);
                            gx_blue[0][0] = image[row - 1][column - 1].rgbtBlue * (-1);

                            // calculate gy
                            gy_red[0][0] = image[row - 1][column - 1].rgbtRed * (-1);
                            gy_green[0][0] = image[row - 1][column - 1].rgbtGreen * (-1);
                            gy_blue[0][0] = image[row - 1][column - 1].rgbtBlue * (-1);
                        }
                        break;
                    case 2:
                        // calculate upper center pixel
                        if (row == 0) {
                            // black if beyond the edge
                            gx_red[0][1] = 0;
                            gx_green[0][1] = 0;
                            gx_blue[0][1] = 0;
                            gy_red[0][1] = 0;
                            gy_green[0][1] = 0;
                            gy_blue[0][1] = 0;
                        }
                        else {
                            // calculate gx
                            gx_red[0][1] = 0;
                            gx_green[0][1] = 0;
                            gx_blue[0][1] = 0;

                            // calculate gy
                            gy_red[0][1] = image[row - 1][column].rgbtRed * (-2);
                            gy_green[0][1] = image[row - 1][column].rgbtGreen * (-2);
                            gy_blue[0][1] = image[row - 1][column].rgbtBlue * (-2);
                        }
                        break;
                    case 3:
                        // calculate upper right pixel
                        if (row == 0 || column == width - 1) {
                            // black if beyond the edge
                            gx_red[0][2] = 0;
                            gx_green[0][2] = 0;
                            gx_blue[0][2] = 0;
                            gy_red[0][2] = 0;
                            gy_green[0][2] = 0;
                            gy_blue[0][2] = 0;
                        }
                        else {
                            // calculate gx
                            gx_red[0][2] = image[row - 1][column + 1].rgbtRed;
                            gx_green[0][2] = image[row - 1][column + 1].rgbtGreen;
                            gx_blue[0][2] = image[row - 1][column + 1].rgbtBlue;

                            // calculate gy
                            gy_red[0][2] = image[row - 1][column + 1].rgbtRed * (-1);
                            gy_green[0][2] = image[row - 1][column + 1].rgbtGreen * (-1);
                            gy_blue[0][2] = image[row - 1][column + 1].rgbtBlue * (-1);
                        }
                        break;
                    case 4:
                        // calculate middle left pixel
                        if (column == 0) {
                            // black if beyond the edge
                            gx_red[1][0] = 0;
                            gx_green[1][0] = 0;
                            gx_blue[1][0] = 0;
                            gy_red[1][0] = 0;
                            gy_green[1][0] = 0;
                            gy_blue[1][0] = 0;
                        }
                        else {
                            // calculate gx
                            gx_red[1][0] = image[row][column - 1].rgbtRed * (-2);
                            gx_green[1][0] = image[row][column - 1].rgbtGreen * (-2);
                            gx_blue[1][0] = image[row][column - 1].rgbtBlue * (-2);

                            // calculate gy
                            gy_red[1][0] = 0;
                            gy_green[1][0] = 0;
                            gy_blue[1][0] = 0;
                        }
                        break;
                    case 5:
                        // calculate middle center pixel
                        // calculate gx
                        gx_red[1][1] = 0;
                        gx_green[1][1] = 0;
                        gx_blue[1][1] = 0;

                        // calculate gy
                        gy_red[1][1] = 0;
                        gy_green[1][1] = 0;
                        gy_blue[1][1] = 0;
                        break;
                    case 6:
                        // calculate middle right pixel
                        if (column == width - 1) {
                            // black if beyond the edge
                            gx_red[1][2] = 0;
                            gx_green[1][2] = 0;
                            gx_blue[1][2] = 0;
                            gy_red[1][2] = 0;
                            gy_green[1][2] = 0;
                            gy_blue[1][2] = 0;
                        }
                        else {
                            // calculate gx
                            gx_red[1][2] = image[row][column + 1].rgbtRed * 2;
                            gx_green[1][2] = image[row][column + 1].rgbtGreen * 2;
                            gx_blue[1][2] = image[row][column + 1].rgbtBlue * 2;

                            // calculate gy
                            gy_red[1][2] = 0;
                            gy_green[1][2] = 0;
                            gy_blue[1][2] = 0;
                        }
                        break;
                    case 7:
                        // calculate lower left pixel
                        if (row == height - 1 || column == 0) {
                            // black if beyond the edge
                            gx_red[2][0] = 0;
                            gx_green[2][0] = 0;
                            gx_blue[2][0] = 0;
                            gy_red[2][0] = 0;
                            gy_green[2][0] = 0;
                            gy_blue[2][0] = 0;
                        }
                        else {
                            // calculate gx
                            gx_red[2][0] = image[row + 1][column - 1].rgbtRed * (-1);
                            gx_green[2][0] = image[row + 1][column - 1].rgbtGreen * (-1);
                            gx_blue[2][0] = image[row + 1][column - 1].rgbtBlue * (-1);

                            // calculate gy
                            gy_red[2][0] = image[row + 1][column - 1].rgbtRed;
                            gy_green[2][0] = image[row + 1][column - 1].rgbtGreen;
                            gy_blue[2][0] = image[row + 1][column - 1].rgbtBlue;
                        }
                        break;
                    case 8:
                        // calculate lower center pixel
                        if (row == height - 1) {
                            // black if beyond the edge
                            gx_red[2][1] = 0;
                            gx_green[2][1] = 0;
                            gx_blue[2][1] = 0;
                            gy_red[2][1] = 0;
                            gy_green[2][1] = 0;
                            gy_blue[2][1] = 0;
                        }
                        else {
                            // calculate gx
                            gx_red[2][1] = 0;
                            gx_green[2][1] = 0;
                            gx_blue[2][1] = 0;

                            // calculate gy
                            gy_red[2][1] = image[row + 1][column].rgbtRed * 2;
                            gy_green[2][1] = image[row + 1][column].rgbtGreen * 2;
                            gy_blue[2][1] = image[row + 1][column].rgbtBlue * 2;
                        }
                        break;
                    case 9:
                        // calculate lower left pixel
                        if (row == height - 1 || column == width - 1) {
                            // black if beyond the edge
                            gx_red[2][2] = 0;
                            gx_green[2][2] = 0;
                            gx_blue[2][2] = 0;
                            gy_red[2][2] = 0;
                            gy_green[2][2] = 0;
                            gy_blue[2][2] = 0;
                        }
                        else {
                            // calculate gx
                            gx_red[2][2] = image[row + 1][column + 1].rgbtRed;
                            gx_green[2][2] = image[row + 1][column + 1].rgbtGreen;
                            gx_blue[2][2] = image[row + 1][column + 1].rgbtBlue;

                            // calculate gy
                            gy_red[2][2] = image[row + 1][column + 1].rgbtRed;
                            gy_green[2][2] = image[row + 1][column + 1].rgbtGreen;
                            gy_blue[2][2] = image[row + 1][column + 1].rgbtBlue;
                        }
                        break;
                }
                pixel++;
                if (pixel == 10) {
                    break;
                }
            }

            // sum the results
            double sum_gx_red = 0;
            double sum_gx_green = 0;
            double sum_gx_blue = 0;
            double sum_gy_red = 0;
            double sum_gy_green = 0;
            double sum_gy_blue = 0;

            for (int x = 0; x < 3; x++) {
                for (int y = 0; y < 3; y++) {
                    sum_gx_red += gx_red[x][y];
                    sum_gx_green += gx_green[x][y];
                    sum_gx_blue += gx_blue[x][y];
                    sum_gy_red += gy_red[x][y];
                    sum_gy_green += gy_green[x][y];
                    sum_gy_blue += gy_blue[x][y];
                }
            }

            // combining gx and gy
            double finalRed = round(sqrt(pow(sum_gx_red, 2) + pow(sum_gy_red, 2)));
            if (finalRed > 255) {
                finalRed = 255;
            }
            if (finalRed < 0) {
                finalRed = 0;
            }

            double finalGreen = round(sqrt(pow(sum_gx_green, 2) + pow(sum_gy_green, 2)));
            if (finalGreen > 255) {
                finalGreen = 255;
            }
            if (finalGreen < 0) {
                finalGreen = 0;
            }

            double finalBlue = round(sqrt(pow(sum_gx_blue, 2) + pow(sum_gy_blue, 2)));
            if (finalBlue > 255) {
                finalBlue = 255;
            }
            if (finalBlue < 0) {
                finalBlue = 0;
            }

            // assigning new value to the pixel
            copyImage[row][column].rgbtRed = (int)finalRed;
            copyImage[row][column].rgbtGreen = (int)finalGreen;
            copyImage[row][column].rgbtBlue = (int)finalBlue;
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