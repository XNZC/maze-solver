#ifndef MAZE_H
#define MAZE_H

#include "../bmp/bmp.h"

typedef struct{

    BMPImage* image;
    int32_t width;
    int32_t height;
    uint8_t* map;

} Maze;

BMPImage* maze_to_bmp(Maze* maze);
Maze* bmp_to_maze(BMPImage* image);
void free_maze(Maze* maze);
void wall_follow(Maze* maze);

#endif