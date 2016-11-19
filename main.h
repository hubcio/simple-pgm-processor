#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define log(fmt, ...) printf("%s" ": " fmt "\n", __func__,##__VA_ARGS__)
#define msg(s, ...) printf(s "\n", ##__VA_ARGS__)

enum menu{
    DEFAULT,
    LOAD,
    SAVE,
    NEW_IMG,
    SHOW_CURRENT_IMAGES,
    PROCESS_IMAGE,
    QUIT,
};

struct program{
    enum menu menu;
    struct pgm_image *images;
    uint16_t number_of_images;
};

// uint16_t = od 0 do 65536
// int16_t = -32767 do 32767

// int8_t = od -127 do 127
// uint8_t = od 0 do 255

struct pgm_image{
    uint16_t max;
    uint16_t width;
    uint16_t height;
    uint16_t **gray;
};

void print_menu(void);
int16_t get_int(int16_t min, int16_t max, char *description);
