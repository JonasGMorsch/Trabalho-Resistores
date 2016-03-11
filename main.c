#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

struct resistores
{
    float resistencia;
    int serie;
    float tolerancia;
    float potencia;
    int quantidade;
};
typedef struct resistores Resistores;

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

struct lista
{
    struct lista* listaanterior;
    struct resistores* ponteirodedados;
    struct lista* listaprox;
};
typedef struct lista Lista;

void inic_cabecalho(Estruturadocabecalho* Cabecalho)
{
    Cabecalho->nelementos = 0;  //inicializando
    Cabecalho->iniciodecabecalho = NULL;   //inicializando
    Cabecalho->fimdecabecalho = NULL;      //inicializando
}

void inserir_resistencia(Resistencia**Arvore, float valor_resistencia ,int quantidade_resistores,int soma)
{
    if(*Arvore == NULL)
    {
        Resistencia* ResistenciaN = (Resistencia*) malloc(sizeof(Resistencia));
        ResistenciaN->esquerda = NULL;
        ResistenciaN->direita = NULL;
        ResistenciaN->valor_resistencia = valor_resistencia;
        ResistenciaN->quantidade_resistores=quantidade_resistores;
        *Arvore=ResistenciaN;
    }
    else
    {
        if(valor_resistencia < (*Arvore)->valor_resistencia)
            inserir_resistencia(&(*Arvore)->esquerda,valor_resistencia,quantidade_resistores,soma);

        if(valor_resistencia > (*Arvore)->valor_resistencia)
            inserir_resistencia(&(*Arvore)->direita, valor_resistencia,quantidade_resistores,soma);

        if(soma==1)
            if(valor_resistencia == (*Arvore)->valor_resistencia)
                (*Arvore)->quantidade_resistores+=quantidade_resistores;

         if(soma==0)
            if(valor_resistencia == (*Arvore)->valor_resistencia)
                (*Arvore)->quantidade_resistores-=quantidade_resistores;
    }
}

void inserir_potencia(Potencia**Arvore, float potencia_resistor, float valor_resistencia ,int quantidade_resistores,int soma)
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

void inserir(Serie**Arvore, int numero_serie,float potencia_resistor,float valor_resistencia ,int quantidade_resistores,int soma)
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


/// //////////////////////////////INICIO EXPORTA TXT /////////////////////////////////
void Exporta_Arquivo_3(Resistencia*Arvore,int numero_serie,float potencia_resistor,FILE * fp)
{
    if(Arvore != NULL)
    {
        Exporta_Arquivo_3(Arvore->esquerda,numero_serie,potencia_resistor,fp);
        printf(" Série: E%d\tPotência:%5.2f\tResistencia:%9.1f\tQuantidade:%6.d\n",numero_serie, potencia_resistor,Arvore->valor_resistencia,Arvore->quantidade_resistores);
        fprintf(fp,"%d\t %5.2f %12.1f %6.d\n",numero_serie, potencia_resistor, Arvore->valor_resistencia, Arvore->quantidade_resistores);
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
/// //////////////////////////////FIM EXPORTA TXT /////////////////////////////////

void Busca_resitencia(Resistencia*Arvore,int numero_serie,float potencia_resistor,float valor_resistencia ,int quantidade_resistores)
{
    if(Arvore != NULL)
    {
        Busca_resitencia(Arvore->esquerda,numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores);
        if((valor_resistencia == Arvore->valor_resistencia && quantidade_resistores ==0) || (quantidade_resistores==Arvore->quantidade_resistores && valor_resistencia ==0) || (valor_resistencia==0 &&quantidade_resistores ==0) || (valor_resistencia==Arvore->valor_resistencia && quantidade_resistores==Arvore->quantidade_resistores))
            printf("Série: E%d\tPotência:%5.2f\tResistencia:%9.1f\tQuantidade:%6.d\n",numero_serie, potencia_resistor,Arvore->valor_resistencia,Arvore->quantidade_resistores);

        Busca_resitencia(Arvore->direita,numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores);
    }
}

void Busca_potencia(Potencia*Arvore,int numero_serie,float potencia_resistor,float valor_resistencia ,int quantidade_resistores)
{
    if(Arvore != NULL)
    {
        Busca_potencia(Arvore->esquerda,numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores);
        if(potencia_resistor == Arvore->potencia_resistor || potencia_resistor ==0)
            Busca_resitencia(Arvore->resistencia,numero_serie, Arvore->potencia_resistor,valor_resistencia,quantidade_resistores);
        Busca_potencia(Arvore->direita,numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores);
    }
}

void Busca_serie(Serie*Arvore, int numero_serie,float potencia_resistor,float valor_resistencia ,int quantidade_resistores)
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

void adiciona_resistor(Serie*Arvore)
{
    int serie=0,quantidade=0,i=0;
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
            }

            if(serie==192 && (!( tolerancia== 0.50 || tolerancia== 0.25 || tolerancia==zeropontoum )))
            {
                if(serie==0 && tolerancia==0)
                {
                    printf(" Informe pelo menos um dos itens abaixo:\n Série \n Tolerância \n");
                    i++;
                }
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


            ///   ////////
            if(i==0)
                inserir(&Arvore,serie,potencia,resitencia,quantidade,0);


            /// //////////


        }
        else
            printf(" Esta série não existe\n Este resistor não pode ser adicionado\n");
    }
    else
        printf(" A adição do resistor foi cancelada\n");
}

void busca_resistor(Serie*Arvore)
{
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

    Busca_serie(Arvore,serie,potencia,resitencia,quantidade);

}

const char NOME_ARQ[] = "resistores.txt";
const char NOME_ARQ2[] = "resistores_arvore.txt";
int main()
{
    setlocale(LC_CTYPE,"portuguese");
    int p=3,n;
    int serie=0,quantidade=0;
    float resitencia=0,potencia=0,tolerancia=0;
    //Lista* ListaAux;
    FILE *fp;


    Serie*Arvore;
    Arvore = NULL;

    fp = fopen(NOME_ARQ,"r");
    if (fp == NULL)
    {
        printf("Erro ao abrir arquivo %s.\n", NOME_ARQ);
        exit(1);
    }

    while(1)
    {
        n=fscanf(fp,"%f %d %f %f %d",&resitencia,&serie,&tolerancia,&potencia,&quantidade);
        //n=fscanf(fp,"%d %f %f %d",&serie,&potencia,&resitencia,&quantidade);
        if (n==EOF)
            break;
        inserir(&Arvore,serie,potencia,resitencia,quantidade,0);
    }

    FILE *fs;


    while(p!=8)
    {
        printf(" Base de dados de resistores:\n\n <1> Consultar Resistores\n <2> Adicionar Resistores\n <3> Buscar Resistores\n <4> Apagar Resistores \n <5> Retirar Resistores \n <6> Salvar e Sair\n <8> Sair\n\n");
        scanf("%d",&p);
        fflush(stdin);
        system("cls");
        switch (p)
        {
        case 1:
            Busca_serie(Arvore,0,0,0,0);
            system("pause");
            system("cls");
            break;
        case 2:
            adiciona_resistor(Arvore);
            system("pause");
            system("cls");
            break;
        case 3:
            busca_resistor(Arvore);
            system("pause");
            system("cls");
            break;
        case 4:
            ///apaga_resistor(Cabecalho);
            system("pause");
            system("cls");
            break;
        case 5:
            ///retira_resistor(Cabecalho);
            system("pause");
            system("cls");
            break;
        case 6:
            system("cls");
            fs = fopen(NOME_ARQ2,"w+");
            Exporta_Arquivo(Arvore,fs);
            fclose(fs);
            //system("pause");
            //system("cls");
            printf("*As alterações foram salvas*\n");
            return 0;
        case 8:
            printf("FIM!");
            break;
        }
    }
    return 0;
}
