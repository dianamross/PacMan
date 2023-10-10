#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//define para elementos que mudam constantemente no mapa:
#define PACMAN '>'                                          //define para qual caractere no mapa eh o pacMan;
#define FANTASMA_ESQUERDA 'B'                               //define para qual caractere no mapa eh o fantasma que tem o sentido inicial para a esquerda;
#define FANTASMA_DIREITA 'C'                                //define para qual caractere no mapa eh o fantasma que tem o sentido inicial para a direita;
#define FANTASMA_BAIXO 'I'                                  //define para qual caractere no mapa eh o fantasma que tem o sentido inicial para baixo;
#define FANTASMA_CIMA 'P'                                   //define para qual caractere no mapa eh o fantasma que tem o sentido inicial para cima;
#define PACMAN_MODIFICADO '&'                               //define para qual caractere no mapa eh o pacMan modificado;

//define para qual sentido os fantasmas estao indo:
#define SENTIDO_ESQUERDA 'e'
#define SENTIDO_DIREITA 'd'
#define SENTIDO_CIMA 'c'
#define SENTIDO_BAIXO 'b'

//define para elementos do mapa:
#define PAREDE '#'                                          //define para qual caractere no mapa eh parede;
#define TELEPORTE '@'                                       //define para qual caractere no mapa eh o teleporte;
#define COMIDA '*'                                          //define para qual caractere no mapa eh a comida;
#define VAZIO ' '                                           //define para qual caractere no mapa eh o vazio;
#define COMIDA_ESPECIAL '%'                                 //define para qual caractere no mapa eh uma comida especial (que da 5 pontos);
#define COMIDA_MOD_PACMAN '!'                               //define para qual caractere no mapa eh uma comida que modifica o pacMan;

//define para as jogadas que movimentam o pacMan no mapa:
#define CIMA 'w'                                            //define para qual caractere faz o pacMan se mover para cima;
#define BAIXO 's'                                           //define para qual caractere faz o pacMan se mover para baixo;    
#define ESQUERDA 'a'                                        //define para qual caractere faz o pacMan se mover para a esquerda;
#define DIREITA 'd'                                         //define para qual caractere faz o pacMan se mover para a direita;


typedef struct {                            //STRUCT DA POSICAO:
    int linha;                              //contem a linha dessa posicao;
    int coluna;                             //contem a coluna dessa posicao;
} tPosicao;


typedef struct {                            //STRUCT DO FANTASMA:
    tPosicao posicaoFantasma;               //contem a struct da posicao do fantasma;    
    char sentido;                           //contem o sentido que o fantasma esta direcionado;
    int existe;                             //contem a verificacao da existencia do fantasma;
} tFantasma;


typedef struct {                            //STRUCT DOS FANTASMAS:
    tFantasma fantasmaB;                    //contem a struct do fantasma B;
    tFantasma fantasmaP;                    //contem a struct do fantasma P;    
    tFantasma fantasmaI;                    //contem a struct do fantasma I;
    tFantasma fantasmaC;                    //contem a struct do fantasma C;    
} tFantasmas;


typedef struct {                            //STRUCT DO PACMAN:
    int pontuacao;                          //contem a pontuacao do jogador, ou seja, quantas comidas o pacMan consumiu;        
    tPosicao posicaoPacMan;                 //contem a struct da posicao do pacMan no mapa;
    int colidiuParede[5000];                //contem o vetor de verificacao de quais jogadas o pacMan colidiu com a parede;
    int usouTeleporte[5000];                //contem o vetor de verificacao de quais jogadas o pacMan usou o teleporte;
    int pegouComida[5000];                  //contem o vetor de verificacao de quais jogadas o pacMan consumiu uma comida;
    int colidiuFantasma[5000];              //contem o vetor de verificacao de quais jogadas o pacMan colidiu com um fantasma;
    int vida;                               //contem a verificacao da vida do pacMan;
    int pegouMod;                           //contem a verificacao se o pacMan esta modificado;
} tPacMan;


typedef struct {                            //STRUCT DE JOGADA:             
    int qtd;                                //contem a quantidade que esse movimento foi usado;
    int comeu;                              //contem a quantidade de comidas consumidas por cada movimento;
    int bateu;                              //contem as vezes que o pacMan colidiu com a parede com esse movimento;
} tMovimento;


typedef struct {                            //STRUCT DAS JOGADAS:    
    int qtdJogadas;                         //contem o total de jogadas da partida;
    tMovimento movimentoW;                  //contem uma struct para cada jogada;
    tMovimento movimentoA;
    tMovimento movimentoS;
    tMovimento movimentoD;   
} tMovimentos;


typedef struct {                            //STRUCT DOS TELEPORTES:
    tPosicao posicaoTeleporte1;             //contem a struct de posicao do primeiro teleporte encontrado;
    int achouTeleporte1;                    //contem a verificacao de que o primeiro teleporte foi encontrado;
    tPosicao posicaoTeleporte2;             //contem a struct de posicao do segundo teleporte encontrado;
    int achouTeleporte2;                    //contem a verificacao de que o segundo teleporte foi encontrado;
} tTeleportes;


typedef struct {                            //STRUCT DOS MAPAS:
    char mapaGeral[41][101];                //contem o mapa geral;           
    char mapaComidas[41][101];              //contem o mapa das comidas;
    char mapaTeleportes[41][101];           //contem o mapa dos teleportes;
    int trilha[41][101];                    //contem o mapa da trilha;
    char mapaComidaEspecial[41][101];       //contem o mapa da comida especial;
    char mapaComidaMod[41][101];            //contem o mapa da comida que modifica o pacMan;
} tMapa;


typedef struct {                            //STRUCT DO JOGO:
    tPacMan pacMan;                         //contem a struct do pacMan;
    tFantasmas fantasmas;                   //contem a struct dos fantasmas;
    int linha;                              //contem a quantidade de linhas e colunas dos mapas;
    int coluna;
    int qtdComidas;                         //contem a quantidade de comidas no mapa;
    tMapa mapas;                            //contem a struct dos mapas;                      
    int limiteJogadas;                      //contem o limite de jogadas;
    tTeleportes teleporte;                  //contem a struct de teleportes;         
    tMovimentos movimentos;                 //contem a struct das jogadas feitas;
    tPosicao comidaEspecial;                //contem a struct da posicao da comida especial que da um bonus de 5 pontos na pontuacao final do pacMan;
    tPosicao comidaMod;                     //contem a struct da posicao da comida que modifica o caractere do pacMan;
} tJogo;


//funcoes para inicializar o jogo;
tJogo IniciarJogo(char argv[]);
tMapa InicializaMapas(int linha, int coluna);
tMapa LeMapa(FILE *mapaEncontrado, int linha, int coluna);
tPacMan ProcuraPacMan(tMapa mapas, int linha, int coluna);
tFantasmas ProcuraFantasmas(tMapa mapas, int linha, int coluna);
tTeleportes ProcuraTeleporte(tMapa mapas, int linha, int coluna);
int ContaQuantasComidas(tMapa mapas, int linha, int coluna);
tPosicao EncontraComidaEspecial(tMapa mapas, int linha, int coluna);
tPosicao EncontraComidaMod(tMapa mapas, int linha, int coluna);

//funcoes para realizar a partida;
void JogarPartida(tJogo jogo, char argv[]);
tMovimentos InicializaMovimentos();
    tMovimento ZerarMovimentos(tMovimento movimento);
tMovimentos ContaJogadas(tMovimentos movimentoss, char jogada);
tFantasmas VerificaParedeFantasma(tMapa mapas, tFantasmas fantasmasAntigos);
tJogo MovimentaFantasmas(tJogo jogoo, char jogada, int i);
    tFantasmas ExecutaMovimentoFantasma (tFantasmas fantasmasAntigos, char fantasmaDoCaso);
    tMapa LimpaCaminhoFantasmas(tMapa mapasAntigos, tFantasmas fantasmas, char fantasmaDoCaso);
tJogo MovimentaPacman(tJogo jogoo, char jogada, int i);
    tMapa PacManComeu(tMapa mapas, tPacMan pacMan, char jogada);
    tMapa PacManComeuEspecial(tMapa mapas, tPacMan pacMan, char jogada);
    tMapa PacManComeuMod(tMapa mapas, tPacMan pacMan, char jogada);
    tJogo PacManTeletransportou(tJogo jogoo, char jogada, int i);
    tJogo PacManEmCimaDoTeleporte(tJogo jogoo, int i);
    tPosicao ExecutaMovimentoPacMan(tPosicao posicaoPacMan, char jogada);
    tMapa LimpaCaminhoPacMan(tMapa mapas, tPacMan pacMan, char jogada);
    tMapa LimpaCaminhoPacManTeletransporte(tMapa mapas, tPacMan pacMan, tTeleportes teleporte, char jogada);
tMapa AtualizaMapa(tMapa mapas, tPacMan pacMan, tFantasmas fantasmas, int linha, int coluna);
tMapa AtualizaMapaMod(tMapa mapas, tPacMan pacMan, tFantasmas fantasmas, int linha, int coluna);
void ImprimeMapaAtual(int linha, int coluna, tMapa mapas, tPacMan pacMan, char jogada);
int PacManNoMapa(int linha, int coluna, tMapa mapas);

//funcoes para fazer os documentos;
void DocumentoInicializacao(tMapa mapas, int coluna, int linha, int posicaoLinhaInicial, int posicaoColunaIniciaL, char argv[]);
void DocumentoTrilha(tMapa mapas, int linha, int coluna, int i, char caminho[]);
void DocumentoResumo(tPacMan pacMan, int i, char jogada, char argv[]);
void DocumentoRanking(tMovimentos movimentos, char caminho[]);
void DocumentoEstatisticas(tMovimentos movimentos, char argv[]);

//funcoes que terminam a partida;
void PacManMorreu(tPacMan pacMan);
void PacManGanhou(tPacMan pacMan);


int main (int argc, char * argv[]) {
    if (argc <= 1) {                                                                                    
        printf ("ERRO: O diretorio de arquivos de configuracao nao foi informado\n");
        exit(0);
    }

    tJogo jogo;

    jogo = IniciarJogo(argv[1]);                                                                     
    
    JogarPartida(jogo, argv[1]);                                                                        
    
    return 0;
}

tJogo IniciarJogo(char argv[]) {                               
    FILE *mapaEncontrado;
    tJogo jogo;
    int i=0, j=0, posicaoLinhaInicial, posicaoColunaInicial;
    char caminhoDoMapa[1045];
    
    sprintf(caminhoDoMapa, "%s/mapa.txt", argv);
    mapaEncontrado = fopen(caminhoDoMapa, "r");
    
    if (mapaEncontrado == NULL) {                                           
        printf ("ERRO: Nao foi possivel localizar mapa.txt\n");                   
        exit(0);
    }
    
    fscanf(mapaEncontrado, "%d", &jogo.linha);                                  //le a quantidade de linhas do mapa;
    fscanf(mapaEncontrado, "%d", &jogo.coluna);                                 //le a quantidade de colunas do mapa;
    jogo.mapas = InicializaMapas(jogo.linha, jogo.coluna);

    fscanf(mapaEncontrado, "%d", &jogo.limiteJogadas);                          //le o limite de jogadas permitidas;
    fscanf(mapaEncontrado, "%*c");

    jogo.mapas = LeMapa(mapaEncontrado, jogo.linha, jogo.coluna);
    jogo.pacMan = ProcuraPacMan(jogo.mapas, jogo.linha, jogo.coluna);
    
    posicaoLinhaInicial = jogo.pacMan.posicaoPacMan.linha;
    posicaoColunaInicial = jogo.pacMan.posicaoPacMan.coluna;                    //chama a funcao que acha o pacMan no mapa e guarda a posicao;
    
    jogo.mapas.trilha[posicaoLinhaInicial][posicaoColunaInicial] = 0;           //determina a posicao inicial do pacMan na trilha;
    
    jogo.fantasmas = ProcuraFantasmas(jogo.mapas, jogo.linha, jogo.coluna);                       //chama a funcao que acha os fantasmas no mapa e guarda a posicao;            
    jogo.teleporte = ProcuraTeleporte(jogo.mapas, jogo.linha, jogo.coluna);                       //chama a funcao que acha os teleportes no mapa e guarda a posicao;    
    jogo.qtdComidas = ContaQuantasComidas(jogo.mapas, jogo.linha, jogo.coluna);                   //chama a funcao que conta as comidas que tem no mapa;
    jogo.comidaEspecial = EncontraComidaEspecial(jogo.mapas, jogo.linha, jogo.coluna);
    jogo.comidaMod = EncontraComidaMod(jogo.mapas, jogo.linha, jogo.coluna);
    DocumentoInicializacao(jogo.mapas, jogo.coluna, jogo.linha, posicaoLinhaInicial, posicaoColunaInicial, argv);                                                              

    fclose(mapaEncontrado);

    return jogo;                                                        
}

tMapa InicializaMapas(int linha, int coluna) {     //funcao que inicializa todos os mapas com as mesmas quantidades de linhas e colunas;
    tMapa mapas;

    mapas.mapaGeral[linha][coluna];              mapas.mapaComidas[linha][coluna];     
    mapas.trilha[linha][coluna];                 mapas.mapaTeleportes[linha][coluna]; 
    mapas.mapaComidaEspecial[linha][coluna];     mapas.mapaComidaMod[linha][coluna];

    return mapas;
}

tMapa LeMapa(FILE *mapaEncontrado, int linha, int coluna) {             //funcao para ler os mapas;
    tMapa mapas;
    int i=0, j=0;

    for (i=0; i<linha; i++) {
        for (j=0; j<coluna; j++) {
            fscanf(mapaEncontrado, "%c", &mapas.mapaGeral[i][j]);       //le caractere por caractere do mapa e insere no mapa geral;          
            mapas.mapaComidas[i][j] = mapas.mapaGeral[i][j];            //iguala o mapa de comidas ao mapa geral;
            mapas.mapaTeleportes[i][j] = mapas.mapaGeral[i][j];         //iguala o mapa de teleportes ao mapa geral;
            mapas.mapaComidaEspecial[i][j] = mapas.mapaGeral[i][j];     //iguala o mapa de comidas especiais ao mapa geral;
            mapas.mapaComidaMod[i][j] = mapas.mapaGeral[i][j];          //iguala o mapa de comidas que modificam o pacman ao mapa geral;
            mapas.trilha[i][j] = -1;                                    //inicia todas as posicoes da trilha com -1;
        }
        fscanf(mapaEncontrado, "%*c");
    }

    return mapas;
}

tPacMan ProcuraPacMan(tMapa mapas, int linha, int coluna) {      //procura o pacMan no mapa;
    int i=0, j=0;
    tPacMan pacMan;
    
    for (i=0; i<linha; i++) {
        for (j=0; j<coluna; j++) {
            if (mapas.mapaGeral[i][j]==PACMAN) {                 //se achar o pacMan no mapa...
                pacMan.posicaoPacMan.linha = i;                  //determina a linha e a coluna em que o pacman se encontra;
                pacMan.posicaoPacMan.coluna = j;
            }
        }
    }

    return pacMan;
}

tFantasmas ProcuraFantasmas(tMapa mapas, int linha, int coluna) {          //procura todos os fantasmas do mapa;
    tFantasmas fantasmas;
    int i=0, j=0;
    fantasmas.fantasmaB.existe = 0;     fantasmas.fantasmaP.existe = 0;    //inicializa os fantasmas como se todos nao existissem, ou seja, sao iguais a zero;
    fantasmas.fantasmaI.existe = 0;     fantasmas.fantasmaC.existe = 0;

    for (i=0; i<linha; i++) {
        for (j=0; j<coluna; j++) {
            if (mapas.mapaGeral[i][j]==FANTASMA_ESQUERDA) {                                                               //se achar um fantasma B...
                fantasmas.fantasmaB.sentido = SENTIDO_ESQUERDA;    fantasmas.fantasmaB.posicaoFantasma.linha = i;         //determina o sentido inicial do fantasma B, no caso esquerda;
                fantasmas.fantasmaB.existe = 1;       fantasmas.fantasmaB.posicaoFantasma.coluna = j;                     //determina que o fantasma B existe, ou seja, igual a 1;
            } else if (mapas.mapaGeral[i][j]==FANTASMA_CIMA) {                                                         
                fantasmas.fantasmaP.sentido = SENTIDO_CIMA;    fantasmas.fantasmaP.posicaoFantasma.linha = i;         
                fantasmas.fantasmaP.existe = 1;       fantasmas.fantasmaP.posicaoFantasma.coluna = j;        
            } else if (mapas.mapaGeral[i][j]==FANTASMA_BAIXO) {                                                        
                fantasmas.fantasmaI.sentido = SENTIDO_BAIXO;    fantasmas.fantasmaI.posicaoFantasma.linha = i;        
                fantasmas.fantasmaI.existe = 1;       fantasmas.fantasmaI.posicaoFantasma.coluna = j;        
            } else if (mapas.mapaGeral[i][j]==FANTASMA_DIREITA) {                                                         
                fantasmas.fantasmaC.sentido = SENTIDO_DIREITA;    fantasmas.fantasmaC.posicaoFantasma.linha = i;        
                fantasmas.fantasmaC.existe = 1;       fantasmas.fantasmaC.posicaoFantasma.coluna = j;        
            }
        }
    }

    return fantasmas;
}

tTeleportes ProcuraTeleporte(tMapa mapas, int linha, int coluna) {      //procura os dois teleportes no mapa;
    int i=0, j=0;
    tTeleportes teleporte;

    teleporte.achouTeleporte1 = 0;
    teleporte.achouTeleporte2 = 0;

    for (i=0; i<linha; i++) {
        for (j=0; j<coluna; j++) {
            if (mapas.mapaGeral[i][j]==TELEPORTE) {                     //se achar um teleporte no mapa...
                if (teleporte.achouTeleporte1 == 0) {                   //vai verificar se ja achou o primeiro, se nao achou...
                    teleporte.achouTeleporte1 = 1;                      //atribui 1 a variavel, ou seja, achou o teleporte 1;
                    teleporte.posicaoTeleporte1.linha = i;              //determina a linha e a coluna que o primeiro teleporte se encontra;
                    teleporte.posicaoTeleporte1.coluna = j;
                } else if (teleporte.achouTeleporte2 == 0) {            //se o primeiro teleporte ja foi encontrado, vai verificar se o segundo ja foi tambem, se nao...
                    teleporte.achouTeleporte2 = 1;                      //atribui 1 a variavel, ou seja, achou o teleporte 1;
                    teleporte.posicaoTeleporte2.linha = i;              //determina a linha e a coluna que o segundo teleporte se encontra;
                    teleporte.posicaoTeleporte2.coluna = j;
                }
            }
        }
    }

    return teleporte;
}

tPosicao EncontraComidaEspecial(tMapa mapas, int linha, int coluna) {   //procura as comidas especiais e guarda as suas posicoes;
    tPosicao comidaEspecial;
    int i=0, j=0;

    for (i=0; i<linha; i++) {
        for (j=0; j<coluna; j++) {
            if (mapas.mapaGeral[i][j] == COMIDA_ESPECIAL) {             //se achar uma comida especial...
                comidaEspecial.linha = i;                               //guarda a coluna e a linha do mapa em que se encontra;
                comidaEspecial.coluna = j;
            }
        }
    }

    return comidaEspecial;
}

tPosicao EncontraComidaMod(tMapa mapas, int linha, int coluna) {     //procura as comidas que modificam o pacman e guarda as suas posicoes;
    tPosicao comidaMod;
    int i=0, j=0;

    for (i=0; i<linha; i++) {
        for (j=0; j<coluna; j++) {
            if (mapas.mapaGeral[i][j] == COMIDA_MOD_PACMAN) {        //se encontrar uma dessas comidas...
                comidaMod.linha = i;                                 //guarda a coluna e a linha do mapa em que se encontra;
                comidaMod.coluna = j;
            }
        }
    }

    return comidaMod;
}

int ContaQuantasComidas(tMapa mapas, int linha, int coluna) {          //conta a quantidade de comidas presentes no mapa;
    int qtdComidas=0, i=0, j=0;

    for (i=0; i<linha; i++) {
        for (j=0; j<coluna; j++) {
            if (mapas.mapaGeral[i][j] == COMIDA) {      
                qtdComidas++;                             
            }
        }
    }

    return qtdComidas;
}

//funcao que cria o doc inicializacao.txt, onde tem o mapa e a posicao inicial do pacman
void DocumentoInicializacao(tMapa mapas, int coluna, int linha, int posicaoLinhaInicial, int posicaoColunaIniciaL, char argv[]) {           
    FILE *docInicializa;
    int i=0, j=0;
    char caminho[1045];

    sprintf(caminho, "%s/saida/inicializacao.txt", argv);

    docInicializa = fopen(caminho, "w");

    for (i=0; i<linha; i++) {
        for (j=0; j<coluna; j++) {
            fprintf(docInicializa, "%c", mapas.mapaGeral[i][j]);
        }
        fprintf(docInicializa, "\n");
    }
    fprintf(docInicializa, "Pac-Man comecara o jogo na linha %d e coluna %d\n", posicaoLinhaInicial+1, posicaoColunaIniciaL+1);

    fclose(docInicializa);
}

void JogarPartida(tJogo jogo, char argv[]) {            //funcao em que o jogador ira inserir suas jogadas;
    int i=1, contMod=0;
    char jogada;
    char caminho[1045];
    
    jogo.movimentos = InicializaMovimentos();           //iniciaiza a quantidade de jogadas na partida;
    jogo.pacMan.pontuacao = 0;                          //inicia a partida com a pontuacao do jogador zerada;
    jogo.pacMan.vida = 1;                               //inicia a partida com a vida do pacMan igual a 1, ou seja, vivo;
    jogo.pacMan.pegouMod = 0;  

    sprintf(caminho, "%s", argv);

    while(scanf("%c", &jogada)) {                       //enquanto uma jogada for inserida e nao atingir o limite de jogadas, o jogo vai rodar;   
        scanf("%*c");

        if (i > jogo.limiteJogadas) {                   //verifica se a quantidade de jogadas lidas atingiu o limite de jogadas, se sim...
            jogo.pacMan.vida = 0;                       //mata o pacMan, ja que o usuario perdeu o jogo;
            PacManMorreu(jogo.pacMan);                  //chama a funcao que imprime a mensagem que o jogador perdeu a partida;
            break;                                      //quebra o loop de jogadas e encerra a partida;                         
        }
                            
        jogo.movimentos = ContaJogadas(jogo.movimentos, jogada);

        jogo.pacMan.colidiuParede[i] = 0;   jogo.pacMan.usouTeleporte[i] = 0;                               //inicia todas as acoes do pacman como zero;
        jogo.pacMan.pegouComida[i] = 0;     jogo.pacMan.colidiuFantasma[i] = 0;                   

        jogo.fantasmas = VerificaParedeFantasma(jogo.mapas, jogo.fantasmas);                                //verifica se em frente dos fantasmas tem uma parede, porque se tiver, ele trocara o sentido do fantasma;     
        jogo = MovimentaFantasmas(jogo, jogada, i);                                                         //movimenta os fantasmas de acordo com o sentido e direcao em que eles estao;
        jogo.mapas = AtualizaMapa(jogo.mapas, jogo.pacMan, jogo.fantasmas, jogo.linha, jogo.coluna);        //atualiza o mapa para podermos movimentar o pacman
        
        if (jogo.pacMan.vida != 0) {                                                                        //se o pacman estiver vivo...
            jogo = MovimentaPacman(jogo, jogada, i);                                                        //chama a funcao que movimenta o pacMan de acordo com a jogada inserida;
            jogo.mapas = AtualizaMapa(jogo.mapas, jogo.pacMan, jogo.fantasmas, jogo.linha, jogo.coluna);                             
            
            if (jogo.pacMan.pegouMod != 0) {                                                                      //se o pacman consumiu uma comida em que faz mudar o seu caractere do mapa...
                contMod++;                                                                                        
                if ((contMod > 0) && (contMod <= 15)) {
                    jogo.mapas = AtualizaMapaMod(jogo.mapas, jogo.pacMan, jogo.fantasmas, jogo.linha, jogo.coluna);   //atualiza o mapa com o pacman modificado se nao tiver ultrapassado 15 jogadas em que o pacman ja esta modificado;

                    if (contMod == 15) {
                        contMod = 0;
                        jogo.pacMan.pegouMod = 0;                                              //se ja tem 15 jogadas que o pacman eh printado modificado, reinicia a verificacao do pacman ter consumido a comida para ele voltar ao seu caractere original;
                    }
                }
            }
        }

        ImprimeMapaAtual(jogo.linha, jogo.coluna, jogo.mapas, jogo.pacMan, jogada);            //imprime a situacao do mapa apos toda jogada, com as posicoes novas dos fantasmas e do pacMan;

        if (PacManNoMapa(jogo.linha, jogo.coluna, jogo.mapas) != 1) {        //se a funcao que verifica se o pacMan ainda esta no mapa retornar algo diferente de 1, quer dizer que ele foi morto por um fantasma;
            jogo.pacMan.colidiuFantasma[i] = 1;                              //atualiza que o pacMan colidiu com um fantasma para que essa informacao esteja no documento de resumo;
            DocumentoResumo(jogo.pacMan, i, jogada, caminho);         
            PacManMorreu(jogo.pacMan);                                 
            break;                                              
        }

        DocumentoTrilha(jogo.mapas, jogo.linha, jogo.coluna, i, caminho);   //chama a funcao que faz a trilha do pacman;   
        DocumentoResumo(jogo.pacMan, i, jogada, caminho);                   //chama a funcao que faz o resumo da partida;
        
        if (jogo.qtdComidas == 0) {                        //se as comidas acabaram, ou seja, o pacman consumiu todas...
            PacManGanhou(jogo.pacMan);                     //o jogador ganhou a partida;
            break;                                                    
        }
        if (jogo.pacMan.vida == 0) {                       //se o pacman esta morto...
            PacManMorreu(jogo.pacMan);                     //o jogador perdeu a partida;
            break;                                             
        }
        i++;
    }
                                                                                
    DocumentoRanking(jogo.movimentos, caminho);            //apos o fim da partida, chama a funcao que faz o ranking das jogadas;               
    DocumentoEstatisticas(jogo.movimentos, caminho);       //chama a funcao que faz o documento de estatisticas da partida;
}

tMovimentos InicializaMovimentos() {
    tMovimentos movimentos;
    
    movimentos.qtdJogadas = 0;
    movimentos.movimentoD = ZerarMovimentos(movimentos.movimentoD);    movimentos.movimentoA = ZerarMovimentos(movimentos.movimentoA);
    movimentos.movimentoW = ZerarMovimentos(movimentos.movimentoW);    movimentos.movimentoS = ZerarMovimentos(movimentos.movimentoS);
    
    return movimentos;
}

tMovimento ZerarMovimentos(tMovimento movimento) {
    movimento.qtd = movimento.bateu = movimento.comeu = 0;

    return movimento;
}

tMovimentos ContaJogadas(tMovimentos movimentos, char jogada) {
    movimentos.qtdJogadas++; 

    if (jogada == 'w') {
        movimentos.movimentoW.qtd++;
    } else if (jogada == 'a') {
        movimentos.movimentoA.qtd++;
    } else if (jogada == 'd') {
        movimentos.movimentoD.qtd++;
    } else if (jogada == 's') {
        movimentos.movimentoS.qtd++;
    }
    
    return movimentos;
}

tFantasmas VerificaParedeFantasma(tMapa mapas, tFantasmas fantasmas) {                      
    int fantasmaBlinha = fantasmas.fantasmaB.posicaoFantasma.linha;      int fantasmaIlinha = fantasmas.fantasmaI.posicaoFantasma.linha; 
    int fantasmaBcoluna = fantasmas.fantasmaB.posicaoFantasma.coluna;    int fantasmaIcoluna = fantasmas.fantasmaI.posicaoFantasma.coluna;
    int fantasmaClinha = fantasmas.fantasmaC.posicaoFantasma.linha;      int fantasmaPlinha = fantasmas.fantasmaP.posicaoFantasma.linha;  
    int fantasmaCcoluna = fantasmas.fantasmaC.posicaoFantasma.coluna;    int fantasmaPcoluna = fantasmas.fantasmaP.posicaoFantasma.coluna;

    if (fantasmas.fantasmaB.existe == 1) {                                  //verifica se o fantasma B existe;
        if ((fantasmas.fantasmaB.sentido == SENTIDO_ESQUERDA) && 
            (mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna-1] == PAREDE)) {              
            fantasmas.fantasmaB.sentido = SENTIDO_DIREITA;                         
        } else if ((fantasmas.fantasmaB.sentido == SENTIDO_DIREITA) && 
                    (mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna+1] == PAREDE)) {
            fantasmas.fantasmaB.sentido = SENTIDO_ESQUERDA;                        
        }
    } 
    if (fantasmas.fantasmaC.existe == 1) {                              
        if ((fantasmas.fantasmaC.sentido == SENTIDO_ESQUERDA) && 
            (mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna-1] == PAREDE)) {
            fantasmas.fantasmaC.sentido = SENTIDO_DIREITA;                         
        } else if ((fantasmas.fantasmaC.sentido == SENTIDO_DIREITA) && 
                   (mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna+1] == PAREDE)) {
            fantasmas.fantasmaC.sentido = SENTIDO_ESQUERDA;                          
        }
    }
    if (fantasmas.fantasmaI.existe == 1) {                                  //verifica se o fantasma I existe;
        if ((fantasmas.fantasmaI.sentido == SENTIDO_BAIXO) && 
            (mapas.mapaGeral[fantasmaIlinha+1][fantasmaIcoluna] == PAREDE)) {                  
            fantasmas.fantasmaI.sentido = SENTIDO_CIMA;                          //se em baixo do fantasma tem uma parede, ele muda o sentido e executa um movimento para cima;
        } else if ((fantasmas.fantasmaI.sentido == SENTIDO_CIMA) && 
                   (mapas.mapaGeral[fantasmaIlinha-1][fantasmaIcoluna] == PAREDE)) {
            fantasmas.fantasmaI.sentido = SENTIDO_BAIXO;                          //se em cima do fantasma tem uma parede, ele muda o sentido e executa um movimento para baixo;
        }
    }
    if (fantasmas.fantasmaP.existe == 1) {                                  //verifica se o fantasma P existe;
        if ((fantasmas.fantasmaP.sentido == SENTIDO_BAIXO) && 
            (mapas.mapaGeral[fantasmaPlinha+1][fantasmaPcoluna] == PAREDE)) {
            fantasmas.fantasmaP.sentido = SENTIDO_CIMA;                          //se em baixo do fantasma tem uma parede, ele muda o sentido e executa um movimento para cim
        } else if ((fantasmas.fantasmaP.sentido == SENTIDO_CIMA) &&
                   (mapas.mapaGeral[fantasmaPlinha-1][fantasmaPcoluna] == PAREDE)) {
            fantasmas.fantasmaP.sentido = SENTIDO_BAIXO;                          //se em cima do fantasma tem uma parede, ele muda o sentido e executa um movimento para baixo;
        }
    }

    return fantasmas;
}

tJogo MovimentaFantasmas(tJogo jogo, char jogada, int i) {                                          //movimenta o fantasma a cada jogada;
    char fantasmaDoCaso;                                                                      //determina uma constante char para a proxima funcao a ser chamada saber a qual dos fantasmas esta se referindo;
    int fantasmaBlinha = jogo.fantasmas.fantasmaB.posicaoFantasma.linha;      int fantasmaIlinha = jogo.fantasmas.fantasmaI.posicaoFantasma.linha; 
    int fantasmaBcoluna = jogo.fantasmas.fantasmaB.posicaoFantasma.coluna;    int fantasmaIcoluna = jogo.fantasmas.fantasmaI.posicaoFantasma.coluna;
    int fantasmaClinha = jogo.fantasmas.fantasmaC.posicaoFantasma.linha;      int fantasmaPlinha = jogo.fantasmas.fantasmaP.posicaoFantasma.linha;  
    int fantasmaCcoluna = jogo.fantasmas.fantasmaC.posicaoFantasma.coluna;    int fantasmaPcoluna = jogo.fantasmas.fantasmaP.posicaoFantasma.coluna;

    if (jogo.fantasmas.fantasmaB.existe == 1) {                                               //verifica se o fantasma B existe;
        fantasmaDoCaso = FANTASMA_ESQUERDA;
        if (((jogo.fantasmas.fantasmaB.sentido == SENTIDO_ESQUERDA) && 
            ((jogo.mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna-1] == PACMAN) || 
            (jogo.mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna-1] == PACMAN_MODIFICADO)) && (jogada == 'd'))) {
            jogo.pacMan.vida = 0;
            jogo.pacMan.colidiuFantasma[i] = 1;                                                                                                                //retorna que o pacMan morreu;                                                                                                                                 //se nao for nenhum dos casos acima, como por exemplo o fantasma encontrar uma comida, ele anda normalmente;
        } else if (((jogo.fantasmas.fantasmaB.sentido == SENTIDO_DIREITA) && 
                   ((jogo.mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna+1] == PACMAN) ||
                   (jogo.mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna-1] == PACMAN_MODIFICADO)) && (jogada == 'a'))) {
            jogo.pacMan.vida = 0;
            jogo.pacMan.colidiuFantasma[i] = 1; 
        }
        jogo.fantasmas = ExecutaMovimentoFantasma(jogo.fantasmas, fantasmaDoCaso);
        jogo.mapas = LimpaCaminhoFantasmas(jogo.mapas, jogo.fantasmas, fantasmaDoCaso);
    }

    if (jogo.fantasmas.fantasmaC.existe == 1) {                                               //verifica se o fantasma C existe;
        fantasmaDoCaso = FANTASMA_DIREITA;                                                               
        if (((jogo.fantasmas.fantasmaC.sentido == SENTIDO_ESQUERDA) && 
            ((jogo.mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna-1] == PACMAN) ||
            (jogo.mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna-1] == PACMAN_MODIFICADO)) && (jogada == 'd'))) {
            jogo.pacMan.vida = 0;
            jogo.pacMan.colidiuFantasma[i] = 1;
        } else if ((jogo.fantasmas.fantasmaC.sentido == SENTIDO_DIREITA) &&
                   (jogo.mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna+1] == PACMAN) && (jogada == 'a')) {
            jogo.pacMan.vida = 0;
            jogo.pacMan.colidiuFantasma[i] = 1;  
        }
        jogo.fantasmas = ExecutaMovimentoFantasma(jogo.fantasmas, fantasmaDoCaso);
        jogo.mapas = LimpaCaminhoFantasmas(jogo.mapas, jogo.fantasmas, fantasmaDoCaso);    
    } 

    if (jogo.fantasmas.fantasmaI.existe == 1) {                                               //verifica se o fantasma I existe;
        fantasmaDoCaso = FANTASMA_BAIXO;
        if (((jogo.fantasmas.fantasmaI.sentido == SENTIDO_BAIXO) && 
            ((jogo.mapas.mapaGeral[fantasmaIlinha+1][fantasmaIcoluna] == PACMAN) ||
            (jogo.mapas.mapaGeral[fantasmaIlinha+1][fantasmaIcoluna] == PACMAN_MODIFICADO)) && (jogada == 'w'))) {
            jogo.pacMan.vida = 0;
            jogo.pacMan.colidiuFantasma[i] = 1;
        } else if ((jogo.fantasmas.fantasmaI.sentido == SENTIDO_CIMA) && 
                   (jogo.mapas.mapaGeral[fantasmaIlinha-1][fantasmaIcoluna] == PACMAN) && (jogada == 's')) {
            jogo.pacMan.vida = 0;
            jogo.pacMan.colidiuFantasma[i] = 1;
        }
        jogo.fantasmas = ExecutaMovimentoFantasma(jogo.fantasmas, fantasmaDoCaso);
        jogo.mapas = LimpaCaminhoFantasmas(jogo.mapas, jogo.fantasmas, fantasmaDoCaso);
    } 

    if (jogo.fantasmas.fantasmaP.existe == 1) {                                               //verifica se o fantasma P existe;
        fantasmaDoCaso = FANTASMA_CIMA;
        if (((jogo.fantasmas.fantasmaP.sentido == SENTIDO_BAIXO) && 
            ((jogo.mapas.mapaGeral[fantasmaPlinha+1][fantasmaPcoluna] == PACMAN) ||
            (jogo.mapas.mapaGeral[fantasmaPlinha+1][fantasmaPcoluna] == PACMAN_MODIFICADO)) && (jogada == 'w'))) {
            jogo.pacMan.vida = 0;
            jogo.pacMan.colidiuFantasma[i] = 1;
        } else if ((jogo.fantasmas.fantasmaP.sentido == SENTIDO_CIMA) &&
                   (jogo.mapas.mapaGeral[fantasmaPlinha-1][fantasmaPcoluna] == PACMAN) && (jogada == 's')) {
            jogo.pacMan.vida = 0;
            jogo.pacMan.colidiuFantasma[i] = 1;
        }
        jogo.fantasmas = ExecutaMovimentoFantasma(jogo.fantasmas, fantasmaDoCaso);
        jogo.mapas = LimpaCaminhoFantasmas(jogo.mapas, jogo.fantasmas, fantasmaDoCaso);
    }

    return jogo;
}

tFantasmas ExecutaMovimentoFantasma (tFantasmas fantasmas, char fantasmaDoCaso) {   
    int fantasmaBcoluna = fantasmas.fantasmaB.posicaoFantasma.coluna;   int fantasmaCcoluna = fantasmas.fantasmaC.posicaoFantasma.coluna;
    int fantasmaIlinha = fantasmas.fantasmaI.posicaoFantasma.linha;     int fantasmaPlinha = fantasmas.fantasmaP.posicaoFantasma.linha;

    if (fantasmaDoCaso == FANTASMA_ESQUERDA) {                          
        if (fantasmas.fantasmaB.sentido == SENTIDO_ESQUERDA) {                                                                    
            fantasmas.fantasmaB.posicaoFantasma.coluna = fantasmaBcoluna-1;
        } else if (fantasmas.fantasmaB.sentido == SENTIDO_DIREITA) {
            fantasmas.fantasmaB.posicaoFantasma.coluna = fantasmaBcoluna+1;
        }
    } 
    else if (fantasmaDoCaso == FANTASMA_DIREITA) {                
        if (fantasmas.fantasmaC.sentido == SENTIDO_ESQUERDA) {                                                                  
            fantasmas.fantasmaC.posicaoFantasma.coluna = fantasmaCcoluna-1;
        }  else if (fantasmas.fantasmaC.sentido == SENTIDO_DIREITA) {
            fantasmas.fantasmaC.posicaoFantasma.coluna = fantasmaCcoluna+1;
        }
    }
    else if (fantasmaDoCaso == FANTASMA_BAIXO) {                                                                               
        if (fantasmas.fantasmaI.sentido == SENTIDO_BAIXO) {                                                                    
            fantasmas.fantasmaI.posicaoFantasma.linha = fantasmaIlinha+1;
        } else if (fantasmas.fantasmaI.sentido == SENTIDO_CIMA) {
            fantasmas.fantasmaI.posicaoFantasma.linha = fantasmaIlinha-1;
        }
    }
    else if (fantasmaDoCaso == FANTASMA_CIMA) {                  
        if (fantasmas.fantasmaP.sentido == SENTIDO_BAIXO) {                                    
            fantasmas.fantasmaP.posicaoFantasma.linha =  fantasmaPlinha+1;
        } else if (fantasmas.fantasmaP.sentido == SENTIDO_CIMA) {
            fantasmas.fantasmaP.posicaoFantasma.linha =  fantasmaPlinha-1;
        }
    }

    return fantasmas;
}

tMapa LimpaCaminhoFantasmas(tMapa mapas, tFantasmas fantasmas, char fantasmaDoCaso) {
    int fantasmaBlinha = fantasmas.fantasmaB.posicaoFantasma.linha;      int fantasmaIlinha = fantasmas.fantasmaI.posicaoFantasma.linha; 
    int fantasmaBcoluna = fantasmas.fantasmaB.posicaoFantasma.coluna;    int fantasmaIcoluna = fantasmas.fantasmaI.posicaoFantasma.coluna;
    int fantasmaClinha = fantasmas.fantasmaC.posicaoFantasma.linha;      int fantasmaPlinha = fantasmas.fantasmaP.posicaoFantasma.linha;  
    int fantasmaCcoluna = fantasmas.fantasmaC.posicaoFantasma.coluna;    int fantasmaPcoluna = fantasmas.fantasmaP.posicaoFantasma.coluna;

    if (fantasmaDoCaso == FANTASMA_ESQUERDA) {    
        if (fantasmas.fantasmaB.sentido == SENTIDO_ESQUERDA) {
            if (mapas.mapaComidas[fantasmaBlinha][fantasmaBcoluna+1] == COMIDA) {
                mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna+1] = COMIDA;
            } else if (mapas.mapaTeleportes[fantasmaBlinha][fantasmaBcoluna+1] ==  TELEPORTE) {
                mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna+1] = TELEPORTE;
            } else if (mapas.mapaComidaEspecial[fantasmaBlinha][fantasmaBcoluna+1] == COMIDA_ESPECIAL) {
                mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna+1] = COMIDA_ESPECIAL; 
            } else if (mapas.mapaComidaMod[fantasmaBlinha][fantasmaBcoluna+1] == COMIDA_MOD_PACMAN) {
                mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna+1] = COMIDA_MOD_PACMAN; 
            } else {
                mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna+1] = VAZIO;
            }
        } else if (fantasmas.fantasmaB.sentido == SENTIDO_DIREITA) {
            if (mapas.mapaComidas[fantasmaBlinha][fantasmaBcoluna-1] == COMIDA) {
                mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna-1] = COMIDA;
            } else if (mapas.mapaTeleportes[fantasmaBlinha][fantasmaBcoluna-1] == TELEPORTE) {
                mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna-1] = TELEPORTE;
            } else if (mapas.mapaComidaEspecial[fantasmaBlinha][fantasmaBcoluna-1] == COMIDA_ESPECIAL) {
                mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna-1] = COMIDA_ESPECIAL; 
            } else if (mapas.mapaComidaMod[fantasmaBlinha][fantasmaBcoluna-1] == COMIDA_MOD_PACMAN) {
                mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna+1] = COMIDA_MOD_PACMAN; 
            } else {
                mapas.mapaGeral[fantasmaBlinha][fantasmaBcoluna-1] = VAZIO;
            }
        }
    } 
    else if (fantasmaDoCaso == FANTASMA_DIREITA) {
        if (fantasmas.fantasmaC.sentido == SENTIDO_ESQUERDA) {
            if (mapas.mapaComidas[fantasmaClinha][fantasmaCcoluna+1] == COMIDA) {
                mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna+1] = COMIDA;
            } else if (mapas.mapaTeleportes[fantasmaClinha][fantasmaCcoluna+1] == TELEPORTE) {
                mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna+1] = TELEPORTE;
            } else if (mapas.mapaComidaEspecial[fantasmaClinha][fantasmaCcoluna+1] == COMIDA_ESPECIAL) {
                mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna+1] = COMIDA_ESPECIAL; 
            } else if (mapas.mapaComidaMod[fantasmaClinha][fantasmaCcoluna+1] == COMIDA_MOD_PACMAN) {
                mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna+1] = COMIDA_MOD_PACMAN; 
            } else {
                mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna+1] = VAZIO;
            }
        } else if (fantasmas.fantasmaC.sentido == SENTIDO_DIREITA) {
            if (mapas.mapaComidas[fantasmaClinha][fantasmaCcoluna-1] == COMIDA) {
                mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna-1] = COMIDA;
            } else if (mapas.mapaTeleportes[fantasmaClinha][fantasmaCcoluna-1] == TELEPORTE) {
                mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna-1] = TELEPORTE;
            } else if (mapas.mapaComidaEspecial[fantasmaClinha][fantasmaCcoluna-1] == COMIDA_ESPECIAL) {
                mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna-1] = COMIDA_ESPECIAL; 
            } else if (mapas.mapaComidaMod[fantasmaClinha][fantasmaCcoluna-1] == COMIDA_MOD_PACMAN) {
                mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna-1] = COMIDA_MOD_PACMAN; 
            } else {
                mapas.mapaGeral[fantasmaClinha][fantasmaCcoluna-1] = VAZIO;
            }
        }
    }
    else if (fantasmaDoCaso == FANTASMA_BAIXO) {
        if (fantasmas.fantasmaI.sentido == SENTIDO_BAIXO) {
            if (mapas.mapaComidas[fantasmaIlinha-1][fantasmaIcoluna] == COMIDA) {
                mapas.mapaGeral[fantasmaIlinha-1][fantasmaIcoluna] = COMIDA;
            } else if (mapas.mapaTeleportes[fantasmaIlinha-1][fantasmaIcoluna] == TELEPORTE) {
                mapas.mapaGeral[fantasmaIlinha-1][fantasmaIcoluna] = TELEPORTE;
            } else if (mapas.mapaComidaEspecial[fantasmaIlinha-1][fantasmaIcoluna] == COMIDA_ESPECIAL) {
                mapas.mapaGeral[fantasmaIlinha-1][fantasmaIcoluna] = COMIDA_ESPECIAL; 
            } else if (mapas.mapaComidaMod[fantasmaIlinha-1][fantasmaIcoluna] == COMIDA_MOD_PACMAN) {
                mapas.mapaGeral[fantasmaIlinha-1][fantasmaIcoluna] = COMIDA_MOD_PACMAN; 
            } else {
                mapas.mapaGeral[fantasmaIlinha-1][fantasmaIcoluna] = VAZIO;
            }
        } else if (fantasmas.fantasmaI.sentido == SENTIDO_CIMA) {
            if (mapas.mapaComidas[fantasmaIlinha+1][fantasmaIcoluna] == COMIDA) {
                mapas.mapaGeral[fantasmaIlinha+1][fantasmaIcoluna] = COMIDA;
            } else if (mapas.mapaTeleportes[fantasmaIlinha+1][fantasmaIcoluna] == TELEPORTE) {
                mapas.mapaGeral[fantasmaIlinha+1][fantasmaIcoluna] = TELEPORTE;
            } else if (mapas.mapaComidaEspecial[fantasmaIlinha+1][fantasmaIcoluna] == COMIDA_ESPECIAL) {
                mapas.mapaGeral[fantasmaIlinha+1][fantasmaIcoluna] = COMIDA_ESPECIAL; 
            } else if (mapas.mapaComidaMod[fantasmaIlinha+1][fantasmaIcoluna] == COMIDA_MOD_PACMAN) {
                mapas.mapaGeral[fantasmaIlinha+1][fantasmaIcoluna] = COMIDA_MOD_PACMAN; 
            } else {
                mapas.mapaGeral[fantasmaIlinha+1][fantasmaIcoluna] = VAZIO;
            }
        }
    }
    else if (fantasmaDoCaso == FANTASMA_CIMA) {
        if (fantasmas.fantasmaP.sentido == SENTIDO_BAIXO) {
            if (mapas.mapaComidas[fantasmaPlinha-1][fantasmaPcoluna] == COMIDA) {
                mapas.mapaGeral[fantasmaPlinha-1][fantasmaPcoluna] = COMIDA;
            } else if (mapas.mapaTeleportes[fantasmaPlinha-1][fantasmaPcoluna] == TELEPORTE) {
                mapas.mapaGeral[fantasmaPlinha-1][fantasmaPcoluna] = TELEPORTE;
            } else if (mapas.mapaComidaEspecial[fantasmaPlinha-1][fantasmaPcoluna] == COMIDA_ESPECIAL) {
                mapas.mapaGeral[fantasmaPlinha-1][fantasmaPcoluna] = COMIDA_ESPECIAL; 
            } else if (mapas.mapaComidaMod[fantasmaPlinha-1][fantasmaPcoluna] == COMIDA_MOD_PACMAN) {
                mapas.mapaGeral[fantasmaPlinha-1][fantasmaPcoluna] = COMIDA_MOD_PACMAN; 
            } else {
                mapas.mapaGeral[fantasmaPlinha-1][fantasmaPcoluna] = VAZIO;
            }
        } else if (fantasmas.fantasmaP.sentido == SENTIDO_CIMA) {
            if (mapas.mapaComidas[fantasmaPlinha+1][fantasmaPcoluna] == COMIDA) {
                mapas.mapaGeral[fantasmaPlinha+1][fantasmaPcoluna] = COMIDA;
            } else if (mapas.mapaTeleportes[fantasmaPlinha+1][fantasmaPcoluna] == TELEPORTE) {
                mapas.mapaGeral[fantasmaPlinha+1][fantasmaPcoluna] = TELEPORTE;
            } else if (mapas.mapaComidaEspecial[fantasmaPlinha+1][fantasmaPcoluna] == COMIDA_ESPECIAL) {
                mapas.mapaGeral[fantasmaPlinha+1][fantasmaPcoluna] = COMIDA_ESPECIAL; 
            } else if (mapas.mapaComidaMod[fantasmaPlinha+1][fantasmaPcoluna] == COMIDA_MOD_PACMAN) {
                mapas.mapaGeral[fantasmaPlinha+1][fantasmaPcoluna] = COMIDA_MOD_PACMAN; 
            } else {
                mapas.mapaGeral[fantasmaPlinha+1][fantasmaPcoluna] = VAZIO;
            }
        }
    }

    return mapas;
}

tJogo MovimentaPacman(tJogo jogo, char jogada, int i) {                          //com base na jogada inserida, determina o movimento do pacMan;
    int pacManLinha = jogo.pacMan.posicaoPacMan.linha;
    int pacManColuna = jogo.pacMan.posicaoPacMan.coluna;

    if (jogada == 'w') {                                                                                                //se a jogada for para cima...
        if (jogo.mapas.mapaGeral[pacManLinha-1][pacManColuna] == COMIDA) {         
            jogo.mapas = PacManComeu(jogo.mapas, jogo.pacMan, jogada);                                                                          
            jogo.pacMan.pegouComida[i] = 1;                                                                                   
            jogo.movimentos.movimentoW.comeu++;
            jogo.qtdComidas--;  
            jogo.pacMan.pontuacao++;                                                                                       
        } else if (jogo.mapas.mapaGeral[pacManLinha-1][pacManColuna] == COMIDA_ESPECIAL) {
            jogo.mapas = PacManComeuEspecial(jogo.mapas, jogo.pacMan, jogada);
            jogo.pacMan.pontuacao+=5;
        } else if (jogo.mapas.mapaGeral[pacManLinha-1][pacManColuna] == COMIDA_MOD_PACMAN) {
            jogo.mapas = PacManComeuMod(jogo.mapas, jogo.pacMan, jogada);
            jogo.pacMan.pegouMod = 1;
        }else if (jogo.mapas.mapaGeral[pacManLinha-1][pacManColuna] == TELEPORTE) {
            jogo = PacManTeletransportou(jogo, jogada, i);                                                                   
            jogo.mapas = LimpaCaminhoPacManTeletransporte(jogo.mapas, jogo.pacMan, jogo.teleporte, jogada);                                          
            jogo.pacMan.usouTeleporte[i] = 1;                                                                              
        } else if (jogo.mapas.mapaGeral[pacManLinha-1][pacManColuna] == PAREDE) {       
            if (jogo.mapas.mapaTeleportes[pacManLinha][pacManColuna] == TELEPORTE) {
                jogo = PacManEmCimaDoTeleporte(jogo, i);                                                                  
                jogo.pacMan.usouTeleporte[i] = 1;                                                                            
            }
            jogo.pacMan.colidiuParede[i] = 1;                                                                                
            jogo.movimentos.movimentoW.bateu++;                                                                               
        } else if ((jogo.mapas.mapaGeral[pacManLinha-1][pacManColuna] == FANTASMA_ESQUERDA)
                || (jogo.mapas.mapaGeral[pacManLinha-1][pacManColuna] == FANTASMA_DIREITA)
                || (jogo.mapas.mapaGeral[pacManLinha-1][pacManColuna] == FANTASMA_BAIXO)
                || (jogo.mapas.mapaGeral[pacManLinha-1][pacManColuna] == FANTASMA_CIMA)) {         
            jogo.pacMan.vida=0;                                                                                               //retorna que o pacMan morreu;
            jogo.pacMan.colidiuFantasma[i] = 1;                                                                               //retorna que o pacMan colidiu com um fantasma;
            jogo.mapas = LimpaCaminhoPacMan(jogo.mapas, jogo.pacMan, jogada);                                                                          //limpa o rastro do pacMan;
        }

        if ((jogo.pacMan.colidiuParede[i] != 1) && (jogo.pacMan.usouTeleporte[i] != 1) && (jogo.pacMan.vida != 0)) {          //se o pacMan nao bateu em uma parede, nao usou teleporte e coninua vivo...
            jogo.pacMan.posicaoPacMan = ExecutaMovimentoPacMan(jogo.pacMan.posicaoPacMan, jogada);                                           //chama a funcao que executa o movimento do pacMan;   
            jogo.mapas = LimpaCaminhoPacMan(jogo.mapas, jogo.pacMan, jogada);                                                                          //limpa o rastro do pacMan; 
        }
        if (jogo.pacMan.vida == 1) {                                                                                          //se o pacMan ainda esta vivo...
            jogo.mapas.trilha[jogo.pacMan.posicaoPacMan.linha][jogo.pacMan.posicaoPacMan.coluna] = i;                         //adiciona o numero da jogada ao mapa da trilha na posicao em que o pacMan se encontra depois de mover;       
        }
        
    } 
    
    else if (jogada == 's') {
        if (jogo.mapas.mapaGeral[pacManLinha+1][pacManColuna] == COMIDA) {   
            jogo.mapas = PacManComeu(jogo.mapas, jogo.pacMan, jogada);
            jogo.pacMan.pegouComida[i] = 1;   
            jogo.movimentos.movimentoS.comeu++;
            jogo.qtdComidas--;         
            jogo.pacMan.pontuacao++;                                                              
        } else if (jogo.mapas.mapaGeral[pacManLinha+1][pacManColuna] == COMIDA_ESPECIAL) {
            jogo.mapas = PacManComeuEspecial(jogo.mapas, jogo.pacMan, jogada);
            jogo.pacMan.pontuacao+=5;
        } else if (jogo.mapas.mapaGeral[pacManLinha+1][pacManColuna] == COMIDA_MOD_PACMAN) {
            jogo.mapas = PacManComeuMod(jogo.mapas, jogo.pacMan, jogada);
            jogo.pacMan.pegouMod = 1;
        } else if (jogo.mapas.mapaGeral[pacManLinha+1][pacManColuna] == TELEPORTE) {
            jogo = PacManTeletransportou(jogo, jogada, i);
            jogo.mapas = LimpaCaminhoPacManTeletransporte(jogo.mapas, jogo.pacMan, jogo.teleporte, jogada);   
            jogo.pacMan.usouTeleporte[i] = 1;
        } else if (jogo.mapas.mapaGeral[pacManLinha+1][pacManColuna] == PAREDE) {        
            if (jogo.mapas.mapaTeleportes[pacManLinha][pacManColuna] == TELEPORTE) {
                jogo = PacManEmCimaDoTeleporte(jogo, i);
                jogo.pacMan.usouTeleporte[i] = 1;
            }
            jogo.pacMan.colidiuParede[i] = 1;
            jogo.movimentos.movimentoS.bateu++;                                                                       
        } else if ((jogo.mapas.mapaGeral[pacManLinha+1][pacManColuna] == FANTASMA_ESQUERDA)
                || (jogo.mapas.mapaGeral[pacManLinha+1][pacManColuna] == FANTASMA_DIREITA)
                || (jogo.mapas.mapaGeral[pacManLinha+1][pacManColuna] == FANTASMA_BAIXO)
                || (jogo.mapas.mapaGeral[pacManLinha+1][pacManColuna] == FANTASMA_CIMA)) {                                                        
            jogo.pacMan.vida=0;
            jogo.pacMan.colidiuFantasma[i] = 1;
            jogo.mapas = LimpaCaminhoPacMan(jogo.mapas, jogo.pacMan, jogada);               
        }

        if ((jogo.pacMan.colidiuParede[i] != 1) && (jogo.pacMan.usouTeleporte[i] != 1) && (jogo.pacMan.vida != 0)) {
            jogo.pacMan.posicaoPacMan = ExecutaMovimentoPacMan(jogo.pacMan.posicaoPacMan, jogada);                                
            jogo.mapas = LimpaCaminhoPacMan(jogo.mapas, jogo.pacMan, jogada);
        }
        if (jogo.pacMan.vida == 1) {
            jogo.mapas.trilha[jogo.pacMan.posicaoPacMan.linha][jogo.pacMan.posicaoPacMan.coluna] = i;  
        }
    }
    
    else if (jogada == 'a') {                                                                               
        if (jogo.mapas.mapaGeral[pacManLinha][pacManColuna-1] == COMIDA) {                
            jogo.mapas = PacManComeu(jogo.mapas, jogo.pacMan, jogada);  
            jogo.pacMan.pegouComida[i] = 1;   
            jogo.movimentos.movimentoA.comeu++;  
            jogo.qtdComidas--;           
            jogo.pacMan.pontuacao++;                                                                                                                                
        } else if (jogo.mapas.mapaGeral[pacManLinha][pacManColuna-1] == COMIDA_ESPECIAL) {
            jogo.mapas = PacManComeuEspecial(jogo.mapas, jogo.pacMan, jogada);
            jogo.pacMan.pontuacao+=5; 
        } else if (jogo.mapas.mapaGeral[pacManLinha][pacManColuna-1] == COMIDA_MOD_PACMAN) {
            jogo.mapas = PacManComeuMod(jogo.mapas, jogo.pacMan, jogada);
            jogo.pacMan.pegouMod = 1;
        } else if (jogo.mapas.mapaGeral[pacManLinha][pacManColuna-1] == TELEPORTE) {
            jogo = PacManTeletransportou(jogo, jogada, i);
            jogo.mapas = LimpaCaminhoPacManTeletransporte(jogo.mapas, jogo.pacMan, jogo.teleporte, jogada);    
            jogo.pacMan.usouTeleporte[i] = 1;
        } else if (jogo.mapas.mapaGeral[pacManLinha][pacManColuna-1] == PAREDE) {        
            if (jogo.mapas.mapaTeleportes[pacManLinha][pacManColuna] == TELEPORTE) {
                jogo = PacManEmCimaDoTeleporte(jogo, i);
                jogo.pacMan.usouTeleporte[i] = 1;
            }
            jogo.pacMan.colidiuParede[i] = 1;
            jogo.movimentos.movimentoA.bateu++;                                                                      
        } else if ((jogo.mapas.mapaGeral[pacManLinha][pacManColuna-1] == FANTASMA_ESQUERDA)
                || (jogo.mapas.mapaGeral[pacManLinha][pacManColuna-1] == FANTASMA_DIREITA)
                || (jogo.mapas.mapaGeral[pacManLinha][pacManColuna-1] == FANTASMA_BAIXO)
                || (jogo.mapas.mapaGeral[pacManLinha][pacManColuna-1] == FANTASMA_CIMA)) {                                                        
            jogo.pacMan.vida=0;
            jogo.pacMan.colidiuFantasma[i] = 1;
            jogo.mapas = LimpaCaminhoPacMan(jogo.mapas, jogo.pacMan, jogada);             
        }

        if ((jogo.pacMan.colidiuParede[i] != 1) && (jogo.pacMan.usouTeleporte[i] != 1) && (jogo.pacMan.vida != 0)) {
            jogo.pacMan.posicaoPacMan = ExecutaMovimentoPacMan(jogo.pacMan.posicaoPacMan, jogada);                       
            jogo.mapas = LimpaCaminhoPacMan(jogo.mapas, jogo.pacMan, jogada);
        }
        if (jogo.pacMan.vida == 1) {
            jogo.mapas.trilha[jogo.pacMan.posicaoPacMan.linha][jogo.pacMan.posicaoPacMan.coluna] = i;  
        }
    } 
    
    else if (jogada == 'd') {                                                                          
        if (jogo.mapas.mapaGeral[pacManLinha][pacManColuna+1] == COMIDA) {              
            jogo.mapas = PacManComeu(jogo.mapas, jogo.pacMan, jogada);
            jogo.pacMan.pegouComida[i] = 1; 
            jogo.movimentos.movimentoD.comeu++; 
            jogo.qtdComidas--;        
            jogo.pacMan.pontuacao++;                                                               
        } else if (jogo.mapas.mapaGeral[pacManLinha][pacManColuna+1] == COMIDA_ESPECIAL) {
            jogo.mapas = PacManComeuEspecial(jogo.mapas, jogo.pacMan, jogada);
            jogo.pacMan.pontuacao+=5;
        } else if (jogo.mapas.mapaGeral[pacManLinha][pacManColuna+1] == COMIDA_MOD_PACMAN) {
            jogo.mapas = PacManComeuMod(jogo.mapas, jogo.pacMan, jogada);
            jogo.pacMan.pegouMod = 1;
        } else if (jogo.mapas.mapaGeral[pacManLinha][pacManColuna+1] == TELEPORTE) {
            jogo = PacManTeletransportou(jogo, jogada, i);
            jogo.mapas = LimpaCaminhoPacManTeletransporte(jogo.mapas, jogo.pacMan, jogo.teleporte, jogada);   
            jogo.pacMan.usouTeleporte[i] = 1;
        } else if (jogo.mapas.mapaGeral[pacManLinha][pacManColuna+1] == PAREDE) {       
            if (jogo.mapas.mapaTeleportes[pacManLinha][pacManColuna] == TELEPORTE) {
                jogo = PacManEmCimaDoTeleporte(jogo, i);
                jogo.pacMan.usouTeleporte[i] = 1;
            }
            jogo.pacMan.colidiuParede[i] = 1;
            jogo.movimentos.movimentoD.bateu++;                                                                     
        } else if ((jogo.mapas.mapaGeral[pacManLinha][pacManColuna+1] == FANTASMA_ESQUERDA)
                || (jogo.mapas.mapaGeral[pacManLinha][pacManColuna+1] == FANTASMA_DIREITA)
                || (jogo.mapas.mapaGeral[pacManLinha][pacManColuna+1] == FANTASMA_BAIXO)
                || (jogo.mapas.mapaGeral[pacManLinha][pacManColuna+1] == FANTASMA_CIMA)) {                                    
            jogo.pacMan.vida=0;
            jogo.pacMan.colidiuFantasma[i] = 1;
            jogo.mapas = LimpaCaminhoPacMan(jogo.mapas, jogo.pacMan, jogada);             
        }
        
        if ((jogo.pacMan.colidiuParede[i] != 1) && (jogo.pacMan.usouTeleporte[i] != 1) && (jogo.pacMan.vida != 0)) {
            jogo.pacMan.posicaoPacMan = ExecutaMovimentoPacMan(jogo.pacMan.posicaoPacMan, jogada);                             
            jogo.mapas = LimpaCaminhoPacMan(jogo.mapas, jogo.pacMan, jogada); 
        }
        if (jogo.pacMan.vida == 1) {
            jogo.mapas.trilha[jogo.pacMan.posicaoPacMan.linha][jogo.pacMan.posicaoPacMan.coluna] = i;   
        }
    }

    return jogo;
}

tMapa PacManComeu(tMapa mapas, tPacMan pacMan, char jogada) {                                         //limpa a posicao da comida que o pacMan vai consumir do mapa geral e do mapa de comida;
    int pacManLinha = pacMan.posicaoPacMan.linha;
    int pacManColuna = pacMan.posicaoPacMan.coluna;
    
    if (jogada == 'w') {                                                              //se a jogada foi para cima...
        mapas.mapaComidas[pacManLinha-1][pacManColuna] = VAZIO;
        mapas.mapaGeral[pacManLinha-1][pacManColuna] = VAZIO;                                        
    } 
    
    else if (jogada == 's') {                                                          //se a jogada for para baixo...
        mapas.mapaComidas[pacManLinha+1][pacManColuna] = VAZIO;
        mapas.mapaGeral[pacManLinha+1][pacManColuna] = VAZIO;
    } 
    
    else if (jogada == 'a') {                                                        //se a jogada for para a esquerda...
        mapas.mapaComidas[pacManLinha][pacManColuna-1] = VAZIO;
        mapas.mapaGeral[pacManLinha][pacManColuna-1] = VAZIO;
    } 
    
    else if (jogada == 'd') {                                                         //se a jogada for para a direita...
        mapas.mapaComidas[pacManLinha][pacManColuna+1] = VAZIO;
        mapas.mapaGeral[pacManLinha][pacManColuna+1] = VAZIO;
    }

    return mapas;
}

tMapa PacManComeuEspecial(tMapa mapas, tPacMan pacMan, char jogada) {
    int pacManLinha = pacMan.posicaoPacMan.linha;
    int pacManColuna = pacMan.posicaoPacMan.coluna;
    
    if (jogada == 'w') {                                                              //se a jogada foi para cima...
        mapas.mapaComidaEspecial[pacManLinha-1][pacManColuna] = VAZIO;
        mapas.mapaGeral[pacManLinha-1][pacManColuna] = VAZIO;                                        
    } 
    
    else if (jogada == 's') {                                                          //se a jogada for para baixo...
        mapas.mapaComidaEspecial[pacManLinha+1][pacManColuna] = VAZIO;
        mapas.mapaGeral[pacManLinha+1][pacManColuna] = VAZIO;
    } 
    
    else if (jogada == 'a') {                                                        //se a jogada for para a esquerda...
        mapas.mapaComidaEspecial[pacManLinha][pacManColuna-1] = VAZIO;
        mapas.mapaGeral[pacManLinha][pacManColuna-1] = VAZIO;
    } 
    
    else if (jogada == 'd') {                                                         //se a jogada for para a direita...
        mapas.mapaComidaEspecial[pacManLinha][pacManColuna+1] = VAZIO;
        mapas.mapaGeral[pacManLinha][pacManColuna+1] = VAZIO;
    }

    return mapas;
}

tMapa PacManComeuMod(tMapa mapas, tPacMan pacMan, char jogada) {
    int pacManLinha = pacMan.posicaoPacMan.linha;
    int pacManColuna = pacMan.posicaoPacMan.coluna;
    
    if (jogada == 'w') {                                                              //se a jogada foi para cima...
        mapas.mapaComidaMod[pacManLinha-1][pacManColuna] = VAZIO;
        mapas.mapaGeral[pacManLinha-1][pacManColuna] = VAZIO;                                        
    } 
    
    else if (jogada == 's') {                                                          //se a jogada for para baixo...
        mapas.mapaComidaMod[pacManLinha+1][pacManColuna] = VAZIO;
        mapas.mapaGeral[pacManLinha+1][pacManColuna] = VAZIO;
    } 
    
    else if (jogada == 'a') {                                                        //se a jogada for para a esquerda...
        mapas.mapaComidaMod[pacManLinha][pacManColuna-1] = VAZIO;
        mapas.mapaGeral[pacManLinha][pacManColuna-1] = VAZIO;
    } 
    
    else if (jogada == 'd') {                                                         //se a jogada for para a direita...
        mapas.mapaComidaMod[pacManLinha][pacManColuna+1] = VAZIO;
        mapas.mapaGeral[pacManLinha][pacManColuna+1] = VAZIO;
    }

    return mapas;
}

tJogo PacManTeletransportou(tJogo jogo, char jogada, int i) {
    int pacManLinha = jogo.pacMan.posicaoPacMan.linha;               int pacManColuna = jogo.pacMan.posicaoPacMan.coluna;
    int teleporte1linha = jogo.teleporte.posicaoTeleporte1.linha;    int teleporte1coluna = jogo.teleporte.posicaoTeleporte1.coluna;
    int teleporte2linha = jogo.teleporte.posicaoTeleporte2.linha;    int teleporte2coluna = jogo.teleporte.posicaoTeleporte2.coluna;  

    if (jogada == 'w') {
        if ((pacManLinha-1 == teleporte1linha) && (pacManColuna == teleporte1coluna)) {
            jogo.pacMan.posicaoPacMan.linha = teleporte2linha ;
            jogo.pacMan.posicaoPacMan.coluna = teleporte2coluna;
            jogo.mapas.trilha[teleporte1linha][teleporte1coluna] = i;
        } else if ((pacManLinha-1 == teleporte2linha) && (pacManColuna == teleporte2coluna)) {
            jogo.pacMan.posicaoPacMan.linha = teleporte1linha;
            jogo.pacMan.posicaoPacMan.coluna = teleporte1coluna;
            jogo.mapas.trilha[teleporte2linha][teleporte2coluna] = i;
        }
    }
    else if (jogada == 's') {
        if ((pacManLinha+1 == teleporte1linha) && (pacManColuna == teleporte1coluna)) {
            jogo.pacMan.posicaoPacMan.linha = teleporte2linha;
            jogo.pacMan.posicaoPacMan.coluna = teleporte2coluna;
            jogo.mapas.trilha[teleporte1linha][teleporte1coluna] = i;
        } else if ((pacManLinha+1 == teleporte2linha) && (pacManColuna == teleporte2coluna)) {
            jogo.pacMan.posicaoPacMan.linha = teleporte1linha;
            jogo.pacMan.posicaoPacMan.coluna = teleporte1coluna;
            jogo.mapas.trilha[teleporte2linha][teleporte2coluna] = i;
        }
    }
    else if (jogada == 'a') {
        if ((pacManLinha == teleporte1linha) && (pacManColuna-1 == teleporte1coluna)) {
            jogo.pacMan.posicaoPacMan.linha = teleporte2linha;
            jogo.pacMan.posicaoPacMan.coluna = teleporte2coluna;
            jogo.mapas.trilha[teleporte1linha][teleporte1coluna] = i;
        } else if ((pacManLinha == teleporte2linha) && (pacManColuna-1 == teleporte2coluna)) {
            jogo.pacMan.posicaoPacMan.linha = teleporte1linha;
            jogo.pacMan.posicaoPacMan.coluna = teleporte1coluna;
            jogo.mapas.trilha[teleporte2linha][teleporte2coluna] = i;
        }
    }
    else if (jogada == 'd') {
        if ((pacManLinha == teleporte1linha) && (pacManColuna+1 == teleporte1coluna)) {
            jogo.pacMan.posicaoPacMan.linha = teleporte2linha;
            jogo.pacMan.posicaoPacMan.coluna = teleporte2coluna;
            jogo.mapas.trilha[teleporte1linha][teleporte1coluna] = i;
        } else if ((pacManLinha == teleporte2linha) && (pacManColuna+1 == teleporte2coluna)) {
            jogo.pacMan.posicaoPacMan.linha = teleporte1linha;
            jogo.pacMan.posicaoPacMan.coluna = teleporte1coluna;
            jogo.mapas.trilha[teleporte2linha][teleporte2coluna] = i;
        }
    }

    return jogo;
}

tJogo PacManEmCimaDoTeleporte(tJogo jogo, int i) {
    int pacManLinha = jogo.pacMan.posicaoPacMan.linha;               int pacManColuna = jogo.pacMan.posicaoPacMan.coluna;
    int teleporte1linha = jogo.teleporte.posicaoTeleporte1.linha;    int teleporte1coluna = jogo.teleporte.posicaoTeleporte1.coluna;
    int teleporte2linha = jogo.teleporte.posicaoTeleporte2.linha;    int teleporte2coluna = jogo.teleporte.posicaoTeleporte2.coluna; 

    if ((pacManLinha == teleporte1linha) && (pacManColuna == teleporte1coluna)) {
        jogo.pacMan.posicaoPacMan.linha = teleporte2linha;
        jogo.pacMan.posicaoPacMan.coluna = teleporte2coluna;
        jogo.mapas.mapaGeral[teleporte1linha][teleporte1coluna] = TELEPORTE;
        jogo.mapas.trilha[teleporte1linha][teleporte1coluna] = i; 
    } else if ((pacManLinha == teleporte2linha) && (pacManColuna == teleporte2coluna)) {
        jogo.pacMan.posicaoPacMan.linha = teleporte1linha;
        jogo.pacMan.posicaoPacMan.coluna = teleporte1coluna;
        jogo.mapas.mapaGeral[teleporte2linha][teleporte2coluna] = TELEPORTE;
        jogo.mapas.trilha[teleporte2linha][teleporte2coluna] = i;
    }

    return jogo;
}

tPosicao ExecutaMovimentoPacMan(tPosicao posicaoPacMan, char jogada) {                //a partir da jogada lida, determina a nova posicao do pacMan no mapa;

    if (jogada == 'w') {                                                      //se a jogada for para cima...
        posicaoPacMan.linha = posicaoPacMan.linha-1;
    } else if (jogada == 's') {                                               //se a jogada for para baixo...
        posicaoPacMan.linha = posicaoPacMan.linha+1;
    } else if (jogada == 'a') {                                               //se a jogada por para a esquerda...
        posicaoPacMan.coluna = posicaoPacMan.coluna-1;
    } else if (jogada == 'd') {                                               //se a jogada for para a direita...
        posicaoPacMan.coluna = posicaoPacMan.coluna+1;
    }

    return posicaoPacMan;
}

tMapa LimpaCaminhoPacMan(tMapa mapas, tPacMan pacMan, char jogada) {
    int pacManLinha = pacMan.posicaoPacMan.linha;   
    int pacManColuna = pacMan.posicaoPacMan.coluna;

    if (pacMan.vida == 0) {
        if (mapas.mapaTeleportes[pacManLinha][pacManColuna] == TELEPORTE) {
            mapas.mapaGeral[pacManLinha][pacManColuna] = TELEPORTE;
        } else {
            mapas.mapaGeral[pacManLinha][pacManColuna] = VAZIO;
        }
    } else {
        if (jogada == 'w') {
            if (mapas.mapaTeleportes[pacManLinha+1][pacManColuna] == TELEPORTE) {
                mapas.mapaGeral[pacManLinha+1][pacManColuna] = TELEPORTE;
            } else {
                mapas.mapaGeral[pacManLinha+1][pacManColuna] = VAZIO;
            }
        } 
        else if (jogada == 's') {
            if (mapas.mapaTeleportes[pacManLinha-1][pacManColuna] == TELEPORTE) {
                mapas.mapaGeral[pacManLinha-1][pacManColuna] = TELEPORTE;
            } else {
                mapas.mapaGeral[pacManLinha-1][pacManColuna] = VAZIO;
            }
        }
        else if (jogada == 'a') {
            if (mapas.mapaTeleportes[pacManLinha][pacManColuna+1] == TELEPORTE) {
                mapas.mapaGeral[pacManLinha][pacManColuna+1] = TELEPORTE;
            } else {
                mapas.mapaGeral[pacManLinha][pacManColuna+1] = VAZIO;
            }
        }
        else if (jogada == 'd') {
            if (mapas.mapaTeleportes[pacManLinha][pacManColuna-1] == TELEPORTE) {
                mapas.mapaGeral[pacManLinha][pacManColuna-1] = TELEPORTE;
            } else {
                mapas.mapaGeral[pacManLinha][pacManColuna-1] = VAZIO;
            }
        }
    }

    return mapas;
}

tMapa LimpaCaminhoPacManTeletransporte(tMapa mapas, tPacMan pacMan, tTeleportes teleporte, char jogada) {
    int pacManLinha = pacMan.posicaoPacMan.linha;               int pacManColuna = pacMan.posicaoPacMan.coluna;
    int teleporte1linha = teleporte.posicaoTeleporte1.linha;    int teleporte1coluna = teleporte.posicaoTeleporte1.coluna;
    int teleporte2linha = teleporte.posicaoTeleporte2.linha;    int teleporte2coluna = teleporte.posicaoTeleporte2.coluna;

    if (jogada == 'w') {
        if ((pacManLinha == teleporte2linha) && (pacManColuna == teleporte2coluna)) {
            mapas.mapaGeral[teleporte1linha+1][teleporte1coluna] = VAZIO;
        }
        else if ((pacManLinha == teleporte1linha) && (pacManColuna == teleporte1coluna)) {
            mapas.mapaGeral[teleporte2linha+1][teleporte2coluna] = VAZIO;
        }
    } else if (jogada == 's') {
        if ((pacManLinha == teleporte2linha) && (pacManColuna == teleporte2coluna)) {
            mapas.mapaGeral[teleporte1linha-1][teleporte1coluna] = VAZIO;
        }
        else if ((pacManLinha == teleporte1linha) && (pacManColuna == teleporte1coluna)) {
            mapas.mapaGeral[teleporte2linha-1][teleporte2coluna] = VAZIO;
        }
    } else if (jogada == 'a') {
        if ((pacManLinha == teleporte2linha) && (pacManColuna == teleporte2coluna)) {
            mapas.mapaGeral[teleporte1linha][ teleporte1coluna+1] = VAZIO;
        }
        else if ((pacManLinha == teleporte1linha) && (pacManColuna == teleporte1coluna)) {
            mapas.mapaGeral[teleporte2linha][teleporte2coluna+1] = VAZIO;
        }
    } else if (jogada == 'd') {
        if ((pacManLinha == teleporte2linha) && (pacManColuna == teleporte2coluna)) {
            mapas.mapaGeral[teleporte1linha][teleporte1coluna-1] = VAZIO;
        }
        else if ((pacManLinha == teleporte1linha) && (pacManColuna == teleporte1coluna)) {
            mapas.mapaGeral[teleporte2linha][teleporte2coluna-1] = VAZIO;
        }
    }

    return mapas;
}

tMapa AtualizaMapa(tMapa mapas, tPacMan pacMan, tFantasmas fantasmas, int linha, int coluna) {
    int fantasmaBlinha = fantasmas.fantasmaB.posicaoFantasma.linha;      int fantasmaIlinha = fantasmas.fantasmaI.posicaoFantasma.linha; 
    int fantasmaBcoluna = fantasmas.fantasmaB.posicaoFantasma.coluna;    int fantasmaIcoluna = fantasmas.fantasmaI.posicaoFantasma.coluna;
    int fantasmaClinha = fantasmas.fantasmaC.posicaoFantasma.linha;      int fantasmaPlinha = fantasmas.fantasmaP.posicaoFantasma.linha;  
    int fantasmaCcoluna = fantasmas.fantasmaC.posicaoFantasma.coluna;    int fantasmaPcoluna = fantasmas.fantasmaP.posicaoFantasma.coluna;
    int pacManLinha = pacMan.posicaoPacMan.linha;
    int pacManColuna = pacMan.posicaoPacMan.coluna;
    int i=0, j=0;

    for (i=0; i<linha; i++) {
        for (j=0; j<coluna; j++) {
            if ((pacMan.vida == 1) && (i == pacManLinha) && (j == pacManColuna)) {
                    mapas.mapaGeral[i][j] = PACMAN;
            } if ((fantasmas.fantasmaB.existe == 1) && (i == fantasmaBlinha) && (j == fantasmaBcoluna)) {
                    mapas.mapaGeral[i][j] = FANTASMA_ESQUERDA;
            } if ((fantasmas.fantasmaC.existe == 1) && (i == fantasmaClinha) && (j == fantasmaCcoluna)) {
                    mapas.mapaGeral[i][j] = FANTASMA_DIREITA;
            } if ((fantasmas.fantasmaI.existe == 1) && (i == fantasmaIlinha) && (j == fantasmaIcoluna)) {
                    mapas.mapaGeral[i][j] = FANTASMA_BAIXO;
            } if ((fantasmas.fantasmaP.existe == 1) && (i == fantasmaPlinha) && (j == fantasmaPcoluna)) {
                    mapas.mapaGeral[i][j] = FANTASMA_CIMA;
            }
        }
    }

    return mapas;
}

tMapa AtualizaMapaMod(tMapa mapas, tPacMan pacMan, tFantasmas fantasmas, int linha, int coluna) {
    int pacManLinha = pacMan.posicaoPacMan.linha;     int pacManColuna = pacMan.posicaoPacMan.coluna;
    int i=0, j=0;

    for (i=0; i<linha; i++) {
        for (j=0; j<coluna; j++) {
            if ((pacMan.vida == 1) && (i == pacManLinha) && (j == pacManColuna)) {
                mapas.mapaGeral[i][j] = PACMAN_MODIFICADO;
            }
        }
    }

    return mapas;
}

void ImprimeMapaAtual(int linha, int coluna, tMapa mapas, tPacMan pacMan, char jogada) {                          //imprime o mapa na sua situacao atual apos cada jogada e sua pontuacao acumulada ate entao;
    int i=0, j=0;

    printf("Estado do jogo apos o movimento '%c':\n", jogada);
    for (i=0; i<linha; i++) {
        for (j=0; j<coluna; j++) {
            printf("%c", mapas.mapaGeral[i][j]);
        }
        printf("\n");
    }
    printf("Pontuacao: %d\n", pacMan.pontuacao);
    printf("\n");
}

int PacManNoMapa(int linha, int coluna, tMapa mapas) {                            //verifica se o pacMan ainda ta no mapa;
    int pacManNoMapa=0, i=0, j=0;                         //inicializa a variavel pacManNoMapa zerada;          

    for (i=0; i<linha; i++) {                                          
        for (j=0; j<coluna; j++) {
            if ((mapas.mapaGeral[i][j]==PACMAN) || (mapas.mapaGeral[i][j]==PACMAN_MODIFICADO)) {        //se o pacMan estiver no mapa ainda, determina 1 para a variavel e retorna; 
                pacManNoMapa = 1;
            }
        }
    }

    return pacManNoMapa;
}

void DocumentoTrilha(tMapa mapas, int linha, int coluna, int i, char caminho[]) {                //funcao que faz a trilha pela qual o pacMan percorreu durante a partida;
    FILE *docTrilha;
    int l=0, j=0, g=0;
    char caminhoTrilha[1045];

    sprintf(caminhoTrilha, "%s/saida/trilha.txt", caminho);

    docTrilha = fopen(caminhoTrilha, "w");                               //cria ou subescreve o documento trilha.txt";

    for (l=0; l<linha; l++) {
        for (j=0; j<coluna; j++) {
            if (mapas.trilha[l][j] != -1) {                          //se o que estiver inserido nessa posicao da trilha for maior ou igual a 0...
                fprintf(docTrilha, "%d ", mapas.trilha[l][j]);      //vai imprimir o valor de i nessa posicao, indicando o caminho que o pacMan percorreu;
            } else {                                                     //mas, se o valor nessa posicao da trilha for menor que 0, ou seja, estiver igual a -1 por conta da inicializacao feita anteriormente...
                fprintf(docTrilha, "# ");                                //vai imprimir uma hashtag, que significa que o pacMan nao passou por esta posicao no mapa;
            }
        }
        fprintf(docTrilha, "\n");
    }

    fclose(docTrilha);
}

void DocumentoResumo(tPacMan pacMan, int i, char jogada, char argv[]) {                 //funcao que faz o documento de resumo;
    FILE *docResumo;
    char caminhoResumo[1045];

    sprintf(caminhoResumo, "%s/saida/resumo.txt", argv);

    docResumo = fopen(caminhoResumo, "a");                                          //cria o documento ou adiciona mais informacoes ao final se ja existir;
    if (pacMan.colidiuFantasma[i] == 1) {                                    //se o pacm colidiu com um fantasma nessa jogada, adiciona o tipo de movimento e o numero da jogada;
        fprintf(docResumo, "Movimento %d (%c) fim de jogo por encostar em um fantasma\n", i, jogada);
    }
    if (pacMan.pegouComida[i] == 1) {                                          //se o pacman consumiu uma comida nessa jogada, adiciona o tipo de movimento e o numero da jogada;
        fprintf(docResumo, "Movimento %d (%c) pegou comida\n", i, jogada);
    } 
    if (pacMan.colidiuParede[i] == 1) {                                      //se o pacman colidiu com uma parede nessa jogada, adiciona o tipo de movimento e o numero da jogada;
        fprintf(docResumo, "Movimento %d (%c) colidiu na parede\n", i, jogada);
    }
    
    fclose(docResumo);
}

void DocumentoRanking(tMovimentos movimentos, char caminho[]) {                     //funcao que faz o documento do ranking;
    FILE *docRanking;
    char caminhoRanking[1045];

    sprintf(caminhoRanking, "%s/saida/ranking.txt", caminho);

    docRanking = fopen(caminhoRanking, "w");
    int i=0, j=0;
    char movimento[4] = {'w', 'a', 'd', 's'};
    int qtdComeu[4] = {movimentos.movimentoW.comeu, movimentos.movimentoA.comeu, movimentos.movimentoD.comeu, movimentos.movimentoS.comeu};
    int qtdBateu[4] = {movimentos.movimentoW.bateu, movimentos.movimentoA.bateu, movimentos.movimentoD.bateu, movimentos.movimentoS.bateu};
    int qtd[4] = {movimentos.movimentoW.qtd, movimentos.movimentoA.qtd, movimentos.movimentoD.qtd, movimentos.movimentoS.qtd};

    for(i=0; i<4; i++) {
        for (j=i+1; j<4; j++) {
            if ((qtdComeu[j] > qtdComeu[i]) || 
                (qtdComeu[j] == qtdComeu[i]) && (qtdBateu[j] < qtdBateu[i]) || 
                (qtdComeu[j] == qtdComeu[i]) && (qtdBateu[j] == qtdBateu[i]) && (qtd[j] > qtd[i]) || 
                (qtdComeu[j] == qtdComeu[i]) && (qtdBateu[j] == qtdBateu[i]) && (qtd[j] == qtd[i]) && 
                (movimento[j] < movimento[i])) {
                
                char aux = movimento[i];         int auxComeu = qtdComeu[i];
                movimento[i] = movimento[j];    qtdComeu[i] = qtdComeu[j];     
                movimento[j] = aux;              qtdComeu[j] = auxComeu;

                int auxBateu = qtdBateu[i];       int auxQtd = qtd[i];
                qtdBateu[i] = qtdBateu[j];        qtd[i] = qtd[j];
                qtdBateu[j] = auxBateu;           qtd[j] = auxQtd;
            }
        }
    }
    
    for (i=0; i<4; i++) {
        fprintf(docRanking, "%c,%d,%d,%d\n", movimento[i], qtdComeu[i], qtdBateu[i], qtd[i]);     
    }

    fclose(docRanking);
}

void DocumentoEstatisticas(tMovimentos movimentos, char argv[]) {               //funcao que faz o documento de estatisticas das jogadas;
    FILE *docEstatis;
    int jogadasSemPonto, jogadasBateuParede;
    char caminhoEstatis[1045];

    sprintf(caminhoEstatis, "%s/saida/estatisticas.txt", argv);

    docEstatis = fopen(caminhoEstatis, "w");

    fprintf(docEstatis, "Numero de movimentos: %d\n", movimentos.qtdJogadas);          //imprime o total de movimentos feitos na jogada;       

    jogadasSemPonto = movimentos.qtdJogadas - (movimentos.movimentoA.comeu + movimentos.movimentoD.comeu + movimentos.movimentoS.comeu + movimentos.movimentoW.comeu);
    fprintf(docEstatis, "Numero de movimentos sem pontuar: %d\n", jogadasSemPonto);         //imprime a quantidade de jogadas sem pontuar, ou seja, sem consumir uma comida;
    
    jogadasBateuParede = movimentos.movimentoA.bateu + movimentos.movimentoD.bateu + movimentos.movimentoS.bateu + movimentos.movimentoW.bateu;
    fprintf(docEstatis, "Numero de colisoes com parede: %d\n", jogadasBateuParede);         //imprime a quantidade de jogadas que fizeram o pacMan colidir com uma parede;

    fprintf(docEstatis, "Numero de movimentos para baixo: %d\n", movimentos.movimentoS.qtd);        //imprime a quantidade de jogadas para baixo usadas na partida;
    fprintf(docEstatis, "Numero de movimentos para cima: %d\n", movimentos.movimentoW.qtd);         //imprime a quantidade de jogadas para cima usadas na partida;
    fprintf(docEstatis, "Numero de movimentos para esquerda: %d\n", movimentos.movimentoA.qtd);     //imprime a quantidade de jogadas para a esquerda usadas na partida;
    fprintf(docEstatis, "Numero de movimentos para direita: %d\n", movimentos.movimentoD.qtd);      //imprime a quantidade de jogadas para a direita uusadas na partida;

    fclose(docEstatis);
}

void PacManMorreu(tPacMan pacMan) {                                           //imprime a mensagem que o usuario perdeu;
    printf ("Game over!\n");
    printf ("Pontuacao final: %d\n", pacMan.pontuacao);
}

void PacManGanhou(tPacMan pacMan) {                                           //imprime a mensagem que o usuario ganhou;
    printf ("Voce venceu!\n");
    printf ("Pontuacao final: %d\n", pacMan.pontuacao);
}
