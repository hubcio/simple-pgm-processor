#include "main.h"

int main(void){
    struct program state;

    while(1){
        switch(state.menu){
            case LOAD:
                break;

            case QUIT:
                return 0;

            case SAVE:break;
            case NEW_IMG:break;
            case SHOW_CURRENT_IMAGES:break;
            case PROCESS_IMAGE:break;

            case DEFAULT:
            default:
                print_menu();
                state.menu = get_int(DEFAULT, QUIT, "Type number and click enter:");
                break;
        }
    }
}

void print_menu(void){
    for(int i=0; i<=QUIT; i++){
        i == LOAD ? msg("%d - Load PGM image from file", LOAD) : 0;
        i == SAVE ? msg("%d - Save PGM image to file", SAVE) : 0;
        i == NEW_IMG ? msg("%d - New PGM image", NEW_IMG) : 0;
        i == SHOW_CURRENT_IMAGES ? msg("%d - Info about loaded images", SHOW_CURRENT_IMAGES) : 0;
        i == PROCESS_IMAGE ? msg("%d - Process one of loaded images", PROCESS_IMAGE) : 0;
        i == QUIT ? msg("%d - Exit program", QUIT) : 0;
    }
}

int16_t get_int(int16_t min, int16_t max, char *description){
    if (description!=NULL) msg("%s", description);
    int n = 0;
    char buffer[128];
    fgets(buffer,sizeof(buffer),stdin);
    n = atoi(buffer);
    fflush(stdin);
    if (n<min || n>max) return -1;
    else return n;
}