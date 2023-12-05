#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

const int SHADOW_W = 120;
const int SHADOW_H = 170;

typedef struct Shadow
{
    float x, y;
} Shadow;

void initShadow(Shadow *shadow, float x, float y)
{
    shadow->x = x;
    shadow->y = y;
}

void draw_shadow(Shadow shadow)
{ // carta
    al_draw_filled_rectangle(shadow.x, shadow.y, shadow.x + SHADOW_W, shadow.y + SHADOW_H, al_map_rgba(17, 17, 17, 180));
}