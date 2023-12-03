#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include "card.h"
#include "shadow.h"

const int SCREEN_W = 1280;
const int SCREEN_H = 720;

const float FPS = 60;

typedef struct Player
{
    int lives;
    int card_quantity;
    int card_grabbed_index;
    bool is_card_being_dragged;
} Player;

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

Card possible_cards[10];

void add_card(Card cards[], int index, ALLEGRO_BITMAP* card_bitmap)
{
    Card previousCard = cards[index - 1];
    Card card;

    initCard(&card, previousCard.x + 50, SCREEN_H - 174, previousCard.value + 1, card_bitmap);
    cards[index] = card;
}

void start_screen()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    ALLEGRO_FONT* font = al_create_builtin_font();
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 - 30,
        ALLEGRO_ALIGN_CENTER, "Pressione ENTER para iniciar o jogo");
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 30,
        ALLEGRO_ALIGN_CENTER, "Use o mouse para arrastar as cartas e ordená-las");
    al_flip_display();
}

int main()
{
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_BITMAP* img;
    ALLEGRO_BITMAP* card_bitmap[10];

    if (!al_init())
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display)
    {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    if (!al_init_image_addon())
    {
        printf("couldn't initialize image\n");
        return 1;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }

    if (!al_install_mouse())
    {
        fprintf(stderr, "failed to install mouse!\n");
        return -1;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "failed to install mouse!\n");
        return -1;
    }

    timer = al_create_timer(1.0 / FPS);
    if (!timer)
    {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue)
    {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    srand(time(NULL));
    card_bitmap[0] = al_load_bitmap("cards.jpg");
    card_bitmap[1] = al_load_bitmap("cards.jpg");
    card_bitmap[2] = al_load_bitmap("cards.jpg");
    card_bitmap[3] = al_load_bitmap("cards.jpg");
    card_bitmap[4] = al_load_bitmap("cards.jpg");
    card_bitmap[5] = al_load_bitmap("cards.jpg");
    card_bitmap[6] = al_load_bitmap("cards.jpg");
    card_bitmap[7] = al_load_bitmap("cards.jpg");
    card_bitmap[8] = al_load_bitmap("cards.jpg");
    card_bitmap[9] = al_load_bitmap("cards.jpg");

    Player player = { .lives = 3, .card_quantity = 2 }; // player = {lives}
    Card cards[9];
    Shadow shadows[9];
    initCard(&cards[0], 250, SCREEN_H - 174, 0, card_bitmap[0]);
    initCard(&cards[1], 300, SCREEN_H - 174, 1, card_bitmap[1]);

    for (int i = 0; i < 10; ++i)
    {
        initShadow(&shadows[i], 250 + (i * 125), 200);
    }

    int fase = 0;

    img = al_load_bitmap("game2d_background.jpg");

    int playing = 1;
    int checkPositions = 0;   // Flag para ativar/desativar a verificação das posições das cartas
    int positionsChecked = 0; // Flag para indicar se as posições das cartas foram verificadas

    al_start_timer(timer);

    // Mostra a tela de início
    start_screen();

    // Aguarda até que o jogador pressione ENTER para começar o jogo
    bool game_started = false;
    while (!game_started)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
        {
            game_started = true;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            playing = 0;
            game_started = true; // Sair do loop se a janela for fechada
        }
    }

    while (playing)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            draw_scenario();
            al_draw_bitmap(img, 0, 0, 0);
            draw_player_info(player);

            for (int i = 0; i < player.card_quantity; ++i)
            {
                draw_shadow(shadows[i]);
            }

            if (checkPositions)
            {
                // Desenha as linhas de contorno das posições das cartas
                /*al_draw_rectangle(CARD_1_AREA_X_START, SCREEN_H / 2 - CARD_H / 2,
                    CARD_1_AREA_X_END, SCREEN_H / 2 + CARD_H / 2,
                    al_map_rgb(255, 0, 0), 2);
                al_draw_rectangle(CARD_2_AREA_X_START, SCREEN_H / 2 - CARD_H / 2,
                    CARD_2_AREA_X_END, SCREEN_H / 2 + CARD_H / 2,
                    al_map_rgb(255, 0, 0), 2);
                al_draw_rectangle(CARD_3_AREA_X_START, SCREEN_H / 2 - CARD_H / 2,
                    CARD_3_AREA_X_END, SCREEN_H / 2 + CARD_H / 2,
                    al_map_rgb(255, 0, 0), 2);*/
            }

            for (int i = 0; i < player.card_quantity; ++i)
            {
                draw_card(cards[i]);
            }

            if (player.lives == 0)
            {
                al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(255, 0, 0, 100));
            }

            al_flip_display();
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            playing = 0;
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (ev.mouse.button == 1 && player.lives > 0)
            {
                for (int i = 0; i < player.card_quantity; ++i)
                {
                    if (ev.mouse.x >= cards[i].x && ev.mouse.x <= cards[i].x + CARD_W &&
                        ev.mouse.y >= cards[i].y && ev.mouse.y <= cards[i].y + CARD_H)
                    {
                        // Inicia o arrasto da carta
                        player.card_grabbed_index = i;
                        player.is_card_being_dragged = true;
                    }
                }
            }
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            if (ev.mouse.button == 1 && player.lives > 0)
            {
                player.is_card_being_dragged = false;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (player.lives == 0)
            {
                break;
            }
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                positionsChecked = 1;
                checkPositions = 0;

                // Verifica se as cartas estão nas posições corretas
                if (!is_ordered(cards, player.card_quantity))
                {
                    player.lives--; // Reduz a vida se as cartas não estiverem nas posições corretas
                }
                else
                {
                   // player.lives++;
                }
                if (player.card_quantity < 10)
                {
                    player.card_quantity++;
                    add_card(cards, player.card_quantity - 1, card_bitmap[player.card_quantity - 1]);
                }
            }
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            // Atualiza a posição da carta enquanto estiver sendo arrastada
            if (player.is_card_being_dragged)
            {
                cards[player.card_grabbed_index].x = ev.mouse.x - CARD_W / 2;
                cards[player.card_grabbed_index].y = ev.mouse.y - CARD_H / 2;
            }
        }

        if (!playing)
        {
            break;
        }
    }
  

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(img);
    al_destroy_timer(timer);
    

    for (int i = 0; i < 10; i++)
    {
        al_destroy_bitmap(card_bitmap[i]);
    }

    return 0;
}