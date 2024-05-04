#include <SDL2/SDL.h>
#include "time.h"

char *show_time(Uint32 miliseconds){
    int seconds = (miliseconds / 1000) % 60;
    int minutes = (miliseconds / (1000 * 60) % 60);
    int hours = (miliseconds / (1000 * 60 * 60) % 24);
    char seconds_str[10];
    sprintf(seconds_str, "%d", seconds);
    char minutes_str[10];
    sprintf(minutes_str, "%d", minutes);
    char hours_str[10];
    sprintf(hours_str, "%d", hours);
    char *result = malloc(strlen(seconds_str) + strlen(minutes_str) + strlen(hours_str) + 5 );
    strcpy(result, seconds_str);
    strcat(result, ":");
    strcat(result, minutes_str);
    strcat(result, ":");
    strcat(result, hours_str);
    return result;
}
