#include "bmp.h"

#define MAGIC_VALUE         0x4D42
#define OFFSET              54
#define DIB_HEADER_SIZE     40
#define NUM_PLANES          1
#define COMPRESSION         0
#define NUM_COLORS          0
#define IMPORTANT_COLORS    0
#define BITS_PER_PIXEL      24
#define BITS_PER_BYTE       8


BMPImage* read_image(const char* filename);
BMPImage* new_BMPImage();
bool write_image(const char* filename, BMPImage* image);
void free_image(BMPImage* image);
bool validate_image_header(BMPHeader* header, FILE* pF);

int get_file_size(FILE* pF);
int get_image_size(BMPHeader *header);
int get_image_row_size(BMPHeader *header);
int get_padding(BMPHeader *header);
int get_bytes_per_pixel(BMPHeader  *header);


BMPImage* read_image(const char* filename){
    FILE* pF = fopen(filename, "rb");

    if (pF == NULL){
        printf("Failed to open file");
        return NULL;
    }

    
    BMPImage* pBMP = malloc(sizeof(*pBMP));
    rewind(pF);

    size_t header_read = fread(&pBMP->header, sizeof(pBMP->header), 1, pF);

    if (header_read != 1){
        printf("Unable to read header");
        return NULL;
    }else if (validate_image_header(&pBMP->header, pF) == false){
        printf("Invalid BMP header");
        return NULL;
    }

    pBMP->data = malloc(sizeof(*pBMP->data) * pBMP->header.image_size_bytes);

    size_t data_read = fread(pBMP->data, pBMP->header.image_size_bytes, 1, pF);
    if (data_read != 1){
        printf("Unable to read image data");
        return NULL;
    }

    fclose(pF);
    return pBMP;
}

BMPImage* new_BMPImage(BMPImage* image){
    BMPImage* pBMP = malloc(sizeof(*pBMP));
    pBMP->header = image->header;
    pBMP->data = malloc(sizeof(*pBMP->data) * pBMP->header.image_size_bytes);
    return pBMP;
}

bool write_image(const char* filename, BMPImage* image){
    FILE* pF = fopen(filename, "wb");
    if (pF == NULL){
        printf("Could not open file");
    }

    rewind(pF);

    size_t header_write = fwrite(&image->header, sizeof(image->header), 1, pF);
    if (header_write != 1){
        printf("Unable to write header");
        return false;
    }

    size_t data_write = fwrite(image->data, image->header.image_size_bytes, 1, pF);
    if (data_write != 1){
        printf("Unable to write image data");
        return false;
    }

    fclose(pF);

    return true;
}

bool validate_image_header(BMPHeader* header, FILE* pF){
    return
        header->type == MAGIC_VALUE
        && header->offset == OFFSET
        && header->dib_header_size == DIB_HEADER_SIZE
        && header->num_planes == NUM_PLANES
        && header->compression == COMPRESSION
        && header->num_colors == NUM_COLORS 
        && header->important_colors == IMPORTANT_COLORS
        && header->bits_per_pixel == BITS_PER_PIXEL
        && header->size == get_file_size(pF) 
        && header->image_size_bytes == get_image_size(header);
}

void free_image(BMPImage* image){
    if (image){
        free(image->data);
        free(image);
    }
}

int get_file_size(FILE* pF){
    long current_position = ftell(pF);

    fseek(pF, 0L, SEEK_END);
    long file_size = ftell(pF);

    fseek(pF, current_position, SEEK_SET);

    return file_size;
}

int get_image_size(BMPHeader *header)
{
    return get_image_row_size(header) * header->height_px;
}

int get_image_row_size(BMPHeader *header)
{
    int bytes_per_row_without_padding = header->width_px * get_bytes_per_pixel(header);
    return bytes_per_row_without_padding + get_padding(header);
}

int get_padding(BMPHeader *header)
{
    return (4 - (header->width_px * get_bytes_per_pixel(header)) % 4) % 4;
}

int get_bytes_per_pixel(BMPHeader  *header)
{
    return header->bits_per_pixel / BITS_PER_BYTE;
}