#ifndef PALITO_H_INCLUDED
#define PALITO_H_INCLUDED

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

class Palito
{
private:
    ALLEGRO_BITMAP *palito;

public:
    Palito();
    void initImage();
    void drawImage(int, int, ALLEGRO_DISPLAY*);
    void destroyImage();
};

Palito::Palito()
{
    palito = NULL;
}

void Palito::initImage()
{
    palito = al_load_bitmap("palito.png");
    al_convert_mask_to_alpha(palito, al_map_rgb(255, 0, 255));
}

void Palito::drawImage(int x, int y, ALLEGRO_DISPLAY *display)
{
    al_draw_bitmap(palito, x, y, 0);
}

void Palito::destroyImage()
{
    al_destroy_bitmap(palito);
}

#endif // PALITO_H_INCLUDED
