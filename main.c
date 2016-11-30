#include <zlib.h>
#include "main.h"

int main(void) {

    struct program state;

    init_program(&state);

    while (1) {
        switch (state.menu) {
            case LOAD: {
                struct pgm_image *img = init_image();

                get_string(img->name, "Type filename to read:");

                if (get_pgm_file(img->name, img) == -1) {
                    if (state.number_of_images > 0 && strcmp(state.images[state.number_of_images]->name, img->name) == 0) state.number_of_images--;
                    delete_image(img);
                }
                else
                {
                    state.images[state.number_of_images] = img;
                    state.number_of_images++;
                }
                state.menu = DEFAULT;
            }
                break;
            case SAVE: {
                if (state.number_of_images == 0) {
                    msg("Nothing to save!");
                    state.menu = DEFAULT;
                    break;
                }
                char name[100];
                get_string(name, "Type filename to read:");
                uint8_t image_to_save = get_int(-1, state.number_of_images, "Which image do You want to save? \nType -1 to cancel!");

                if (image_to_save == -1){
                    break;
                }
                save_pgm(name, state.images[image_to_save]);

                state.menu = DEFAULT;
            }
                break;
            case DELETE:
            {
                if(state.number_of_images == 0) {
                    msg("Nothing to delete!");
                    state.menu = DEFAULT;
                    break;
                }
                msg("Deleting last PGM image!");

                delete_image(state.images[state.number_of_images-1]);
                state.number_of_images--;

                state.menu = DEFAULT;
            }
                break;
            case SHOW_CURRENT_IMAGES:
                msg("\nCurrent stored images:");
                for (int i = 0; i < state.number_of_images; i++) {
                    msg("%d - [%s] (%d x %d)px", i, state.images[i]->name, state.images[i]->width, state.images[i]->height);
                }
                msg("In total: %d\n", state.number_of_images);
                state.menu = DEFAULT;
                break;
            case PROCESS_IMAGE:
                state.menu = DEFAULT;
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
        i == DELETE ?               msg("%d - Delete PGM image from memory", DELETE) : 0;
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
int8_t get_pgm_file(char filename[], struct pgm_image *img) {
    FILE *in_file;
    char ch;
    int row, col, type;
    int ch_int;
//    chdir("..");
//    chdir("..");
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
    strcpy(img->name,filename);
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
    fscanf(in_file, "%u", &(img->width));
    fscanf(in_file, "%u", &(img->height));
    fscanf(in_file, "%u", &(img->max_val));
    printf("\n width  = %d", img->width);
    printf("\n height = %d", img->height);
    printf("\n max_val = %d", img->max_val);
    printf("\n");

    if (img->width == 0 || img->height == 0 || img->max_val == 0) {
        fprintf(stderr,"ERROR(3): Wrong height, width or max_val?\n");
        return -1;
    }

    img->gray = (uint16_t**)calloc(img->height+1, sizeof(uint16_t*));

    for(size_t i = 0; i < img->height+1; i++) {
        img->gray[i] = (uint16_t*)calloc(img->width, sizeof(uint16_t));
    }

    for (row = img->height - 1; row >= 0; row--)
        for (col = 0; col < img->width; col++) {
            fscanf(in_file, "%d", &ch_int);
            img->gray[row][col] = ch_int;
        }

    fclose(in_file);

    printf("\nDone reading file.\n");
    return 1;
}

void delete_image(struct pgm_image * img){
    if((img->width = 0) || (img->height == 0) || (img->max_val == 0)) {return;}

    free(img);
}

void init_program(struct program * p){
    p->number_of_images = 0;
}

void save_pgm(const char *filename, const struct pgm_image *img) {

    int i, j, nr, nc;
    int gray;
    FILE *iop;

    nr = img->height;
    nc = img->width;

    iop = fopen(filename, "w");
    fprintf(iop, "P2\n");
    fprintf(iop, "%d %d\n", nc, nr);
    fprintf(iop, "255\n");

    for(i = nr - 1; i >= 0; i--)
    {
        for(j = 0; j < nc; j++)
        {
            gray = img->gray[i][j];
            if(gray <  0)
            {
                printf("IMG_WRITE: Found value %d at row %d col %d\n", gray, i, j);
                printf("           Setting red to zero\n");
                gray = 0;
            }

            if(gray  > 255)
            {
                printf("IMG_WRITE: Found value %d at row %d col %d\n", gray, i, j);
                printf("           Setting red to 255\n");
                gray = 255;
            }
            fprintf(iop,"%d ",gray);
        }
        fprintf(iop, "\n");
    }
    fprintf(iop, "\n");
    fclose(iop);
}
