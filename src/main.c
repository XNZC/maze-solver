#include <stdio.h>
#include "bmp/bmp.h"
#include "maze/maze.h"


int main(void){
    BMPImage* image = read_image("../bitmaps/4.bmp");
    Maze* maze = bmp_to_maze(image);

    wall_follow(maze);
    BMPImage* solved_image = maze_to_bmp(maze);

    bool success = write_image("../bitmaps/solved.bmp", solved_image);

    free_maze(maze);
    free_image(image);
    free_image(solved_image);

    return success;
}