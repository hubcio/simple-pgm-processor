#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

#define msg(s, ...) printf(s "\n", ##__VA_ARGS__)
#define MAX_NAME_LEN 50

enum menu {
    DEFAULT,
    LOAD,
    SAVE,
    DELETE,
    SHOW_CURRENT_IMAGES,
    PROCESS_IMAGE,
    QUIT,
};

struct program {
    enum menu menu;
    struct pgm_image *images[10];
    uint16_t number_of_images;
};

// uint16_t = od 0 do 65536
// int16_t = -32767 do 32767

// int8_t = od -127 do 127
// uint8_t = od 0 do 255

struct pgm_image {
    char name[MAX_NAME_LEN+1];
    size_t width;
    size_t height;
    size_t max_val;
    uint16_t **gray;
};

void print_menu(void);

/*
 * Function prototypes
 */

int16_t             get_int(int16_t min, int16_t max, char *description);
void                get_string(char *buf, char *description);
int8_t              get_pgm_file (char filename[], struct pgm_image *img);
struct pgm_image *  init_image();
void                init_program(struct program *p);
void                delete_image(struct pgm_image * img);
void                save_pgm(const char *filename, const struct pgm_image *data);
