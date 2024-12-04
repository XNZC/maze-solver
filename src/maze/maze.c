#include "maze.h"

typedef enum {R, L, U, D} Directions;

Maze* bmp_to_maze(BMPImage* image);
BMPImage* maze_to_bmp(Maze* maze);
void free_maze(Maze* maze);

void wall_follow(Maze* maze);
int get_starting_point(Maze* maze);
int get_finish_point(Maze* maze);
bool is_path(Directions direction, int* current_point, Maze* maze);


Maze* bmp_to_maze(BMPImage* image){
    Maze* maze = malloc(sizeof(*maze));

    int32_t width = image->header.height_px;
    int32_t height = image->header.width_px;

    maze->image = image;
    maze->height = height;
    maze->width = width;
    maze->map = malloc((height * width) * sizeof(uint8_t));


    int r = 0, i = 0;
    for (i = 0; i < image->header.image_size_bytes; i++){
        if ((i+2) % ((width * 3) + 1) == 0){ //skip padding
            i+=1;
            continue;
        }
        i+=2;
        
        if (image->data[i] == 255){
            maze->map[r] = 1;
        }else{
            maze->map[r] = 0;
        }

        r++;
    }
    
    return maze;
}

BMPImage* maze_to_bmp(Maze* maze){
    BMPImage* pBMP = new_BMPImage(maze->image);

    int pBMP_i = 0;
    for (int i = 0; i < (maze->height * maze->width); i++){
        if (i % maze->width == 0 && i != 0){ //padding
            pBMP->data[pBMP_i] = 0x00;
            pBMP->data[pBMP_i+1] = 0x00; 
            pBMP_i += 1;
        }

        if (maze->map[i] == 0){
            pBMP->data[pBMP_i] = 0x00;
            pBMP->data[pBMP_i+1]= 0x00;
            pBMP->data[pBMP_i+2] = 0x00;
        }

        if(maze->map[i] == 1){
            pBMP->data[pBMP_i] = 0xff;
            pBMP->data[pBMP_i+1] = 0xff;
            pBMP->data[pBMP_i+2] = 0xff;
        }

        if(maze->map[i] == 2){
            pBMP->data[pBMP_i] = 0x00;
            pBMP->data[pBMP_i+1] = 0x00;
            pBMP->data[pBMP_i+2] = 0xff;
        }

        if(maze->map[i] > 2){
            pBMP->data[pBMP_i] = 0xff;
            pBMP->data[pBMP_i+1] = 0x00;
            pBMP->data[pBMP_i+2] = 0x00;
        }

        pBMP_i += 3;
    }

    pBMP->data[pBMP_i] = 0x00;
    pBMP->data[pBMP_i+1] = 0x00; 

    return pBMP;
}

void free_maze(Maze* maze){
    if (maze){
        free(maze->map);
        free(maze);
    }
}

void wall_follow(Maze* maze){
    int starting_point = get_starting_point(maze);
    int finish_point = get_finish_point(maze);

    int current_point = starting_point;

    Directions direction = R;
    
    const struct Directions{
        Directions try_direction, next;
    } point_priority[][4] = {
        [R] = {{U, D}, {R, R}, {D, U}, {L, L}},
        [L] = {{D, U}, {L, L}, {U, D}, {R, R}},
        [U] = {{R, R}, {D, U}, {L, L}, {U, D}},
        [D] = {{L, L}, {U, D}, {R, R}, {D, U}},
    };

    while (current_point != finish_point){
        for(int i = 0; i < 4; i++){
            if (is_path(point_priority[direction][i].try_direction, &current_point, maze) == true){
                direction = point_priority[direction][i].next;
                break;
            }
        }
    }
}

int get_starting_point(Maze* maze){
    int starting_point;

    for(int i = 0; i < maze->width; i++){
        if (maze->map[i] == 1){
            starting_point = i;
            break;
        }
    }

    return starting_point;
}

int get_finish_point(Maze* maze){
    int finish_point;
    
    for (int i = 0; i < maze->width; i++){
        if (maze->map[(maze->height - 1) * maze->width + i] == 1){
            finish_point = (maze->height - 1) * maze->width + i;
            break;
        }
    }

    return finish_point;
}

bool is_path(Directions direction, int* current_point, Maze* maze){
    if (direction == R){
        if (maze->map[*current_point + 1] >= 1){
            maze->map[*current_point + 1] += 1;
            *current_point = *current_point + 1;
            return true;
        }
    }

    if (direction == L){
        if (maze->map[*current_point - 1] >= 1){
            maze->map[*current_point - 1] += 1;
            *current_point = *current_point - 1;
            return true;
        }
    }

    if (direction == U){
        if (maze->map[*current_point - maze->width] >= 1){
            maze->map[*current_point - maze->width] += 1;
            *current_point = *current_point - maze->width;
            return true;
        }
    }

    if (direction == D){
        if (maze->map[*current_point + maze->width] >= 1){
            maze->map[*current_point + maze->width] += 1;
            *current_point = *current_point + maze->width;
            return true;
        }
    }

    return false;
}