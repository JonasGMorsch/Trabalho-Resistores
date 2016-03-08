#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

int main()
{
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_FONT *font1 = NULL;
    ALLEGRO_FONT *font2 = NULL;
    ALLEGRO_BITMAP  *img   = NULL;

    al_init();

    display = al_create_display(800, 600);
    al_clear_to_color(al_map_rgb(216,216,191));
    al_set_window_title(display, "Base de dados de Resistores");
    al_init_image_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon();// initialize the ttf (True Type Font) addon

    img = al_load_bitmap("res2.png");
    font1 = al_load_ttf_font("pirulen.TTF",15,0 );
    font2 = al_load_ttf_font("pirulen.TTF",20,0 );
    al_draw_bitmap(img, 1, 1, 1);

    // Exemplo de impressão de valores variáveis
    char *texto = "BASE DE DADOS DE RESISTORES";
    char *texto1 = "Consultar Resistores";
    char *texto2 = "Adicionar Resistores";
    char *texto3 = "Buscar Resistores";
    char *texto4 = "Apagar Resistores";
    char *texto5 = "Retirar Resistores";
    char *texto6 = "Salvar e Sair";
    char *texto7 = "Sair";
    al_draw_textf(font2, al_map_rgb(0, 0, 0), 510, 10, ALLEGRO_ALIGN_CENTRE, "%s", texto);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 100, ALLEGRO_ALIGN_CENTRE, "%s", texto1);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 140, ALLEGRO_ALIGN_CENTRE, "%s", texto2);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 180, ALLEGRO_ALIGN_CENTRE, "%s", texto3);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 220, ALLEGRO_ALIGN_CENTRE, "%s", texto4);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 260, ALLEGRO_ALIGN_CENTRE, "%s", texto5);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 300, ALLEGRO_ALIGN_CENTRE, "%s", texto6);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 340, ALLEGRO_ALIGN_CENTRE, "%s", texto7);

    al_flip_display();

    al_rest(15.0);
    al_destroy_bitmap(img);
    al_destroy_display(display);
    return 0;
}
