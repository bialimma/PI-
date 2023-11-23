#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include "card.h"

const int SCREEN_W = 800;
const int SCREEN_H = 600;

const float FPS = 60;

typedef struct Player {
    int lives;
    int score;
} Player;

void draw_scenario() {
    al_clear_to_color(al_map_rgb(233, 229, 189));
}

void draw_player_info(Player player) {
    al_draw_textf(al_create_builtin_font(), al_map_rgb(0, 0, 0),
        SCREEN_W / 2, 10, ALLEGRO_ALIGN_CENTER,
        "Lives: %d | Score: %d", player.lives, player.score);
}

int main() {
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_BITMAP* img;

    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    if (!al_init_image_addon()) {
        printf("couldn't initialize image\n");
        return 1;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }

    if (!al_install_mouse()) {
        fprintf(stderr, "failed to install mouse!\n");
        return -1;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to install mouse!\n");
        return -1;
    }

    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    Player player = { 3, 0 };
    Card cards[3];
    initCard(&cards[0], 50, SCREEN_H / 2 - CARD_H / 2, 1, al_load_bitmap("cards.jpg"));
    initCard(&cards[1], 200, SCREEN_H / 2 - CARD_H / 2, 2, al_load_bitmap("cards.jpg"));
    initCard(&cards[2], 350, SCREEN_H / 2 - CARD_H / 2, 3, al_load_bitmap("cards.jpg"));

    int fase = 0;

    img = al_load_bitmap("cachorro.jpg");

    int playing = 1;
    int checkPositions = 0; // Flag para ativar/desativar a verificação das posições das cartas
    int positionsChecked = 0; // Flag para indicar se as posições das cartas foram verificadas

    al_start_timer(timer);

    while (playing) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            draw_scenario();
            al_draw_bitmap(img, 0, 0, 0);

            draw_player_info(player);

            if (checkPositions) {
                // Desenha as linhas de contorno das posições das cartas
                al_draw_rectangle(CARD_1_AREA_X_START, SCREEN_H / 2 - CARD_H / 2,
                    CARD_1_AREA_X_END, SCREEN_H / 2 + CARD_H / 2,
                    al_map_rgb(255, 0, 0), 2);
                al_draw_rectangle(CARD_2_AREA_X_START, SCREEN_H / 2 - CARD_H / 2,
                    CARD_2_AREA_X_END, SCREEN_H / 2 + CARD_H / 2,
                    al_map_rgb(255, 0, 0), 2);
                al_draw_rectangle(CARD_3_AREA_X_START, SCREEN_H / 2 - CARD_H / 2,
                    CARD_3_AREA_X_END, SCREEN_H / 2 + CARD_H / 2,
                    al_map_rgb(255, 0, 0), 2);
            }

            for (int i = 0; i < 3; ++i) {
                draw_card(cards[i]);
            }

            al_flip_display();
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            playing = 0;
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (ev.mouse.button & 1) {
                for (int i = 0; i < 3; ++i) {
                    if (ev.mouse.x >= cards[i].x && ev.mouse.x <= cards[i].x + CARD_W &&
                        ev.mouse.y >= cards[i].y && ev.mouse.y <= cards[i].y + CARD_H) {
                        // Inicia o arrasto da carta
                        cards[i].grabbed = 1;
                    }
                }
            }
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            if (ev.mouse.button & 1) {
                for (int i = 0; i < 3; ++i) {
                    // Encerra o arrasto da carta
                    cards[i].grabbed = 0;
                }
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                positionsChecked = 1;
                checkPositions = 0;

                // Verifica se as cartas estão nas posições corretas
                if (!is_ordered(cards)) {
                    player.lives--; // Reduz a vida se as cartas não estiverem nas posições corretas
                }
                else {
                    player.lives++;
                    fase++;

                    if (fase == 1) {
                        initCard(&cards[3], 450, SCREEN_H / 2 - CARD_H / 2, 1, al_load_bitmap("cards.jpg"));
                    }
                    else if (fase == 2) {

                    }
                }
            }
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            for (int i = 0; i < 3; ++i) {
                // Atualiza a posição da carta enquanto estiver sendo arrastada
                if (cards[i].grabbed) {
                    cards[i].x = ev.mouse.x - CARD_W / 2;
                    cards[i].y = ev.mouse.y - CARD_H / 2;
                }
            }
        }
    }

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}