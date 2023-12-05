#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

#define TELA_INICIAL_H_

const int START_W = 1280;
const int START_H = 720;

const float FPS = 60;


typedef struct Start

void draw_scenario()
{
    al_clear_to_color(al_map_rgb(233, 229, 189));
}

void draw_player_info(Player player)
{
    al_draw_textf(al_create_builtin_font(), al_map_rgb(255, 255, 255),
        SCREEN_W / 2, 10, ALLEGRO_ALIGN_CENTER,
        "Lives: %d", player.lives);
}

typedef struct Start 

// Fun��o para a tela de inicializa��o
bool telaInicial(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue);



