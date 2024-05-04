#include "fonts.h"
#include <SDL2/SDL_ttf.h>

TTF_Font *fontDialogBox = NULL;
TTF_Font *fontDialogBoxBold = NULL;
TTF_Font *fontScroll = NULL;

int fontDialogBoxSize = 20;
int fontDialogBoxBoldSize = 20;
int fontScrollSize = 23;

int loadFonts(){
    fontDialogBox = TTF_OpenFont("asset/Fonts/DisposableDroidBB.ttf", fontDialogBoxSize);
    fontDialogBoxBold = TTF_OpenFont("asset/Fonts/DisposableDroidBB_bld.ttf", fontDialogBoxBoldSize);
    fontScroll = TTF_OpenFont("asset/Fonts/ChopinScript.ttf", fontScrollSize);

    if (!fontDialogBox || !fontDialogBoxBold || !fontScroll){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init fonts : %s", SDL_GetError());
        puts("Error in fonts init");
		exit(-1);
    }
    return 0;
}

int freeFonts(){

    TTF_CloseFont(fontDialogBox);
    TTF_CloseFont(fontDialogBoxBold);
    TTF_CloseFont(fontScroll);
    fontDialogBox = NULL;
    fontDialogBoxBold = NULL;
    fontScroll = NULL;
    return 0;
}
