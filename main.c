#include <stdio.h>
#include "stdlib.h"
#include <string.h>
#include <math.h>


// settings
int n = 4; // board height
int m = 4; // board width
int random_start = 0;
//


// game rules
int stay_alive[] = {2, 3};
int dead_to_alive[] = {3};
//

// game variables
int** game_field;
int** game_field_copy;
int stay_alive_size = sizeof (stay_alive) / sizeof (int);
int dead_to_alive_size = sizeof (dead_to_alive) / sizeof (int);
FILE * input;
FILE *output;
int max_iter = 1;
int dump_freq = 1;
char header[54];
int height = 0;
int width = 0;
int size = 0;
//

void initialize() {
    game_field = (int **) malloc(sizeof (int) * n);
    game_field_copy = (int **) malloc(sizeof (int) * n);

    for (int i = 0; i < n; i++) {
        game_field[i] = (int*) malloc(sizeof (int) * m);
        game_field_copy[i] = (int*) malloc(sizeof (int) * m);
        for (int j = 0; j < m; j++) {
            if (random_start)
                game_field[i][j] = rand() % 2;
            else
                game_field[i][j] = 0;
        }
    }
}

void copy_game_field() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            game_field_copy[i][j] = game_field[i][j];
        }
    }
}

int number_of_alive_neighbors(int x, int y) {
    int result = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if ( (i!=0 || j!=0) &&
                 ((x + i) >= 0) && ((x + i) < n) &&
                 ((y + j) >= 0) && ((y + j) < m))
                result += game_field_copy[x + i][y + j]; // !!! check neighbors from previous position (from copy)
        }
    }
    return result;
}

void print_game_field() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d", game_field[i][j]);
        }
        printf("\n");
    }
    printf("//\n");
}

void update() {
    copy_game_field();
//    print_game_field();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int num = number_of_alive_neighbors(i, j);
            if (game_field_copy[i][j]) {
                int result = 0;
                for (int k = 0; k < stay_alive_size; k++) {
                    if (num == stay_alive[k])
                        result = 1;
                }
                game_field[i][j] = result;
            } else {
                int result = 0;
                for (int k = 0; k < dead_to_alive_size; k++) {
                    if (num == dead_to_alive[k])
                        result = 1;
                }
                game_field[i][j] = result;
            }

        }
    }
}

void read_arguments(int argc, char *argv[]) {
    for (int i = 1; i <= argc; i++) {
        if (!strcmp(argv[argc], "--input")) {
            input = fopen(argv[++argc], "r");
        } else if (!strcmp(argv[argc], "--output")) {
            output = fopen(argv[++argc], "r");
        } else if (!strcmp(argv[argc], "--max_iter")) {
            max_iter = atoi(argv[++argc]);
        } else if (!strcmp(argv[argc], "dump_freq")) {
            dump_freq = atoi(argv[++argc]);
        }
    }
}
void read_file(){
    fread(&header, 1, 54, input);
    for (int i = 0; i < 4; ++i) {
        width += header[18 + i] * pow(256, i);
        height += header[22 + i] * pow(256, i);
        size += header[2 + i] * pow(256, i);
    }
}

int main(int argc, char *argv[]) {
    initialize();
    read_arguments(argc, argv);

    game_field[1][1] = 1;
    game_field[1][2] = 1;
    game_field[1][3] = 1;
//    game_field[2][2] = 1;

    for (int t = 0; t < 10; t++) {
        print_game_field();
        update();
    }
    return 0;
}