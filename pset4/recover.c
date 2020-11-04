#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// create type BYTE
typedef uint8_t BYTE;

// constants
const BYTE BYTE_0 = 0xff;
const BYTE BYTE_1 = 0xd8;
const BYTE BYTE_2 = 0xff;
const BYTE BYTE_3_MIN = 0xe0;
const BYTE BYTE_3_MAX = 0xef;
const int SIGNATURE_SIZE = 4;
const long BLOCK_SIZE = 512;

// global variables
BYTE signature[SIGNATURE_SIZE];
int jpegName = -1;
long jpegStart = -1;
long jpegEnd;

int main(int argc, char *argv[]) {
    // check number of arguments
    if (argc != 2) {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // try to open file
    FILE *image = fopen(argv[1], "r");
    if (image == NULL) {
        printf("Can't open file.\n");
        return 1;
    }

    // iterate through file
    long index = 0;
    while (true) {
        // stops if reposition fails
        int reposition = fseek(image, index, SEEK_SET);
        if (reposition != 0) {
            break;
        }

        size_t signature_read = fread(signature, sizeof(BYTE), SIGNATURE_SIZE, image);
        if (signature_read != 4) {
            break;
        }

        // check if jpeg signature was found
        if (signature[0] == BYTE_0 && signature[1] == BYTE_1 && signature[2] == BYTE_2 &&
            (signature[3] >= BYTE_3_MIN && signature[3] <= BYTE_3_MAX)) {
            if (jpegStart != -1) {
                // prepare data to export
                jpegEnd = index;
                jpegName++;
                int picture_bytes = jpegEnd - jpegStart;
                fseek(image, jpegStart, SEEK_SET);
                BYTE buffer[picture_bytes];
                fread(buffer, sizeof(BYTE), picture_bytes, image);

                // set name of file
                char filename[7];
                sprintf(filename, "%03i.jpg", jpegName);
                
                // try to create file and copy data
                FILE *jpegFile = fopen(filename, "w");
                if (jpegFile == NULL) {
                    printf("Fail to create jpeg file.\n");
                    return 1;
                }
                fwrite(buffer, sizeof(BYTE), picture_bytes, jpegFile);
                fclose(jpegFile);
            }
            jpegStart = index;
        }
        if (jpegStart == -1) {
            index++;
        }
        else {
            index += BLOCK_SIZE;
        }
    }
    // prepare data to export
    jpegEnd = index;
    jpegName++;
    int picture_bytes = jpegEnd - jpegStart;
    fseek(image, jpegStart, SEEK_SET);
    BYTE buffer[picture_bytes];
    fread(buffer, sizeof(BYTE), picture_bytes, image);

    // set name of file
    char filename[7];
    sprintf(filename, "%03i.jpg", jpegName);
                
    // try to create file and copy data
    FILE *jpegFile = fopen(filename, "w");
    if (jpegFile == NULL) {
        printf("Fail to create jpeg file.\n");
        return 1;
    }
    fwrite(buffer, sizeof(BYTE), picture_bytes, jpegFile);
    fclose(jpegFile);

    // close file and end program
    fclose(image);
}