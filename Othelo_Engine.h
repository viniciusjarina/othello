/*  EP4 - Alunos: Marcus Vinicius  TIA : 30540445 */
/*				  Vinicius Jarina  TIA : 30519527 */

#ifdef _MSC_VER
	typedef unsigned __int64 uint64_t;
#else
	#include <stdint.h>
#endif

#define set(var, pos)   ((var) |= ((uint64_t)1 << (pos)))
#define get(var, pos)   ((var) & ((uint64_t)1 << (pos)))

#define BONUS_PEDRA  10
#define OTH_INFINITO 10000000

#define POSICAO_NORMAL  0					/* flag de movimento normal */
#define	POSICAO_GANHA    OTH_INFINITO/2     /* jogador ganhou o jogo    */   
#define	POSICAO_PERDE   -OTH_INFINITO/2     /* jogador perdeu o jogo    */

#define CSE (uint64_t) 1 << 0  /* canto superior esquerdo */
#define CSD (uint64_t) 1 << 7  /* canto superios direito  */
#define CIE (uint64_t) 1 << 56 /* canto inferior esquerdo */
#define CID (uint64_t) 1 << 63 /* canto inferior direito  */

#ifdef __cplusplus
	extern "C"
	{
#endif

typedef struct _Tabuleiro
{
   struct _Tabuleiro *proximo;
    
    uint64_t p1; /* "matriz" do jogador 1 */
    uint64_t p2; /* "matriz" do jogador 2 */
    int pontos_1;/* pontos do jogador 1   */
    int pontos_2;/* pontos do jogador 2   */
}Tabuleiro;

int EncontreMelhorPosicao(Tabuleiro *b, int ply, int jogador, int * pontos,int  * pos, int sem_movimento, int pontuacao_minima, int pontuacao_maxima);

void CopiaTabuleiro(Tabuleiro * ptabuleiro,const Tabuleiro * outro); /* Copia um tabuleiro para o outro */
void IniciaTabuleiro(Tabuleiro * ptabuleiro);						 /* Inicia as posicoes iniciais     */

int AdicionaPedra(Tabuleiro * ptabuleiro,int jogador, int pos);     /* Tenta colocar uma pedra, e retorna se é válido ou não */
int ContaTotalMovimentos(const Tabuleiro * ptabuleiro,int jogador); /* Conta o número possível de movimentos de um dado jogador fazer */
int ContaPontuacao(const Tabuleiro * ptabuleiro);                   /* Calcula pontuação do tabuleiro  */
void ContaTotalPedras(Tabuleiro * ptabuleiro,int *um, int *dois);   /* Conta o número de pedras dos jogadores */

void AlocaTabuleiros(Tabuleiro ** pTabuleiro);		/* Aloca uma lista ligada de tabuleiros para lógica da máquina */
void DesalocaTabuleiros(Tabuleiro ** pTabuleiro);	/* Desaloca a lista */

int PegaMovimentoComputador(Tabuleiro * ptabuleiro,int jogador,Tabuleiro *ptemp,int level);/* Determina qual movimento fazer ( onde colocar a pedra no tabuleiro )*/
int MovePedra(int pos,Tabuleiro * ptabuleiro,int * jogador);/* Faz o movimento da pedra*/
int PodeMoverPedra(int pos,const Tabuleiro * ptabuleiro,int njogador);
int VerificaFimDeJogo(Tabuleiro * b, int jogador);
#ifdef __cplusplus
	} /* extern "C" */
#endif