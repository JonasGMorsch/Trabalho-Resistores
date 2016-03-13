#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <stdbool.h>

//#include <stdarg.h> ///

const int SCREEN_W = 800;
const int SCREEN_H = 600;
enum teclas {key_1, key_2, key_3, key_4, key_5, key_6, key_7,key_esc,total_teclas};
int Keys[total_teclas];
int j=100;
bool doexit = false;
bool doimprime = false;

char str[17];
char *texto = "BASE DE DADOS DE RESISTORES";
char *texto1 = "1. Consultar Resistores";
char *texto2 = "2. Adicionar Resistores";
char *texto3 = "3. Buscar Resistores";
char *texto4 = "4. Apagar Resistores";
char *texto5 = "5. Retirar Resistores";
char *texto6 = "6. Salvar e Sair";
char *texto7 = "7. Sair";
char *texto8 = "Aperte ESC para voltar ao Menu.";
char *texto9 = "Instituto Federal";
char *texto10 = "Santa Catarina";


ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_FONT *font1 = NULL;
ALLEGRO_FONT *font2 = NULL;
ALLEGRO_FONT *font3 = NULL;
ALLEGRO_BITMAP  *img   = NULL;
ALLEGRO_BITMAP  *imagem1  = NULL;
ALLEGRO_BITMAP *imagem2 = NULL;
ALLEGRO_BITMAP *imagem3 = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_COLOR c;


struct serie
{
    struct serie *esquerda;
    int numero_serie;
    float valor_tolerancia;
    struct potencia* potencia;
    struct serie *direita;
};
typedef struct serie Serie;

struct potencia
{
    struct potencia *esquerda;
    float potencia_resistor;
    struct resistencia* resistencia;
    struct potencia *direita;
};
typedef struct potencia Potencia;

struct resistencia
{
    struct resistencia *esquerda;
    float valor_resistencia;
    int quantidade_resistores;
    struct resistencia *direita;
};
typedef struct resistencia Resistencia;

struct cabecalho
{
    struct lista* iniciodecabecalho;
    int nelementos;
    struct lista* fimdecabecalho;
};
typedef struct cabecalho Estruturadocabecalho;


/// //////////////////////////////////////////////////////////INICIO INSERSÃO ADIÇÃO REMOÇÁO RESISTORES////////////////////////////////////
void inserir_resistencia(Resistencia**Arvore, float valor_resistencia,int quantidade_resistores,int soma)
{
    if(*Arvore == NULL)
    {
        if(soma==1)
        {
            Resistencia* ResistenciaN = (Resistencia*) malloc(sizeof(Resistencia));
            ResistenciaN->esquerda = NULL;
            ResistenciaN->direita = NULL;
            ResistenciaN->valor_resistencia = valor_resistencia;
            ResistenciaN->quantidade_resistores=quantidade_resistores;
            *Arvore=ResistenciaN;
            printf("\n *Novo resistor gerado com sucesso* \n");
        }
    }
    else
    {
        if(valor_resistencia < (*Arvore)->valor_resistencia)
            inserir_resistencia(&(*Arvore)->esquerda,valor_resistencia,quantidade_resistores,soma);

        if(valor_resistencia > (*Arvore)->valor_resistencia)
            inserir_resistencia(&(*Arvore)->direita, valor_resistencia,quantidade_resistores,soma);

        if(valor_resistencia == (*Arvore)->valor_resistencia)
        {
            if(soma==0)
            {
                if((*Arvore)->quantidade_resistores<quantidade_resistores)
                    printf("\n *A quantidate a ser retirada é maior do que a quantidate em estoque* \n");
                else
                    (*Arvore)->quantidade_resistores-=quantidade_resistores;
                printf("\n *Resistor retirado com sucesso* \n");
            }
            else if(soma==1)
            {
                if((*Arvore)->quantidade_resistores==-1)
                    (*Arvore)->quantidade_resistores=0;
                (*Arvore)->quantidade_resistores+=quantidade_resistores;
                printf("\n *Resistor adicionado com sucesso* \n");
            }
            else if(soma==2)
            {
                (*Arvore)->quantidade_resistores=-1;
                printf("\n *Resistor removido com sucesso* \n");
            }
        }
    }
}
void inserir_potencia(Potencia**Arvore, float potencia_resistor, float valor_resistencia,int quantidade_resistores,int soma)
{
    if(*Arvore == NULL)
    {
        Potencia* PotenciaN = (Potencia*) malloc(sizeof(Potencia));
        PotenciaN->esquerda = NULL;
        PotenciaN->direita = NULL;
        PotenciaN->resistencia =NULL;
        PotenciaN->potencia_resistor = potencia_resistor;
        *Arvore=PotenciaN;
        inserir_resistencia(&PotenciaN->resistencia,valor_resistencia,quantidade_resistores,soma);
    }
    else
    {
        if(potencia_resistor < (*Arvore)->potencia_resistor)
            inserir_potencia(&(*Arvore)->esquerda,potencia_resistor,valor_resistencia,quantidade_resistores,soma);

        if(potencia_resistor > (*Arvore)->potencia_resistor)
            inserir_potencia(&(*Arvore)->direita,potencia_resistor,valor_resistencia,quantidade_resistores,soma);

        if(potencia_resistor == (*Arvore)->potencia_resistor)
            inserir_resistencia(&(*Arvore)->resistencia,valor_resistencia,quantidade_resistores,soma);
    }
}
void inserir(Serie**Arvore, int numero_serie,float potencia_resistor,float valor_resistencia,int quantidade_resistores,int soma)
{
    if(*Arvore == NULL)
    {
        Serie* SerieN = (Serie*) malloc(sizeof(Serie));
        SerieN->esquerda = NULL;
        SerieN->direita = NULL;
        SerieN->numero_serie = numero_serie;
        //SerieN->valor_tolerancia=valor_tolerancia;
        SerieN->potencia = NULL;
        *Arvore=SerieN;
        inserir_potencia(&(*Arvore)->potencia,potencia_resistor,valor_resistencia,quantidade_resistores,soma);
    }
    else
    {
        if(numero_serie < (*Arvore)->numero_serie)
            inserir(&(*Arvore)->esquerda,numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores,soma);

        if(numero_serie > (*Arvore)->numero_serie)
            inserir(&(*Arvore)->direita, numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores,soma);

        if(numero_serie == (*Arvore)->numero_serie)
            inserir_potencia(&(*Arvore)->potencia,potencia_resistor,valor_resistencia,quantidade_resistores,soma);
    }
}
/// //////////////////////////////////////////////////////////FIM INSERSÃO ADIÇÃO REMOÇÁO RESISTORES////////////////////////////////////

/// /////////////////////////////////////INICIO EXPORTA TXT ///////////////////////////////////////////
void Exporta_Arquivo_3(Resistencia*Arvore,int numero_serie,float potencia_resistor,FILE * fp)
{
    if(Arvore != NULL)
    {
        Exporta_Arquivo_3(Arvore->esquerda,numero_serie,potencia_resistor,fp);
        //printf(" Série: E%d\tPotência:%5.2f\tResistencia:%9.1f\tQuantidade:%6d\n",numero_serie, potencia_resistor,Arvore->valor_resistencia,Arvore->quantidade_resistores);
        if(Arvore->quantidade_resistores>0)
            fprintf(fp,"%d\t %5.2f %12.1f %6d\n",numero_serie, potencia_resistor, Arvore->valor_resistencia, Arvore->quantidade_resistores);
        Exporta_Arquivo_3(Arvore->direita,numero_serie,potencia_resistor,fp);
    }
}
void Exporta_Arquivo_2(Potencia*Arvore, int numero_serie,FILE * fp)
{
    if(Arvore != NULL)
    {
        Exporta_Arquivo_2(Arvore->esquerda,numero_serie,fp);
        Exporta_Arquivo_3(Arvore->resistencia,numero_serie,Arvore->potencia_resistor,fp);
        Exporta_Arquivo_2(Arvore->direita,numero_serie,fp);
    }
}
void Exporta_Arquivo(Serie*Arvore,FILE * fp)
{
    if(Arvore != NULL)
    {
        Exporta_Arquivo(Arvore->esquerda,fp);
        Exporta_Arquivo_2(Arvore->potencia,Arvore->numero_serie,fp);
        Exporta_Arquivo(Arvore->direita,fp);
    }
}
/// ///////////////////////////////////////FIM EXPORTA TXT ///////////////////////////////////////////

/// ///////////////////////////////////////////////////INICIO BUSCA RESISTORES////////////////////////////////////////////////////
void Busca_resitencia(Resistencia*Arvore,int numero_serie,float potencia_resistor,float valor_resistencia,int quantidade_resistores)
{
    if(Arvore != NULL)
    {
        Busca_resitencia(Arvore->esquerda,numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores);
        if(((valor_resistencia == Arvore->valor_resistencia && quantidade_resistores ==0) || (quantidade_resistores==Arvore->quantidade_resistores && valor_resistencia ==0) || (valor_resistencia==0 &&quantidade_resistores ==0) || (valor_resistencia==Arvore->valor_resistencia && quantidade_resistores==Arvore->quantidade_resistores)) && (Arvore->quantidade_resistores>=0))
        {
            ///printf("Série: E%d\tPotência:%5.2f\tResistencia:%9.1f\tQuantidade:%6d\n",numero_serie, potencia_resistor,Arvore->valor_resistencia,Arvore->quantidade_resistores);
            al_draw_textf(font1, al_map_rgb(0,0,0),60,j,ALLEGRO_ALIGN_LEFT,"Serie: E%d",numero_serie);
            al_draw_textf(font1, al_map_rgb(0,0,0),200,j,ALLEGRO_ALIGN_LEFT,"Potencia: %.2f",potencia_resistor);
            al_draw_textf(font1, al_map_rgb(0,0,0),380,j,ALLEGRO_ALIGN_LEFT,"Resistencia: %.1f",Arvore->valor_resistencia);
            al_draw_textf(font1, al_map_rgb(0,0,0),600,j,ALLEGRO_ALIGN_LEFT,"Quantidade:%6d",Arvore->quantidade_resistores);
            j=j+20;
        }

        Busca_resitencia(Arvore->direita,numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores);
    }
}
void Busca_potencia(Potencia*Arvore,int numero_serie,float potencia_resistor,float valor_resistencia,int quantidade_resistores)
{
    if(Arvore != NULL)
    {
        Busca_potencia(Arvore->esquerda,numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores);
        if(potencia_resistor == Arvore->potencia_resistor || potencia_resistor ==0)
            Busca_resitencia(Arvore->resistencia,numero_serie, Arvore->potencia_resistor,valor_resistencia,quantidade_resistores);
        Busca_potencia(Arvore->direita,numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores);
    }
}
void Busca_serie(Serie*Arvore, int numero_serie,float potencia_resistor,float valor_resistencia,int quantidade_resistores)
{
    if(Arvore != NULL)
    {
        Busca_serie(Arvore->esquerda,numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores);
        if(numero_serie == Arvore->numero_serie || numero_serie ==0)
        {
            //printf("Série:E%d\n", Arvore->numero_serie);
            Busca_potencia(Arvore->potencia,Arvore->numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores);
        }
        Busca_serie(Arvore->direita,numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores);
    }
}
/// ///////////////////////////////////////////////////INICIO BUSCA RESISTORES////////////////////////////////////////////////////

bool inicializar()
{
    al_init();
    display = al_create_display(SCREEN_W, SCREEN_H);
    al_clear_to_color(al_map_rgb(168,168,168));
    al_set_window_title(display, "Base de dados de Resistores");
    al_init_image_addon();
    al_init_font_addon();
    al_init_primitives_addon();
    al_init_ttf_addon();
    al_install_mouse();
    al_install_keyboard();
    img = al_load_bitmap("res2.png");
    imagem1 = al_load_bitmap("fundo.png");
    imagem2 = al_load_bitmap("opcoes.png");
    imagem3 = al_load_bitmap("ifsc2.png");
    font1 = al_load_ttf_font("arial.TTF",18,15);
    font2 = al_load_ttf_font("arial.TTF",25,15 );
    font3 = al_load_ttf_font("arial.TTF",20,15 );
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    ///al_register_event_source(event_queue, al_get_mouse_event_source());
    return true;
}

char* manipular_entrada(ALLEGRO_EVENT ev)
{
    if (strlen(str) <= 16)
    {
        char temp[] = {ev.keyboard.unichar, '\0'};
        if (ev.keyboard.unichar == ' ')
        {
            strcat(str, temp);
        }
        else if (ev.keyboard.unichar >= '0' && ev.keyboard.unichar <= '9')
        {
            strcat(str, temp);
        }
        else if (ev.keyboard.unichar >= 'A' && ev.keyboard.unichar <= 'Z')
        {
            strcat(str, temp);
        }
        else if (ev.keyboard.unichar >= 'a' && ev.keyboard.unichar <= 'z')
        {
            strcat(str, temp);
        }
        else if (ev.keyboard.unichar =='.')
        {
            strcat(str, temp);
        }
    }

    if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
    {
        return (atoi(str));
    }
    if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0)
    {
        str[strlen(str) - 1] = '\0';
    }
}

void opera_resistor(Serie*Arvore,ALLEGRO_EVENT ev, int soma)
{
    int serie=0,quantidade=0,i=0;
    float resitencia=0,potencia=0,tolerancia=0,zeropontoum=0.1;
    char resposta;

    al_draw_filled_rectangle(450,150,450+200,150+40,al_map_rgb(205,205,205));
    al_draw_filled_rectangle(450,150+90,450+200,150+90+40,al_map_rgb(205,205,205));
    al_draw_filled_rectangle(450,150+180,450+200,150+180+40,al_map_rgb(205,205,205));
    if(soma!=2)
        al_draw_filled_rectangle(450,150+270,450+200,150+270+40,al_map_rgb(205,205,205));

    al_draw_textf(font3, al_map_rgb(0, 0, 0), 100, 100, ALLEGRO_ALIGN_LEFT, "Entre com os valores de:");
    al_draw_textf(font3, al_map_rgb(0, 0, 0), 440, 160, ALLEGRO_ALIGN_RIGHT, "Serie:");
    al_draw_textf(font3, al_map_rgb(0, 0, 0), 440, 250, ALLEGRO_ALIGN_RIGHT, "Potencia:");
    al_draw_textf(font3, al_map_rgb(0, 0, 0), 440, 340, ALLEGRO_ALIGN_RIGHT, "Resistencia:");
    if(soma!=2)
        al_draw_textf(font3, al_map_rgb(0, 0, 0), 440, 430, ALLEGRO_ALIGN_RIGHT, "Quantidade:");

    while(1)
    {
        while(1)
        {
            if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                break;

            al_wait_for_event(event_queue, &ev);
            if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
                if((ev.keyboard.keycode == ALLEGRO_KEY_DOWN)||(ev.keyboard.keycode == ALLEGRO_KEY_ENTER) || (ev.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER))
                    break;

            al_draw_filled_rectangle(450,150,450+200,150+40,al_map_rgb(255,255,255));
            manipular_entrada(ev);
            al_draw_textf(font1, al_map_rgb(0, 0, 0), 460, 160, ALLEGRO_ALIGN_LEFT, "%s",str);
            al_flip_display();
        }
        serie=atoi(str);
        str[0]='\0';
        al_draw_filled_rectangle(450,150,450+200,150+40,al_map_rgb(205,205,205));
        al_draw_textf(font1, al_map_rgb(0, 0, 0), 460, 160, ALLEGRO_ALIGN_LEFT, "%d",serie);
        //al_flip_display();
        if ((serie==6 || serie==12 || serie==24 || serie==48 || serie==96 || serie==192) ||(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE))
            break;
        else
            al_show_native_message_box(display,"Erro"," ", "Serie incorreta!", NULL, 0);
    }

    while(1)
    {
        if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            break;

        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
            if((ev.keyboard.keycode == ALLEGRO_KEY_DOWN)||(ev.keyboard.keycode == ALLEGRO_KEY_ENTER) || (ev.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER))
                break;

        al_draw_filled_rectangle(450,150+90,450+200,150+90+40,al_map_rgb(255,255,255));
        manipular_entrada(ev);
        al_draw_textf(font1, al_map_rgb(0, 0, 0), 460, 160+90, ALLEGRO_ALIGN_LEFT, "%s",str);
        al_flip_display();
    }
    potencia=atof(str);
    str[0]='\0';
    al_draw_filled_rectangle(450,150+90,450+200,150+90+40,al_map_rgb(205,205,205));
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 460, 160+90, ALLEGRO_ALIGN_LEFT, "%f",potencia);
    al_flip_display();

    while(1)
    {
        if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            break;

        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
            if((ev.keyboard.keycode == ALLEGRO_KEY_DOWN)||(ev.keyboard.keycode == ALLEGRO_KEY_ENTER) || (ev.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER))
                break;

        al_draw_filled_rectangle(450,150+180,450+200,150+180+40,al_map_rgb(255,255,255));
        manipular_entrada(ev);
        al_draw_textf(font1, al_map_rgb(0, 0, 0), 460, 160+180, ALLEGRO_ALIGN_LEFT, "%s",str);
        al_flip_display();
    }
    resitencia=atof(str);
    str[0]='\0';
    al_draw_filled_rectangle(450,150+180,450+200,150+180+40,al_map_rgb(205,205,205));
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 460, 160+180, ALLEGRO_ALIGN_LEFT, "%f",resitencia);
    al_flip_display();

    if(soma!=2)
    {
        while(1)
        {
            while(1)
            {
                if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    break;

                al_wait_for_event(event_queue, &ev);
                if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
                    if((ev.keyboard.keycode == ALLEGRO_KEY_DOWN)||(ev.keyboard.keycode == ALLEGRO_KEY_ENTER) || (ev.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER))
                        break;
                al_draw_filled_rectangle(450,150+270,450+200,150+270+40,al_map_rgb(255,255,255));
                manipular_entrada(ev);
                al_draw_textf(font1, al_map_rgb(0, 0, 0), 460, 160+270, ALLEGRO_ALIGN_LEFT, "%s",str);
                al_flip_display();
            }
            quantidade=atoi(str);
            str[0]='\0';
            al_draw_filled_rectangle(450,150+270,450+200,150+270+40,al_map_rgb(205,205,205));
            al_draw_textf(font1, al_map_rgb(0, 0, 0), 460, 160+270, ALLEGRO_ALIGN_LEFT, "%d",quantidade);

            if ((quantidade>0) ||(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) || (soma==3))
                break;
            else
                al_show_native_message_box(display,"Erro"," ", "Preencha a quantidade!", NULL, 0);
        }
    }

    al_draw_textf(font1, al_map_rgb(0, 0, 0), 400, 500, ALLEGRO_ALIGN_CENTRE, "Os valores estao corretos (S/N)?");
    al_flip_display();

    while(1)
    {
        if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            break;

        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
            if((ev.keyboard.keycode == ALLEGRO_KEY_N))
            {
                al_show_native_message_box(display," "," ", "Operacao Cancelada!", NULL, 0);
                break;
            }
        if((ev.keyboard.keycode == ALLEGRO_KEY_S))
        {
            ///al_show_native_message_box(display," "," ", "Operacao Concluida!", NULL, 0);
            if(soma==0 ||soma==1)
            {
                inserir(&Arvore,serie,potencia,resitencia,quantidade,soma);
                break;
            }
            if(soma==2)
            {
                inserir(&Arvore,serie,potencia,resitencia,0,soma);
                break;
            }

            if(soma==3)
            {
                j=100;
                al_clear_to_color(al_map_rgb(168,168,168));
                Busca_serie(Arvore,serie,potencia,resitencia,quantidade);
                al_draw_textf(font1, al_map_rgb(0, 0, 0), 400, 550, ALLEGRO_ALIGN_CENTRE, "%s", texto8);
                al_draw_textf(font2, al_map_rgb(0, 0, 0), 400, 50, ALLEGRO_ALIGN_CENTRE, "Busca concluida!");
                al_flip_display();
            }
            //break;
        }
    }
    al_flip_display();
}

void apaga_resistor(Serie*Arvore)
{
    int/* i=0,*/serie=0,quantidade=0;
    float resitencia=0,potencia=0;
    char resposta;

    printf("\n Entre com os valores de: \n\n Resistência:");
    scanf("%f",&resitencia);
    printf(" Série:");
    scanf("%d",&serie);
    printf(" Potência:");
    scanf("%f",&potencia);
    fflush(stdin);
    printf("\n Os Valores estão corretos? (S/N)");
    scanf("%c",&resposta);

    if (resposta=='S' || resposta=='s')
        inserir(&Arvore,serie,potencia,resitencia,quantidade,2);
    else
        printf("\n *A operação foi cancelada*\n");
}

void retira_resistor(Serie*Arvore)
{
    int/* i=0,*/serie=0,quantidade=0;
    float resitencia=0,potencia=0;
    char resposta;

    printf("\n Entre com os valores de: \n\n Resistência:");
    scanf("%f",&resitencia);
    printf(" Série:");
    scanf("%d",&serie);
    printf(" Potência:");
    scanf("%f",&potencia);
    printf("\n Qual será a quantidade retirada?\n ");
    scanf("%d",&quantidade);
    fflush(stdin);
    printf("\n Os Valores estão corretos? (S/N)");
    scanf("%c",&resposta);

    if (resposta=='S' || resposta=='s')
        inserir(&Arvore,serie,potencia,resitencia,quantidade,0);
    else
        printf("\n *A retirada de resistores foi cancelada*\n");
}

void busca_resistor(Serie*Arvore)
{
    int/* i=0,*/serie=0,quantidade=0;
    float resitencia=0,potencia=0;
    char sa[10];
    char sb[10];
    char sc[10];
    char sd[10];


    al_draw_textf(font3, al_map_rgb(0, 0, 0), 100, 100, ALLEGRO_ALIGN_LEFT, "Entre com os valores de:");
    al_draw_textf(font3, al_map_rgb(0, 0, 0), 440, 160, ALLEGRO_ALIGN_RIGHT, "Serie:");
    al_draw_textf(font3, al_map_rgb(0, 0, 0), 440, 250, ALLEGRO_ALIGN_RIGHT, "Potencia:");
    al_draw_textf(font3, al_map_rgb(0, 0, 0), 440, 340, ALLEGRO_ALIGN_RIGHT, "Resistencia:");
    al_draw_textf(font3, al_map_rgb(0, 0, 0), 440, 430, ALLEGRO_ALIGN_RIGHT, "Quantidade:");
    /*fflush(stdin);
    printf("\n Entre com os valores conhecidos de: \n\n Resistência:");
    fgets(sa, sizeof(sa), stdin);
    resitencia = atof(sa);
    printf(" Série:");
    fgets(sb, sizeof(sb), stdin);
    serie = atoi(sb);
    printf(" Potência:");
    fgets(sc, sizeof(sc), stdin);
    potencia = atof(sc);
    printf(" Quantidade:");
    fgets(sd, sizeof(sd), stdin);
    quantidade = atoi(sd);*/

    Busca_serie(Arvore,serie,potencia,resitencia,quantidade);

}
const char NOME_ARQ[] = "resistores.txt";
const char NOME_ARQ2[] = "resistores_arvore.txt";


void Imprime_menu(void)
{
    al_clear_to_color(al_map_rgb(168,168,168));
    al_draw_bitmap(img, 1, 1, 1);
    al_draw_bitmap(imagem3, 1,430, 0);
    al_draw_textf(font3, al_map_rgb(0, 0, 0), 140, 530, ALLEGRO_ALIGN_LEFT, "%s", texto9);
    al_draw_textf(font3, al_map_rgb(0, 0, 0), 140, 557, ALLEGRO_ALIGN_LEFT, "%s", texto10);
    al_draw_textf(font2, al_map_rgb(0, 0, 0), 510, 30, ALLEGRO_ALIGN_CENTRE, "%s", texto);
    al_draw_bitmap(imagem1,390,90,0);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 100, ALLEGRO_ALIGN_CENTRE, "%s", texto1);
    al_draw_bitmap(imagem1,390,130,0);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 140, ALLEGRO_ALIGN_CENTRE, "%s", texto2);
    al_draw_bitmap(imagem1,390,170,0);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 180, ALLEGRO_ALIGN_CENTRE, "%s", texto3);
    al_draw_bitmap(imagem1,390,210,0);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 220, ALLEGRO_ALIGN_CENTRE, "%s", texto4);
    al_draw_bitmap(imagem1,390,250,0);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 260, ALLEGRO_ALIGN_CENTRE, "%s", texto5);
    al_draw_bitmap(imagem1,390,290,0);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 300, ALLEGRO_ALIGN_CENTRE, "%s", texto6);
    al_draw_bitmap(imagem1,390,330,0);
    al_draw_textf(font1, al_map_rgb(0, 0, 0), 510, 340, ALLEGRO_ALIGN_CENTRE, "%s", texto7);
    al_flip_display();
}

void desenha_caixas(void)
{
    /* al_draw_filled_rectangle(450,150,450+200,150+40,al_map_rgb(205,205,205));
     al_draw_filled_rectangle(450,150+90,450+200,150+90+40,al_map_rgb(205,205,205));
     al_draw_filled_rectangle(450,150+180,450+200,150+180+40,al_map_rgb(205,205,205));
     al_draw_filled_rectangle(450,150+270,450+200,150+270+40,al_map_rgb(205,205,205));*/
}

int main()
{
    setlocale(LC_ALL,"");
    inicializar();

    int p=3,n;
    bool cancelar;
    int serie=0,quantidade=0;
    float resitencia=0,potencia=0/*,tolerancia=0*/;
    FILE *fp;
    FILE *fs;

    Serie*Arvore;
    Arvore = NULL;

    fp = fopen(NOME_ARQ2,"r");
    if (fp == NULL)
    {
        printf("Erro ao abrir arquivo %s.\n", NOME_ARQ2);
        return 0;
    }

    while(1)
    {
        n=fscanf(fp,"%d %f %f %d",&serie,&potencia,&resitencia,&quantidade);
        if (n==EOF)
            break;
        inserir(&Arvore,serie,potencia,resitencia,quantidade,1);
    }
    fclose(fp);
    system("cls");

    while(!doexit)
    {
        while(!doimprime)
        {
            Imprime_menu();

            doimprime=true;
        }
        fflush(stdin);

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (event_queue && ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            doexit = true;
        }
        if(ev.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                doimprime=false;
                break;

            case ALLEGRO_KEY_1:
            case ALLEGRO_KEY_PAD_1:
                j=100;
                al_clear_to_color(al_map_rgb(168,168,168));
                al_draw_textf(font1, al_map_rgb(0, 0, 0), 400, 550, ALLEGRO_ALIGN_CENTRE, "%s", texto8);
                al_draw_textf(font2, al_map_rgb(0, 0, 0), 430, 50, ALLEGRO_ALIGN_CENTRE, "%s", texto1);
                Busca_serie(Arvore,0,0,0,0);
                al_flip_display();
                while(1)
                {
                    al_wait_for_event(event_queue, &ev);
                    //if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
                    if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                        break;
                }
                doimprime=false;
                break;

            case ALLEGRO_KEY_2:
            case ALLEGRO_KEY_PAD_2:
                al_clear_to_color(al_map_rgb(168,168,168));
                al_draw_textf(font1, al_map_rgb(0, 0, 0), 400, 550, ALLEGRO_ALIGN_CENTRE, "%s", texto8);
                al_draw_textf(font2, al_map_rgb(0, 0, 0), 430, 50, ALLEGRO_ALIGN_CENTRE, "%s", texto2);
                //desenha_caixas();
                opera_resistor(Arvore,ev,1);
                //al_flip_display();
                doimprime=0;
                break;
            case ALLEGRO_KEY_3:
            case ALLEGRO_KEY_PAD_3:
                al_clear_to_color(al_map_rgb(168,168,168));
                al_draw_textf(font1, al_map_rgb(0, 0, 0), 400, 550, ALLEGRO_ALIGN_CENTRE, "%s", texto8);
                al_draw_textf(font2, al_map_rgb(0, 0, 0), 430, 50, ALLEGRO_ALIGN_CENTRE, "%s", texto3);
                desenha_caixas();
                al_flip_display();
                opera_resistor(Arvore,ev,3);
                doimprime=0;
                break;
            case ALLEGRO_KEY_4:
            case ALLEGRO_KEY_PAD_4:
                al_clear_to_color(al_map_rgb(168,168,168));
                al_draw_textf(font1, al_map_rgb(0, 0, 0), 400, 550, ALLEGRO_ALIGN_CENTRE, "%s", texto8);
                al_draw_textf(font2, al_map_rgb(0, 0, 0), 430, 50, ALLEGRO_ALIGN_CENTRE, "%s", texto4);
                desenha_caixas();
                al_flip_display();
                opera_resistor(Arvore,ev,2);
                doimprime=0;
                break;
            case ALLEGRO_KEY_5:
            case ALLEGRO_KEY_PAD_5:
                al_clear_to_color(al_map_rgb(168,168,168));
                al_draw_textf(font1, al_map_rgb(0, 0, 0), 400, 550, ALLEGRO_ALIGN_CENTRE, "%s", texto8);
                al_draw_textf(font2, al_map_rgb(0, 0, 0), 430, 50, ALLEGRO_ALIGN_CENTRE, "%s", texto5);
                desenha_caixas();
                al_flip_display();
                opera_resistor(Arvore,ev,0);
                doimprime=0;
                break;
            case ALLEGRO_KEY_6:
            case ALLEGRO_KEY_PAD_6:
                al_clear_to_color(al_map_rgb(168,168,168));
                system("cls");
                fs = fopen(NOME_ARQ2,"w+");
                Exporta_Arquivo(Arvore,fs);
                fclose(fs);
                al_draw_textf(font1, al_map_rgb(0, 0, 0), 400, 250, ALLEGRO_ALIGN_CENTRE, "*As alteracoes foram salvas*");
                al_flip_display();
                al_rest(1.3);
                return 0;
                break;
            case ALLEGRO_KEY_7:
            case ALLEGRO_KEY_PAD_7:
                printf("FIM!");
                doexit = true;
                break;
            }
        }
    }

    al_destroy_bitmap(img);
    al_destroy_display(display);
    return 0;
}
