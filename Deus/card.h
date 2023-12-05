#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

const int CARD_W = 120;
const int CARD_H = 170;

#define CARD_1_AREA_X_START 50
#define CARD_1_AREA_X_END (CARD_1_AREA_X_START + CARD_W)
#define CARD_2_AREA_X_START (CARD_1_AREA_X_END + 50)
#define CARD_2_AREA_X_END (CARD_2_AREA_X_START + CARD_W)
#define CARD_3_AREA_X_START (CARD_2_AREA_X_END + 50)
#define CARD_3_AREA_X_END (CARD_3_AREA_X_START + CARD_W)

typedef struct Card
{
    float x, y;
    int value;
    int grabbed; // Indica se a carta est� sendo arrastada
    bool selected;
    ALLEGRO_BITMAP *imagem;
} Card;

void initCard(Card *card, float x, float y, int value, ALLEGRO_BITMAP *imagem)
{
    card->x = x;
    card->y = y;
    card->value = value;
    card->imagem = imagem;
    card->selected = false;
}

void draw_card(Card card)
{
    al_draw_filled_rectangle(card.x, card.y,
                             card.x + CARD_W, card.y + CARD_H,
                             al_map_rgb(255, 255, 255));
    al_draw_bitmap(card.imagem, card.x, card.y, 0);
    /*al_draw_textf(al_create_builtin_font(), al_map_rgb(0, 0, 0),
                  card.x + CARD_W / 2, card.y + CARD_H / 2 - 10, ALLEGRO_ALIGN_CENTER,
                  "%d", card.value);*/
}

bool is_ordered(Card cards[], int card_quantity)
{
    for (int i = 0; i < card_quantity; i++)
    {
        for (int j = i; j < card_quantity; j++)
        {
            if (cards[i].x > cards[j].x)
            {
                return false;
            }
            else 
                return true;
        }
    }

    //return true;
}
