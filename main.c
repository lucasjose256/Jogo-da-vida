#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#define BUFFER_SIZE 1024

#define ORG -2 // Equivalente ao 254 da tabela ASC estendida
#define VAZ -6 // Equivalente ao 250 da tabela ASC estendida
#define TAM 101


typedef struct tab
{
    char nomeJogo[TAM]; //o nome do jogo deve ser relativo ao padrao
    //de inicializacao. Por exemplo, JogoSapo ou JogoBloco
    int ciclosVida; // Define quanto cada jogo vai rodar (ciclos)
    int dim1,dim2; //dimensoes do tabuleiro linhas x colunas
    char **m; //Atenção! Essa matriz terá que ser alocada dinamicamente
    //para que a funcao que inicializa possa funcionar
}Tab;

char **alocaMatriz(int nL, int nC);

void desalocaMatriz(Tab tabuleiro);

void imprimeMatriz(Tab tabuleiro);

void copiaMatriz(Tab tabuleiro, char **aux);

void limpaMatriz(Tab tabuleiro);
///////////////////////////////////////////////////////////
void IniciaImagens (Tab tabuleiro, int xInic, int yInic);

void jogaJogoVida(Tab tabuleiro, int ciclo_atual);

void menuInicJogo(Tab tabuleiro);

void inicializaInvasao (Tab tabuleiro, int invasao);





//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////



/***************Funções feitas******************/
char **alocaMatriz(int nL, int nC)
{
    int i;
    char **matriz;

    matriz = (char **)malloc(nL*sizeof(int *));
    for (i = 0; i < nC; i++)
    {
      matriz[i] = (char *)malloc(nC*sizeof(int));
    }
    return matriz;
}

void desalocaMatriz(Tab tabuleiro)
{
    int i;
    for(i = 0; i < tabuleiro.dim1; i++)
        free(tabuleiro.m[i]);
    free(tabuleiro.m);
}

void imprimeMatriz(Tab tabuleiro)
{
    int i, j;

    for(i = 0;i < tabuleiro.dim1; i++)
    {
        printf("%c  ", 124);
        for(j = 0; j < tabuleiro.dim2; j++)
        {
            printf("%c  ", tabuleiro.m[i][j]);
        }
        printf("%c  ", 124);
        printf("\n");
    }
}

void copiaMatriz(Tab tabuleiro, char **aux)
{
    int i, j;
    for(i = 0; i < tabuleiro.dim1; i++)
    {
        for(j = 0; j < tabuleiro.dim2; j++)
            tabuleiro.m[i][j] = aux[i][j];
    }
}

/*******************************************/

void limpaMatriz(Tab tabuleiro)
{
    int i,j;
    for(i=0;i<tabuleiro.dim1;i++)
     for(j=0;j<tabuleiro.dim2;j++)
        tabuleiro.m[i][j]=VAZ;
}


void IniciaImagens (Tab tabuleiro, int xInic, int yInic)
{
    int linha_bloco, coluna_bloco; // Varáveis para inidcar quantas linhas e quantas colunas tem a matriz da imagem.
    int i,j; //percorre a matriz do bloco de imagem
    int **mat; //Matriz que receberá o bloco do arquivo, convertido para números inteiros
    char *tok;
    char buf [BUFFER_SIZE];

    FILE *fp;
    fp=fopen(tabuleiro.nomeJogo, "r");


    //Captam a primeira linha do arquivo, onde contém o número de linhas e colunas da imagem, separados por '-'
    fgets (buf, sizeof (buf), fp);
    tok = strtok (buf, "-");
    linha_bloco = atoi(tok);
    tok = strtok (NULL, "-");
    coluna_bloco = atoi(tok);


    /****Alocação das matrizes***/
    mat = (int**)alocaMatriz(linha_bloco, coluna_bloco);

    for(i=0;i<linha_bloco;i++)
    {
        fgets (buf, sizeof (buf), fp);
        tok = strtok (buf, ",");
        mat[i][0] = atoi(tok);

        for(j = 1;j < coluna_bloco; j++)
        {
            tok = strtok (NULL, ",");
            mat[i][j] = atoi(tok);
        }
    }

    limpaMatriz(tabuleiro);
    for(i=0;i<linha_bloco;i++)
    {

        for(j = 0;j < coluna_bloco; j++)
        {
            if(mat[i][j] >0)
                tabuleiro.m[xInic+i][yInic+mat[i][j]-1] = ORG;
        }
    }

}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void jogaJogoVida(Tab tabuleiro, int ciclo_atual)
{

    int viz;
    int i,j;
    Tab tabuleiro_aux;

    viz = 0;

    tabuleiro_aux.dim1 = tabuleiro.dim1;
    tabuleiro_aux.dim2 = tabuleiro.dim2;

    tabuleiro_aux.m = alocaMatriz(tabuleiro_aux.dim1, tabuleiro_aux.dim2); //matriz que devera ser usada para atualizar cada jogada

    limpaMatriz(tabuleiro_aux);

    for(i = 0; i < tabuleiro.dim1; i++)
    {
        for(j = 0; j< tabuleiro.dim2; j++)
        {
            if(i == 0) //Se estiver na primeira linha
            {
                if(j==0) //primeira linha e primeira coluna
                {
                    if(tabuleiro.m[i][j+1] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j+1] == ORG)
                        viz++;

                }
                else if(j == tabuleiro.dim2-1) //primeira linha e ultima coluna
                {
                    if(tabuleiro.m[i][j-1] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j-1] == ORG)
                        viz++;
                }
                else //primeira linha e colunas do meio
                {
                    if(tabuleiro.m[i][j+1] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j+1] == ORG)
                        viz++;
                    if(tabuleiro.m[i][j-1] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j-1] == ORG)
                        viz++;

                }
            }
            else if(i == tabuleiro.dim1-1) //Se estiver na última linha
            {
                if(j==0) //última linha e primeira coluna
                {
                    if(tabuleiro.m[i-1][j] == ORG)
                        viz++;
                    if(tabuleiro.m[i-1][j+1] == ORG)
                        viz++;
                    if(tabuleiro.m[i][j+1] == ORG)
                        viz++;

                }
                else if(j == tabuleiro.dim2-1) //última linha e ultima coluna
                {
                    if(tabuleiro.m[i][j-1] == ORG)
                        viz++;
                    if(tabuleiro.m[i-1][j-1] == ORG)
                        viz++;
                    if(tabuleiro.m[i-1][j] == ORG)
                        viz++;
                }
                else //última linha e colunas do meio
                {

                    if(tabuleiro.m[i-1][j] == ORG)
                        viz++;
                    if(tabuleiro.m[i-1][j+1] == ORG)
                        viz++;
                    if(tabuleiro.m[i][j+1] == ORG)
                        viz++;
                    if(tabuleiro.m[i][j-1] == ORG)
                        viz++;
                    if(tabuleiro.m[i-1][j-1] == ORG)
                        viz++;
                }
            }

            else
            {
                if(j==0) //primeira coluna
                {
                    if(tabuleiro.m[i-1][j] == ORG)
                        viz++;
                    if(tabuleiro.m[i-1][j+1] == ORG)
                        viz++;
                    if(tabuleiro.m[i][j+1] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j+1] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j] == ORG)
                        viz++;
                }
                else if(j == tabuleiro.dim2-1) //ultima coluna
                {

                    if(tabuleiro.m[i-1][j] == ORG)
                        viz++;
                    if(tabuleiro.m[i-1][j-1] == ORG)
                        viz++;
                    if(tabuleiro.m[i][j-1] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j-1] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j] == ORG)
                        viz++;
                }
                else //meio
                {
                    if(tabuleiro.m[i-1][j-1] == ORG)
                        viz++;
                    if(tabuleiro.m[i-1][j] == ORG)
                        viz++;
                    if(tabuleiro.m[i-1][j+1] == ORG)
                        viz++;
                    if(tabuleiro.m[i][j-1] == ORG )
                        viz++;
                    if(tabuleiro.m[i][j+1] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j-1] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j] == ORG)
                        viz++;
                    if(tabuleiro.m[i+1][j+1] == ORG)
                        viz++;
                }
            }
            if(((viz == 2 || viz == 3) && tabuleiro.m[i][j] == ORG) ||(viz == 3 && tabuleiro.m[i][j] == VAZ))
                tabuleiro_aux.m[i][j] = ORG;
            else
                tabuleiro_aux.m[i][j] = VAZ;
            viz = 0;

        }

    }
    copiaMatriz(tabuleiro,tabuleiro_aux.m); //implemente uma funcao que copia uma matriz em outra
    system("cls");
    printf("Ciclo %d\n", ciclo_atual);
    imprimeMatriz(tabuleiro_aux);
    Sleep(100);

    desalocaMatriz(tabuleiro_aux);

}

void menuInicJogo(Tab tabuleiro)
{
    int opcao, xInic, yInic;

    printf("(1)Bloco\n(2)Blinker\n(3)Sapo\n(4)Glider\n(5)LWSS\nEntre com a opcao: ");
    scanf("%d",&opcao);
    switch(opcao)
    {
     case 1:
     {
         strcpy(tabuleiro.nomeJogo,"./Bloco.csv");
         xInic=tabuleiro.dim1/2;
         yInic=tabuleiro.dim2/2;
         break;
     }
     case 2:
     {
         strcpy(tabuleiro.nomeJogo,"./Blinker.csv");
         xInic=tabuleiro.dim1/2;
         yInic=tabuleiro.dim2/2;
         break;
     }
     case 3:
     {
         strcpy(tabuleiro.nomeJogo,"./Sapo.csv");
         xInic=tabuleiro.dim1/2;
         yInic=tabuleiro.dim2/2;
         break;
     }
     case 4:
     {
         strcpy(tabuleiro.nomeJogo,"./Glider.csv");
         xInic=tabuleiro.dim1-4;
         yInic=tabuleiro.dim2-4;
         break;
     }
     case 5:
     {
         strcpy(tabuleiro.nomeJogo,"./LWSS.csv");
         xInic=tabuleiro.dim1-5;
         yInic=tabuleiro.dim2-6;
         break;
     }
    }
    IniciaImagens (tabuleiro, xInic, yInic);
    imprimeMatriz(tabuleiro);

    printf("Se inicializacao correta digite qualquer tecla para iniciar o jogo..."); while(getchar()!='\n'); getchar();

}


void inicializaInvasao (Tab tabuleiro, int invasao)
{
    int i,j;
    int xInic, yInic;
    int tipo_inicializacao; //variavel que recebe qual das imagens sera a inicializada
    char imagens[5][TAM] = {"Bloco", "Blinker", "Sapo", "Glider", "LWSS" };

    Tab tabuleiro_invasao;

    tabuleiro_invasao.dim1 = tabuleiro.dim1;
    tabuleiro_invasao.dim2 = tabuleiro.dim2;
    tabuleiro_invasao.m = alocaMatriz(tabuleiro_invasao.dim1, tabuleiro_invasao.dim2);

    srand(time(NULL));

    xInic = tabuleiro_invasao.dim1/2 - 3 + rand() % tabuleiro_invasao.dim1/4;
    yInic = tabuleiro_invasao.dim2/2 - 3 + rand() % tabuleiro_invasao.dim2/4;
    tipo_inicializacao = 1 + rand()%5; //vai de 1 a 5 no sorteio dos tipos de imagem

    printf("\nVoce sofreu a invasao de um %s na geracao %d!!\n", imagens[tipo_inicializacao-1], invasao);
    Sleep(1000);

    switch(tipo_inicializacao)
    {
     case 1:
     {
         strcpy(tabuleiro_invasao.nomeJogo,"./Bloco.csv");
         break;
     }
     case 2:
     {
         strcpy(tabuleiro_invasao.nomeJogo,"./Blinker.csv");
         break;
     }
     case 3:
     {
         strcpy(tabuleiro_invasao.nomeJogo,"./Sapo.csv");
         break;
     }
     case 4:
     {
         strcpy(tabuleiro_invasao.nomeJogo,"./Glider.csv");
         break;
     }
     case 5:
     {
         strcpy(tabuleiro_invasao.nomeJogo,"./LWSS.csv");
         break;
     }
    }



    IniciaImagens (tabuleiro_invasao, xInic, yInic); //cria uma matriz invasao

    for(i=0;i<tabuleiro.dim1;i++)
    {
        for(j=0;j<tabuleiro.dim2;j++)
        {
            if(tabuleiro_invasao.m[i][j] == ORG)
                tabuleiro.m[i][j] = ORG;
        }
    }
    imprimeMatriz(tabuleiro);

}


int main()
{
    int  n_ciclos; //Conta a quantidade de gerações já realizadas
    int invasao; //Armazena em qual geração será a invasão
    Tab tabuleiro;



    srand(time(NULL));

    /***********Se desejar, modificar aqui o tamanho da matriz do Tabuleiro********************/
    tabuleiro.dim1 = 20; //Quantidade de linhas da matriz do tabuleiro
    tabuleiro.dim2 = 20; //Quantidade de colunas da matriz do tabuleiro
    tabuleiro.m = alocaMatriz(tabuleiro.dim1, tabuleiro.dim2); //Aloca dinamicamente a matriz do Tabuleiro

    do
    {
        n_ciclos = 0; //Ainda está na geração zero
        printf("Digite a quantidade de ciclos que voce deseja: ");
        scanf("%d", &tabuleiro.ciclosVida);

        invasao = 1 + rand()% tabuleiro.ciclosVida-1; //A invasao pode ocorrer após a primeira geração até a penúltima


        menuInicJogo(tabuleiro); //Executa a função com o menu, onde a imagem de inicialização será escolhida


        do
        {
           n_ciclos++; //Incrementa a geração
           jogaJogoVida(tabuleiro, n_ciclos); //Executa o jogo

        }while (n_ciclos < invasao); //Enquanto não chegar na geração que ocorre a invasão


        inicializaInvasao (tabuleiro, invasao); //Executa a função da invasão, com o tabuleiro e a geração da invasão como parâmetros
        printf("Pressione qualquer tecla para continuar...");
        getchar();

        do
        {
           n_ciclos++; //Continua incrementando as gerações
           jogaJogoVida(tabuleiro,n_ciclos); //Executa o jogo

        }while(n_ciclos < tabuleiro.ciclosVida); //Enquanto não atingir o número de ciclos desejados

        printf("\nDeseja repetir o jogo(s/n)? ");

    }while(getchar() =='s'); //Enquanto a resposta do usuário for 's' para a pergunta acima
    desalocaMatriz(tabuleiro); //Libera o espaço alocado
}
