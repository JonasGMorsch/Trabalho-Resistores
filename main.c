#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

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
void inserir_resistencia(Resistencia**Arvore, float valor_resistencia ,int quantidade_resistores,int soma)
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

void Busca_resitencia(Resistencia*Arvore,int numero_serie,float potencia_resistor,float valor_resistencia ,int quantidade_resistores)
{
    if(Arvore != NULL)
    {
        Busca_resitencia(Arvore->esquerda,numero_serie,potencia_resistor,valor_resistencia,quantidade_resistores);
        if(((valor_resistencia == Arvore->valor_resistencia && quantidade_resistores ==0) || (quantidade_resistores==Arvore->quantidade_resistores && valor_resistencia ==0) || (valor_resistencia==0 &&quantidade_resistores ==0) || (valor_resistencia==Arvore->valor_resistencia && quantidade_resistores==Arvore->quantidade_resistores)) && (Arvore->quantidade_resistores>=0))
            printf("Série: E%d\tPotência:%5.2f\tResistencia:%9.1f\tQuantidade:%6d\n",numero_serie, potencia_resistor,Arvore->valor_resistencia,Arvore->quantidade_resistores);

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
/// ///////////////////////////////////////////////////INICIO BUSCA RESISTORES////////////////////////////////////////////////////

void adiciona_resistor(Serie*Arvore)
{
    int serie=0,quantidade=0,i=0;
    float resitencia=0,potencia=0,tolerancia=0,zeropontoum=0.1;
    char resposta;
    printf("\n Entre com os valores de: \n\n Resistência:");
    scanf("%f",&resitencia);
    printf(" Série:");
    scanf("%d",&serie);
    //printf(" Tolerância:");
   // scanf("%f",&tolerancia);
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
                inserir(&Arvore,serie,potencia,resitencia,quantidade,1);
            /// //////////
        }
        else
            printf(" Esta série não existe\n Este resistor não pode ser adicionado\n");
    }
    else
        printf(" A adição do resistor foi cancelada\n");
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
            apaga_resistor(Arvore);
            system("pause");
            system("cls");
            break;
        case 5:
            retira_resistor(Arvore);
            system("pause");
            system("cls");
            break;
        case 6:
            system("cls");
            fs = fopen(NOME_ARQ2,"w+");
            Exporta_Arquivo(Arvore,fs);
            fclose(fs);
            printf("*As alterações foram salvas*\n");
            return 0;
        case 8:
            printf("FIM!");
            return 0;
        }
    }
    return 0;
}
