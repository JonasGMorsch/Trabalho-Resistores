#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

const int SCREEN_W = 800;
const int SCREEN_H = 600;

struct cabecalho
{
    struct lista* iniciodecabecalho;
    int nelementos;
    struct lista* fimdecabecalho;
};
typedef struct cabecalho Estruturadocabecalho;

struct lista
{
    struct lista* listaanterior;
    struct resistores* ponteirodedados;
    struct lista* listaprox;
};
typedef struct lista Lista;

struct resistores
{
    float resistencia;
    int serie;
    float tolerancia;
    float potencia;
    int quantidade;
};
typedef struct resistores Resistores;

void inic_cabecalho(Estruturadocabecalho* Cabecalho)
{
    Cabecalho->nelementos = 0;  //inicializando
    Cabecalho->iniciodecabecalho = NULL;   //inicializando
    Cabecalho->fimdecabecalho = NULL;      //inicializando
}

void inserefim (Estruturadocabecalho* Cabecalho,float r,int s,float t,float w,int q)
{
    Lista* ListaN = (Lista*) malloc(sizeof(Lista)); //lista dupla
    Resistores* ResistoresN = (Resistores*) malloc(sizeof(Resistores));  //banco de bados

    ListaN->ponteirodedados = ResistoresN;

    ListaN->ponteirodedados->resistencia=r;
    ListaN->ponteirodedados->serie=s;
    ListaN->ponteirodedados->tolerancia=t;
    ListaN->ponteirodedados->potencia=w;
    ListaN->ponteirodedados->quantidade= q;

    if (Cabecalho->iniciodecabecalho == NULL)
    {
        Cabecalho->iniciodecabecalho = ListaN;
        ListaN->listaanterior = NULL;
    }
    else
    {
        Cabecalho->fimdecabecalho->listaprox=ListaN;
        ListaN->listaanterior = Cabecalho->fimdecabecalho;
    }
    Cabecalho->fimdecabecalho = ListaN;
    ListaN->listaprox = NULL;
    Cabecalho->nelementos++; // o numero de elemenos cresceu
}

void insere_numero (int v,Estruturadocabecalho* Cabecalho,int r,int s,int w,int q)
{
    Lista* ListaN = (Lista*) malloc(sizeof(Lista)); //criacao da lista
    Resistores* ResistoresN = (Resistores*) malloc(sizeof(Resistores));  //criacao do banco de bados

    ListaN->ponteirodedados = ResistoresN;   //ListaN agora esta ligada à ResistoresN

    ListaN->ponteirodedados->resistencia=r;
    ListaN->ponteirodedados->serie=s;
    ListaN->ponteirodedados->potencia=w;
    ListaN->ponteirodedados->quantidade= q;

    v--;
    if (Cabecalho->iniciodecabecalho == NULL)
    {
        Cabecalho->iniciodecabecalho = ListaN;
        Cabecalho->fimdecabecalho  = ListaN;
        ListaN->listaanterior = NULL;
        ListaN->listaprox = NULL;
    }
    else
    {
        Lista* ListaAux;
        Lista* ListaAux2;
        ListaAux=Cabecalho->iniciodecabecalho;

        if (v==0)
        {
            ListaN->listaanterior =NULL;
            ListaN->listaprox = Cabecalho->iniciodecabecalho;
            Cabecalho->iniciodecabecalho=ListaN;
        }
        else if (v==Cabecalho->nelementos)
        {
            ListaN->listaanterior = Cabecalho->fimdecabecalho;
            Cabecalho->fimdecabecalho->listaprox=ListaN;
            Cabecalho->fimdecabecalho = ListaN;
            ListaN->listaprox = NULL;
        }
        else if (v>Cabecalho->nelementos || v<0)
            printf("Numero invalido \n");
        else
        {
            int i;
            for (i=1; i<v; i++)
                ListaAux=ListaAux->listaprox;

            ListaAux2=ListaAux->listaprox;
            ListaAux->listaprox=ListaN;
            ListaN->listaanterior=ListaAux;
            ListaAux2->listaanterior=ListaN;
            ListaN->listaprox=ListaAux2;
        }
    }
    Cabecalho->nelementos++; // o numero de elemenos cresceu
}
void imprime_todos (Estruturadocabecalho* Cabecalho)
{
    Lista* ListaAux;
    for (ListaAux=Cabecalho->iniciodecabecalho; ListaAux != NULL; ListaAux = ListaAux->listaprox)
        printf("Resis.= %8.1f  Serie.= E%.2d\tToler. = %5.2f Poten.= %5.2f  Quant.= %.4d  \n", ListaAux->ponteirodedados->resistencia, ListaAux->ponteirodedados->serie,ListaAux->ponteirodedados->tolerancia, ListaAux->ponteirodedados->potencia, ListaAux->ponteirodedados->quantidade);
}

void imprime_numero (int v, Estruturadocabecalho* Cabecalho)
{
    Lista* ListaAux;
    ListaAux=Cabecalho->iniciodecabecalho;
    if (v>Cabecalho->nelementos || v<=0)
        printf("Numero invalido \n");
    else if (v==Cabecalho->nelementos)
        printf("info = %d\n",Cabecalho->fimdecabecalho->ponteirodedados->quantidade);
    else
    {
        int i;
        for (i=1; i<v; i++)
            ListaAux=ListaAux->listaprox;
        printf("Resis.= %8.1f  Serie.= E%.2d Toler. = %5.2f Poten.= %5.2f  Quant.= %.4d  \n", ListaAux->ponteirodedados->resistencia, ListaAux->ponteirodedados->serie, ListaAux->ponteirodedados->tolerancia, ListaAux->ponteirodedados->potencia, ListaAux->ponteirodedados->quantidade);
    }
}
void ordernar (Estruturadocabecalho* Cabecalho)
{
    struct resistores* pdados_aux;
    int i,j,k;
    int z=0;
    Lista* ListaAux;
    for (z=0 ; z <= 2 ; z++)
    {
        for(i=Cabecalho->nelementos; i > 1; i--)
            for(j=0; j < i ; j++)
            {
                ListaAux=Cabecalho->iniciodecabecalho;
                for (k=1; k<j; k++)
                    ListaAux=ListaAux->listaprox;

                if((ListaAux->ponteirodedados->resistencia>ListaAux->listaprox->ponteirodedados->resistencia && ListaAux->ponteirodedados->serie==ListaAux->listaprox->ponteirodedados->serie && ListaAux->ponteirodedados->potencia==ListaAux->listaprox->ponteirodedados->potencia) || z==0 || z==1)
                    if((ListaAux->ponteirodedados->potencia>ListaAux->listaprox->ponteirodedados->potencia && ListaAux->ponteirodedados->serie==ListaAux->listaprox->ponteirodedados->serie) || z==0 || z==2)
                        if(ListaAux->ponteirodedados->serie>ListaAux->listaprox->ponteirodedados->serie || z==1 || z==2)
                        {
                            pdados_aux = ListaAux->ponteirodedados;
                            ListaAux->ponteirodedados = ListaAux->listaprox->ponteirodedados;
                            ListaAux->listaprox->ponteirodedados = pdados_aux;
                        }
            }
    }
}

void adiciona_resistor(Estruturadocabecalho* Cabecalho)
{
    Lista* ListaAux;
    ListaAux=Cabecalho->iniciodecabecalho;
    int i=0,serie=0,quantidade=0;
    float resitencia=0,potencia=0,tolerancia=0,zeropontoum=0.1;
    char resposta;

    printf("\n Entre com os valores de: \n\n Resistência:");
    scanf("%f",&resitencia);
    printf(" Série:");
    scanf("%d",&serie);
    printf(" Tolerância:");
    scanf("%f",&tolerancia);
    printf(" Potência:");
    scanf("%f",&potencia);
    printf(" Quantidade:");
    scanf("%d",&quantidade);
    fflush(stdin);
    printf("\n Os Valores estão corretos? (S/N) \n Resis.= %8.1f  Serie.= E%.2d Toler. = %5.2f Poten.= %5.2f  Quant.= %.4d  \n",resitencia,serie,tolerancia,potencia,quantidade);
    scanf("%c",&resposta);
    if (resposta=='S' || resposta=='s')
    {
        if (serie==0 || serie==6 || serie==12 || serie==24 || serie==48 || serie==96 || serie==192)
        {
            if(serie==0 && tolerancia==0)
            {
                printf(" Informe pelo menos um dos itens abaixo:\n Série \n Tolerância \n");
                ListaAux=NULL;
                i++;
            }

            if(serie==192 && (!( tolerancia== 0.50 || tolerancia== 0.25 || tolerancia==zeropontoum )))
            {
                printf(" Para resistores da série E192 é obrigatório informar a tolerância \n");
                while(1)
                {
                    fflush(stdin);
                    printf(" Tolerância:");
                    scanf("%f",&tolerancia);

                    if (tolerancia==0.5 || tolerancia==0.25 || tolerancia==zeropontoum)
                        break;
                    else
                        printf(" A tolerância informada é inválida, tente novamente\n");
                }
            }

            if (serie==24)
                tolerancia=5;
            else if (serie==12)
                tolerancia=10;
            else if (serie==48)
                tolerancia=2;
            else if (serie==6)
                tolerancia=20;
            else if (serie==96)
                tolerancia=1;

            if (tolerancia==20)
                serie=6;
            else if (tolerancia==10)
                serie=12;
            else if (tolerancia==5)
                serie=24;
            else if (tolerancia==2)
                serie=48;
            else if (tolerancia==1)
                serie=96;


            while(ListaAux!=NULL)
            {
                if (ListaAux->ponteirodedados->resistencia == resitencia && ListaAux->ponteirodedados->serie == serie && ListaAux->ponteirodedados->tolerancia == tolerancia && ListaAux->ponteirodedados->potencia == potencia)
                {
                    fflush(stdin);
                    printf(" Este Resistor já está na lista, deseja adicionar a quantidade digitada? (S/N)\n");
                    scanf("%c",&resposta);
                    if (resposta=='S' || resposta=='s')
                    {
                        ListaAux->ponteirodedados->quantidade=ListaAux->ponteirodedados->quantidade+quantidade;
                        printf(" A adição do resistor foi bem sucedida\n");
                    }
                    else
                        printf(" A adição do resistor foi cancelada\n");
                    i++;
                }
                ListaAux=ListaAux->listaprox;
            }
            if(i==0)
            {
                Lista* ListaN = (Lista*) malloc(sizeof(Lista)); //lista dupla
                Resistores* ResistoresN = (Resistores*) malloc(sizeof(Resistores));  //banco de bados

                ListaN->ponteirodedados = ResistoresN;

                ListaN->ponteirodedados->resistencia=resitencia;
                ListaN->ponteirodedados->serie=serie;
                ListaN->ponteirodedados->tolerancia=tolerancia;
                ListaN->ponteirodedados->potencia=potencia;
                ListaN->ponteirodedados->quantidade= quantidade;

                if (Cabecalho->iniciodecabecalho == NULL)
                {
                    Cabecalho->iniciodecabecalho = ListaN;
                    ListaN->listaanterior = NULL;
                }
                else
                {
                    Cabecalho->fimdecabecalho->listaprox=ListaN;
                    ListaN->listaanterior = Cabecalho->fimdecabecalho;
                }
                Cabecalho->fimdecabecalho = ListaN;
                ListaN->listaprox = NULL;
                Cabecalho->nelementos++;
                printf(" A adição do resistor foi bem sucedida\n");
            }
        }
        else
            printf(" Esta série não existe\n Este resistor não pode ser adicionado\n");
    }
    else
        printf(" A adição do resistor foi cancelada\n");
}

void busca_resistor(Estruturadocabecalho* Cabecalho)
{
    Lista* ListaAux;
    ListaAux=Cabecalho->iniciodecabecalho;
    int i=0,serie=0,quantidade=0;
    float resitencia=0,potencia=0;
    char sa[10];
    char sb[10];
    char sc[10];
    char sd[10];

    fflush(stdin);
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
    quantidade = atoi(sd);

    while(ListaAux!=NULL)
    {
        if ((ListaAux->ponteirodedados->resistencia == resitencia || resitencia==0) && (ListaAux->ponteirodedados->serie == serie || serie==0) && (ListaAux->ponteirodedados->potencia == potencia || potencia==0) && (ListaAux->ponteirodedados->quantidade == quantidade || quantidade==0))
        {
            printf("Resis.= %8.1f  Serie.= E%.2d  Poten.= %5.2f  Quant.= %.4d  \n", ListaAux->ponteirodedados->resistencia, ListaAux->ponteirodedados->serie, ListaAux->ponteirodedados->potencia, ListaAux->ponteirodedados->quantidade);
            i++;
        }
        ListaAux=ListaAux->listaprox;
    }
    if(i==0)
        printf("\n *Nenhum resultado foi encontrado*\n");
}

void apaga_resistor(Estruturadocabecalho* Cabecalho)
{
    Lista* ListaAux;
    ListaAux=Cabecalho->iniciodecabecalho;
    int i=0,serie=0;
    float resitencia=0,potencia=0;
    char sa[10];
    char sb[10];
    char sc[10];

    fflush(stdin);
    printf("\n Entre com os valores conhecidos de: \n\n Resistência:");
    fgets(sa, sizeof(sa), stdin);
    resitencia = atof(sa);
    printf(" Série:");
    fgets(sb, sizeof(sb), stdin);
    serie = atoi(sb);
    printf(" Potência:");
    fgets(sc, sizeof(sc), stdin);
    potencia = atof(sc);
    while(ListaAux!=NULL)
    {
        if (ListaAux->ponteirodedados->resistencia == resitencia && ListaAux->ponteirodedados->serie == serie && ListaAux->ponteirodedados->potencia == potencia)
        {
            if (ListaAux->listaanterior==NULL)
            {
                Cabecalho->iniciodecabecalho=ListaAux->listaprox;
                Cabecalho->iniciodecabecalho->listaanterior=NULL;
            }
            else
                ListaAux->listaanterior->listaprox=ListaAux->listaprox;

            if (ListaAux->listaprox==NULL)
                Cabecalho->fimdecabecalho=ListaAux->listaanterior;
            else
                ListaAux->listaprox->listaanterior=ListaAux->listaanterior;
            Cabecalho->nelementos--;
            free(ListaAux);
            i++;
        }
        ListaAux=ListaAux->listaprox;
    }
    if(i==0)
        printf("\n *Nenhum item foi encontrado/deletado*\n");
    else
        printf("\n *O resistor foi apagado com sucesso*\n");
}

void retira_resistor(Estruturadocabecalho* Cabecalho)
{
    Lista* ListaAux;
    ListaAux=Cabecalho->iniciodecabecalho;
    int i=0,serie=0,quantidade=0;
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
    {
        while(ListaAux!=NULL)
        {
            if (ListaAux->ponteirodedados->resistencia == resitencia && ListaAux->ponteirodedados->serie == serie && ListaAux->ponteirodedados->potencia == potencia)
            {
                ListaAux->ponteirodedados->quantidade=ListaAux->ponteirodedados->quantidade-quantidade;
                i++;
                printf("\n *A quantidade de resistores foi retirada com sucesso*\n");
            }
            ListaAux=ListaAux->listaprox;
        }
        if(i==0)
            printf("\n *Nenhum item foi encontrado/retirado*\n");
    }
    else
        printf("\n *A retirada de resistores foi cancelada*\n");
}

const char NOME_ARQ[] = "resistores.txt";
int main()
{
//////////////////ALLEGRO ////////////////////////////////////////////////////////////////////
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_FONT *font1 = NULL;
    ALLEGRO_FONT *font2 = NULL;
    ALLEGRO_BITMAP  *img   = NULL;
    ALLEGRO_BITMAP  *imagem1  = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;

    al_init();

    display = al_create_display(SCREEN_W, SCREEN_H);
    al_clear_to_color(al_map_rgb(168,168,168));
    al_set_window_title(display, "Base de dados de Resistores");
    al_init_image_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon();// initialize the ttf (True Type Font) addon
    al_install_mouse();
    al_install_keyboard();
    img = al_load_bitmap("res2.png");
    imagem1 = al_load_bitmap("fundo.png");
    font1 = al_load_ttf_font("pirulen.TTF",15,0 );
    font2 = al_load_ttf_font("pirulen.TTF",20,0 );
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
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

    setlocale(LC_CTYPE,"portuguese");
    int p=3,n;
    int serie=0,quantidade=0;
    float resitencia=0,potencia=0,tolerancia=0;
    Lista* ListaAux;
    FILE *fp;
    fp = fopen(NOME_ARQ,"r");
    if (fp == NULL)
    {
        printf("Erro ao abrir arquivo %s.\n", NOME_ARQ);
        exit(1);
    }
    Estruturadocabecalho* Cabecalho = (Estruturadocabecalho*) malloc(sizeof(Estruturadocabecalho));   //criando cabecalho
    inic_cabecalho(Cabecalho);
    while(1)
    {
        n=fscanf(fp,"%f %d %f %f %d",&resitencia,&serie,&tolerancia,&potencia,&quantidade);
        if (n==EOF)
            break;
        inserefim(Cabecalho,resitencia,serie,tolerancia,potencia,quantidade);
    }
    ordernar(Cabecalho);

    while(p!=7)
    {
        //printf("aqui no while");
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            printf("aqui");
            if (ev.mouse.x >= al_get_bitmap_width(imagem1) && ev.mouse.x <= SCREEN_W -10 && ev.mouse.y <= SCREEN_H - 10 && ev.mouse.y >= al_get_bitmap_height(imagem1) - 10)
            {
                p = 1;
                printf("aqui2");
            }
        }
        /*
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            if (ev.mouse.x >= 520 && ev.mouse.x <= 700 && ev.mouse.y <= 100 && ev.mouse.y >= 110)
            {
                p = 2;

            }
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            if (ev.mouse.x >= 520 && ev.mouse.x <= 700 && ev.mouse.y <= 100 && ev.mouse.y >= 110)
            {
                p = 3;

            }
        }

        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            if (ev.mouse.x >= 520 && ev.mouse.x <= 700 && ev.mouse.y <= 100 && ev.mouse.y >= 110)
            {
                p = 4;

            }
        }

        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            if (ev.mouse.x >= 520 && ev.mouse.x <= 700 && ev.mouse.y <= 100 && ev.mouse.y >= 110)
            {
                p = 5;

            }
        }

        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            if (ev.mouse.x >= 520 && ev.mouse.x <= 700 && ev.mouse.y <= 100 && ev.mouse.y >= 110)
            {
                p = 6;

            }
        }

        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            if (ev.mouse.x >= 520 && ev.mouse.x <= 700 && ev.mouse.y <= 100 && ev.mouse.y >= 110)
            {
                p = 7;

            }
        }
*/

        fflush(stdin);
        system("cls");
        switch (p)
        {
        case 1:
            ordernar(Cabecalho);
            imprime_todos(Cabecalho);
            system("pause");
            system("cls");
            break;
        case 2:
            adiciona_resistor(Cabecalho);
            system("pause");
            system("cls");
            break;
        case 3:
            busca_resistor(Cabecalho);
            system("pause");
            system("cls");
            break;
        case 4:
            apaga_resistor(Cabecalho);
            system("pause");
            system("cls");
            break;
        case 5:
            retira_resistor(Cabecalho);
            system("pause");
            system("cls");
            break;
        case 6:
            fp = fopen(NOME_ARQ,"w");
            for (ListaAux=Cabecalho->iniciodecabecalho; ListaAux != NULL; ListaAux = ListaAux->listaprox)
                fprintf(fp,"%8.1f\t%.2d\t%5.2f\t%5.2f\t%.4d \n", ListaAux->ponteirodedados->resistencia, ListaAux->ponteirodedados->serie,ListaAux->ponteirodedados->tolerancia, ListaAux->ponteirodedados->potencia, ListaAux->ponteirodedados->quantidade);
            //system("pause");
            system("cls");
            printf("*As alterações foram salvas*\n");
            return 0;
            break;
        case 7:
            printf("FIM!");
            break;
        }

    }


    al_destroy_bitmap(img);
    al_destroy_display(display);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return 0;
}
