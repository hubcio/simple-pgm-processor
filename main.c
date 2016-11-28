#include <unistd.h>
#include "main.h"

int main(void) {
    struct program state;

    state.number_of_images = 0;

    while (1) {
        switch (state.menu) {
            case LOAD:
                /* If there are no images in memory */
                if (state.number_of_images == 0) {
                    state.images = init_image();
                    get_string(state.images[0].name, "Type filename to read:");
                    if (get_pgm_file(state.images[0].name, &state.images[0])){
                        state.number_of_images++;
                    }
                }
// else if (state.number_of_images >= 1){
//                    //get number of images, increment it by one, create new instance
//                    struct pgm_image *img = (struct pgm_image*)calloc(1,sizeof(struct pgm_image));
//                    state.number_of_images++;
//                    state.images[state.number_of_images] = *img;
//                }
                state.menu = DEFAULT;
                break;
            case SAVE:
                break;
            case SHOW_CURRENT_IMAGES:
                for (int i = 0; i < state.number_of_images; i++) {
                    msg("%d - [%s]", i, state.images[i].name);
                }
                state.menu = DEFAULT;
                break;
            case PROCESS_IMAGE:
                break;
            case QUIT:
                return 0;
            case DEFAULT:
            default:
                print_menu();
                state.menu = get_int(DEFAULT, QUIT, "Type number and click enter:");
                break;
        }
    }
}

void print_menu(void) {
    for (int i = 0; i <= QUIT; i++) {
        i == LOAD ?                 msg("%d - Load PGM image from file", LOAD) : 0;
        i == SAVE ?                 msg("%d - Save PGM image to file", SAVE) : 0;
        i == SHOW_CURRENT_IMAGES ?  msg("%d - Info about loaded images", SHOW_CURRENT_IMAGES) : 0;
        i == PROCESS_IMAGE ?        msg("%d - Process one of loaded images", PROCESS_IMAGE) : 0;
        i == QUIT ?                 msg("%d - Exit program", QUIT) : 0;
    }
}

int16_t get_int(int16_t min, int16_t max, char *description) {
    int n = 0;
    char buffer[128];
    if (description != NULL) msg("%s", description);
    fgets(buffer, sizeof(buffer), stdin);
    n = atoi(buffer);
    fflush(stdin);
    if (n < min || n > max) return -1;
    else return n;
}

void get_string(char *buf, char *description) {
    char *result = NULL;
    if (description != NULL) msg("%s", description);
    result = fgets(buf, MAX_NAME_LEN, stdin);
    int i=0;
    while (result[i] != '\0') {
        if (result[i] == '\n'){
            result[i] = '\0';
        }
        i++;
    }
    if (result != NULL) {
        if (feof (stdin)) {
            printf("\nEnd of file\n");
        }
    } else
        printf("\nReading error\n");
}

struct pgm_image *init_image() {
    struct pgm_image *img = (struct pgm_image *) calloc(1, sizeof(struct pgm_image));
    return img;
}

/*Gets an ascii pgm image file, store as a color pgm.*/
bool get_pgm_file(char filename[], struct pgm_image *img) {
    FILE *in_file;
    char ch;
    int row, col, type;
    int ch_int;
    chdir("..");
    in_file = fopen(filename, "r");

    int MAX_PATH_LENGTH = 80;
    char path[MAX_PATH_LENGTH];
    getcwd(path, MAX_PATH_LENGTH);

    printf("Current Directory = %s\n", path);

    if (in_file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n\r", filename);
        return -1;
    }
    printf("\nReading image file: %s\n", filename);
    /*determine pgm image type (only type three can be used)*/
    ch = getc(in_file);
    if (ch != 'P') {
        fprintf(stderr,"ERROR(1): Not valid pgm/ppm file type\n\r");
        return -1;
    }
    ch = getc(in_file);
    /*convert the one digit integer currently represented as a character to
      an integer(48 == '0')*/
    type = ch - 48;
    if ((type != 2) && (type != 3) && (type != 5) && (type != 6)) {
        fprintf(stderr,"ERROR(2): Not valid pgm/ppm file type\n");
        return -1;
    }
    while (getc(in_file) != '\n');             /* skip to end of line*/
    while (getc(in_file) == '#')              /* skip comment lines */
    {
        while (getc(in_file) != '\n');          /* skip to end of comment line */
    }
    /*there seems to be a difference between color and b/w.  This line is needed
      by b/w but doesn't effect color reading...*/
    fseek(in_file, -1, SEEK_CUR);             /* backup one character*/
    fscanf(in_file, "%" SCNd16, &(img->width));
    fscanf(in_file, "%" SCNd16, &(img->height));
    fscanf(in_file, "%" SCNd16, &(img->max_val));
    printf("\n width  = %d", img->width);
    printf("\n height = %d", img->height);
    printf("\n max_val = %d", img->max_val);
    printf("\n");

    img->gray = (uint16_t**)calloc(img->width+1, sizeof(uint16_t));

    for(uint16_t i = 0; i < img->height + 1; i++) {
        img->gray[i] = (uint16_t*)calloc(img->height, sizeof(uint16_t));
    }

    for (row = img->height - 1; row >= 0; row--)
        for (col = 0; col < img->width; col++) {
            fscanf(in_file, "%d", &ch_int);
            msg("row:%d, col:%d\n",row,col);
            img->gray[row][col] = ch_int;
        }

    fclose(in_file);
    printf("\nDone reading file.\n");
    return 1;
}