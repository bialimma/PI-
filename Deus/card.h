#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

const int CARD_W = 100;
const int CARD_H = 150;

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
    ALLEGRO_BITMAP* imagem;
} Card;

void initCard(Card* card, float x, float y, int value, ALLEGRO_BITMAP* imagem) {
    card->x = x;
    card->y = y;
    card->value = value;
    card->grabbed = 0;
    card->imagem = imagem;
}

void draw_card(Card card) { //carta 
    al_draw_filled_rectangle(card.x, card.y,
        card.x + CARD_W, card.y + CARD_H,
        al_map_rgb(255, 255, 255));
    al_draw_bitmap(card.imagem, card.x, card.y, 0);
    al_draw_textf(al_create_builtin_font(), al_map_rgb(0, 0, 0),
        card.x + CARD_W / 2, card.y + CARD_H / 2 - 10, ALLEGRO_ALIGN_CENTER,
        "%d", card.value);
}

bool is_ordered(Card cards[]) {
    if (cards[0].x < cards[1].x && cards[1].x < cards[2].x && cards[2].x < cards[3].x)//add mais cartas 
    {
        return true;
    }
    else {
        return false;
    }
}