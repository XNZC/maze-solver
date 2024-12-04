#ifndef BMP_HANDLER_H
#define BMP_HANDLER_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#pragma pack(push, 1)

typedef struct{

    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t dib_header_size;
    int32_t width_px;
    int32_t height_px;
    uint16_t num_planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size_bytes;
    int32_t x_resolution_ppm;
    int32_t y_resolution_ppm;
    uint32_t num_colors;
    uint32_t important_colors;

}  BMPHeader;

#pragma pack(pop)

typedef struct{
    BMPHeader header;
    uint8_t* data;
} BMPImage;



BMPImage* read_image(const char* filename);
BMPImage* new_BMPImage(BMPImage* image);
bool write_image(const char* filename, BMPImage* image);
void free_image(BMPImage* image);
bool validate_image_header(BMPHeader* header, FILE* pF);

#endif