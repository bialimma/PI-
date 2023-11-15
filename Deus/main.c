#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

const int SCREEN_W = 800;
const int SCREEN_H = 600;
const int CARD_W = 100;
const int CARD_H = 150;
const float FPS = 60;

#define CARD_1_AREA_X_START 50
#define CARD_1_AREA_X_END (CARD_1_AREA_X_START + CARD_W)
#define CARD_2_AREA_X_START (CARD_1_AREA_X_END + 50)
#define CARD_2_AREA_X_END (CARD_2_AREA_X_START + CARD_W)
#define CARD_3_AREA_X_START (CARD_2_AREA_X_END + 50)
#define CARD_3_AREA_X_END (CARD_3_AREA_X_START + CARD_W)

typedef struct Card {
    float x, y;
    int value;
    int grabbed;  // Indica se a carta está sendo arrastada
} Card;

typedef struct Player {
    int lives;
    int score;
} Player;

void initCard(Card* card, float x, float y, int value) {
    card->x = x;
    card->y = y;
    card->value = value;
    card->grabbed = 0;
}

void draw_card(Card card) { //carta 
    al_draw_filled_rectangle(card.x, card.y,
        card.x + CARD_W, card.y + CARD_H,
        al_map_rgb(255, 255, 255));
    al_draw_textf(al_create_builtin_font(), al_map_rgb(0, 0, 0),
        card.x + CARD_W / 2, card.y + CARD_H / 2 - 10, ALLEGRO_ALIGN_CENTER,
        "%d", card.value);
}

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

    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
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
    initCard(&cards[0], 50, SCREEN_H / 2 - CARD_H / 2, 1);
    initCard(&cards[1], 200, SCREEN_H / 2 - CARD_H / 2, 2);
    initCard(&cards[2], 350, SCREEN_H / 2 - CARD_H / 2, 3);

    int playing = 1;
    int checkPositions = 0; // Flag para ativar/desativar a verificação das posições das cartas
    int positionsChecked = 0; // Flag para indicar se as posições das cartas foram verificadas

    al_start_timer(timer);

    while (playing) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            draw_scenario();
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
                if (cards[0].x < CARD_1_AREA_X_START || cards[0].x > CARD_1_AREA_X_END ||
                    cards[1].x < CARD_2_AREA_X_START || cards[1].x > CARD_2_AREA_X_END ||
                    cards[2].x < CARD_3_AREA_X_START || cards[2].x > CARD_3_AREA_X_END ||
                    cards[0].value != 1 || cards[1].value != 2 || cards[2].value != 3) {
                    player.lives--; // Reduz a vida se as cartas não estiverem nas posições corretas
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