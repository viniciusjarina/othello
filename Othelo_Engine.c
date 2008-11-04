
#include <stdlib.h>

#include "Othelo_Engine.h"

int VerificaFimDeJogo(Tabuleiro * b, int jogador)
{
	if (!ContaTotalMovimentos(b,jogador)) /* Se não há moviventos válidos */
    {
		if (!ContaTotalMovimentos(b,3 - jogador)) /* Verifica se o outro jogador tem movimentos válidos */
			return 2;/* Se nenhum dos dois têm movimentos válidos, o jogo acabou =( */
		return 1;    /* Se não é a vez do outro .(eh-eh) */
    }
    return 0; /* O jogador pode mover normalmente */
}    

int PegaMovimentoComputador(Tabuleiro * ptabuleiro,int jogador,Tabuleiro *ptemp,int level) /* Determina qual movimento fazer ( onde colocar a pedra no tabuleiro )*/
{
    int	pos = -1, n1, n2, ntot;
	int ret;
	int pontos;
	
   	ContaTotalPedras(ptabuleiro,&n1, &n2);/* Conta o total de peças no tabuleiro */
	ntot = n1 + n2 + level;
	
	if (ntot > 50) /* Caso haja mais pedras, aumenta o nível de recursão */
		level++;
	if (ntot > 52)
		level++;
	if (ntot > 54)
		level++;
	if (ntot > 56)
		level++;
	if (ntot > 58)
		level++;
				
	if (level > 20)
		level = 20;
	
	CopiaTabuleiro(ptemp,ptabuleiro);
	
	/* Deixa o computador,verificar melhor jogada */
	ret = EncontreMelhorPosicao(ptemp, level, jogador, &pontos, &pos,0, -OTH_INFINITO, OTH_INFINITO);
	
	if(ret == -1)
		return -1;
	if (pos < 0 || pos > 63)
		return -1;
	return pos;
}


int MovePedra(int pos,Tabuleiro * ptabuleiro,int * jogador)/* Faz o movimento da pedra */
{
    Tabuleiro test;
    int nflips, r;
    
    CopiaTabuleiro(&test,ptabuleiro);
    
    nflips = AdicionaPedra(&test,*jogador, pos);/* Tenta adicionar a pedra no tabuleiro test */
   
    if(nflips > 0)/* Só faz o movimento se for possível */
    {
		CopiaTabuleiro(ptabuleiro,&test);
		*jogador = 3 - *jogador;			/* Troca o jogador 1->2, 2->1 */
		
		r = VerificaFimDeJogo(ptabuleiro, *jogador); /*Verifica se o jogo acabou*/
		if (r == 2) /* Ninguém mais pode mover nenhuma pedra */
			return -1;
		if (r == 1) /* O próximo jogador não tem mais jogadas, intão volta pro anterior */
			*jogador = 3 - *jogador;
		return 1;
    }
    return 0;
}

void CopiaTabuleiro(Tabuleiro * ptabuleiro,const Tabuleiro * other)
{
    ptabuleiro->p1 = other->p1;
    ptabuleiro->p2 = other->p2;
    ptabuleiro->pontos_1 = other->pontos_1;
    ptabuleiro->pontos_2 = other->pontos_2;
}

void IniciaTabuleiro( Tabuleiro *ptabuleiro)
{
	ptabuleiro->proximo = NULL;
    ptabuleiro->p1 = ptabuleiro->p2 = 0;
    ptabuleiro->pontos_1 = ptabuleiro->pontos_2 = 2;
	
    set(ptabuleiro->p1,28); /* Coloca as 4 pedras iniciais */
    set(ptabuleiro->p1,35);
    set(ptabuleiro->p2,27);
    set(ptabuleiro->p2,36);
}

int PodeMoverPedra(int pos,const Tabuleiro * ptabuleiro,int njogador)
{
	Tabuleiro test;
    int nflips;
    
    CopiaTabuleiro(&test,ptabuleiro);
    
    nflips = AdicionaPedra(&test,njogador, pos);/* Verifica numero de movimentos */

	return nflips > 0;  
}

int AdicionaPedra(Tabuleiro *ptabuleiro,int jogador, int p)
{
	uint64_t *inimigo;
    uint64_t *amigo;
    int *pontos_inimigo;
    int *pontos_amigo;
    uint64_t pedras_invertidas;
    int	total_invertida = 0,invertidas,xp,yp,tp;
    
    int y = p / 8;
    int x = p - y * 8;        
	
	int di,dj;
         
    if (get(ptabuleiro->p1, p) || get(ptabuleiro->p2, p)) /* Se já tiver uma pedra do jogador 1 ou 2 */
    	return -1;
    	
   	if (jogador == 1)
    {
		inimigo = &ptabuleiro->p2;
		amigo	  = &ptabuleiro->p1;
		pontos_inimigo = &ptabuleiro->pontos_2;
		pontos_amigo   = &ptabuleiro->pontos_1;
    }
    else
    {
		inimigo = &ptabuleiro->p1;
		amigo   = &ptabuleiro->p2;
		pontos_inimigo = &ptabuleiro->pontos_1;
		pontos_amigo   = &ptabuleiro->pontos_2;
    }
        
    for (di = -1; di < 2; di++) /* testa todas as direções */
    {
		for (dj = -1; dj < 2; dj++)
		{
			
			if (!(di || dj))
				continue;
			
			invertidas = 0;			/* enquanto não encontras as bordas to tabuleiro */
			pedras_invertidas = 0;  /* conta o número de pedras inimigas */
			xp = x + di;			/* e marque-as como invertidas		 */
			yp = y + dj;
			
			while (xp >= 0 && xp < 8 && yp >= 0 && yp < 8)
			{
				tp = yp * 8 + xp;/* pega a posição */
				
				if (get(*inimigo, tp))
				{
					set(pedras_invertidas, tp); /* se tiver pedra inimiga,marca como invertida */
					invertidas++;
				}
				else
				{
					if (invertidas && get(*amigo, tp)) /* verifica se é uma pedra nossa    */
					{
						ptabuleiro->p1 ^= pedras_invertidas;
						ptabuleiro->p2 ^= pedras_invertidas;
						total_invertida += invertidas; /* conta o total de pedras invertidas */
					}
					break;
				}
				xp += di;
				yp += dj;
			}
		}
    }
	
    if (total_invertida) /* Após verificar todas as direções. total_flipped contém o número de pedras que foram invertidas.*/
    {  
		set(*amigo, p);						    /* marca a pedra da jogada */
		*pontos_amigo   += total_invertida + 1; /* computa o novo placar */
		*pontos_inimigo -= total_invertida;
    }
    return total_invertida;
}

int ContaTotalMovimentos(const Tabuleiro *ptabuleiro,int jogador)
{
    Tabuleiro tmp;
    int nr_movimentos = 0, pos;
    
    for (pos = 0; pos < 64; pos++)
    {
		CopiaTabuleiro(&tmp,ptabuleiro);/* Copia pro tabuleiro temporario */
		if (AdicionaPedra(&tmp,jogador, pos) > 0) /* Se é possível adicionar mais uma pedra */
			nr_movimentos++;
    }
    return nr_movimentos;
}

void ContaTotalPedras(Tabuleiro * ptabuleiro,int *um, int *dois)/* Conta o número de pedras de cada jogador */
{
    *um = ptabuleiro->pontos_1;
    *dois = ptabuleiro->pontos_2;
}

/* Computa uma pontuãção para o tabuleiro,
   usando um ponto diferente para cada casa do tabuleiro

 - verifica se ha controle das pedras vizinhas,modificando a pontuação
 - detectando que casas vazias ja pertencem ao jogador da casa vizinha

   No tabuleiro de pontos,pontos positivos são bons (por exemplo os cantos).Pontos Negativos são ruins ( os antepenultios )
   O computador vai favorecer movimentos que tenha o meior numero de pedras 

  jogador 1 produzira resultados positivos 
  jogador 2 produzira resultados negativos
*/
int ContaPontuacao(const Tabuleiro * ptabuleiro)
{
	static int pontos[64] =
	{   /* pontos de cada casa */
		100, -30,  15,  15,  15,  15, -30, 100,
		-30, -50, -20, -20, -20, -20, -50, -30,
		15,  -20,   0,   0,   0,   0, -20,  15,
		15,  -20,   0,   0,   0,   0, -20,  15,
		15,  -20,   0,   0,   0,   0, -20,  15,
		15,  -20,   0,   0,   0,   0, -20,  15,
		-30, -50, -20, -20, -20, -20, -50, -30,
		100, -30,  15,  15,  15,  15, -30, 100
	};
	
	static int pesos[64] = /* pessos das casas em porcentagem */
	{
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0,10,10,10,
		10,10,20,20,20,20,20,20,
		30,40,60,70,80,90,90,100
	};
	
    int pos_pontos = 0;
    int ponto_pedra = (ptabuleiro->pontos_1 - ptabuleiro->pontos_2) * BONUS_PEDRA;
    int peso_pedra = pesos[ptabuleiro->pontos_1 + ptabuleiro->pontos_2];
	int i;
	    
    for (i = 0; i < 64; i++) /* faz a contagem das casa de acordo com a posição */
    {
		if (get(ptabuleiro->p1, i)) /* Se a pedra for do jogador 1 */
		{
			if (((i == 1 || i == 8 || i == 9) && (ptabuleiro->p1 & CSE))	||
				((i == 6 || i == 14 || i == 15) && (ptabuleiro->p1 & CSD))	||
				((i == 48 || i == 49 || i == 57) && (ptabuleiro->p1 & CIE))	||
				((i == 54 || i == 55 || i == 62) && (ptabuleiro->p1 & CID))	)
			{
				pos_pontos -= pontos[i];/* se estiver em alguns dos cantos,subtrai a pontuacao */
			}
			else
			{
				pos_pontos += pontos[i];/* Se não considera normalmente */
			}
		}
		if (get(ptabuleiro->p2, i)) /* Se a pedra for do jogador 2 */
		{
            if (((i == 1  || i == 8  || i == 9)  && (ptabuleiro->p2 & CSE))	||
				((i == 6  || i == 14 || i == 15) && (ptabuleiro->p2 & CSD)) ||
				((i == 48 || i == 49 || i == 57) && (ptabuleiro->p2 & CIE)) ||
				((i == 54 || i == 55 || i == 62) && (ptabuleiro->p2 & CID))	)
			{
				pos_pontos += pontos[i];/* se estiver em alguns dos cantos,soma a pontuacao */
			}
			else
			{
				pos_pontos -= pontos[i];/* Se não considera normalmente */
			}
		}
    }
    return (pos_pontos*(100-peso_pedra) + ponto_pedra*peso_pedra) / 100;
}


void AlocaTabuleiros(Tabuleiro ** pTabuleiros)/* Aloca lista ligada de tabuleiros */
{	
	int i;
	Tabuleiro * tmp;
	
	*pTabuleiros = 0;
	
    for (i = 0; i <= 20; i++)
    {
		tmp = (Tabuleiro*)malloc(sizeof(Tabuleiro));
		tmp->p1 = tmp->p2 = tmp->pontos_1 = tmp->pontos_2 = 0;
		tmp->proximo = *pTabuleiros;
		*pTabuleiros = tmp;
    }
}


void DesalocaTabuleiros(Tabuleiro ** ppTabuleiros) /* Libera lista alocada previamente */
{
	Tabuleiro * tmp;
	while(*ppTabuleiros)
	{
		tmp = (*ppTabuleiros)->proximo;
		free(*ppTabuleiros);
		*ppTabuleiros = tmp;
	}
}

/* Conta o numero de pedras e determina quem sera o ganhador ou o perdedor	*/
int VericaVencedor(Tabuleiro *b, int jogador)
{
    int	um,dois;
	   
    ContaTotalPedras(b,&um, &dois);
    if ((jogador == 1 && um > dois) || (jogador == 2 && dois > um))
		return POSICAO_GANHA + ((jogador == 1) ? um : dois) * 100;
    if ((jogador == 1 && um < dois) || (jogador == 2 && dois < um))
		return POSICAO_PERDE - ((jogador == 1) ? dois : um) * 100;
    return POSICAO_NORMAL;
}

/* Avalia o tabuleiro, e determina a melhor jogada para fazer
 
 Para cada jogada possivel é avaliado um novo tabuleiroe selecionado o 
 a melhor jogada.Depois é feita a mesma coisa recursivamente,só que para o
 outro jogador.

 Cada jogada ganha um ponto baseada no numero de pedras ganhas,e essa
 pontuacao é passada adiante recursivamente para se encontrar uma boa jogada
 */
int EncontreMelhorPosicao(Tabuleiro *b, int ply, int jogador, int * pontos, int * pos,int sem_movimento, int pontuacao_minima, int pontuacao_maxima)
{
	Tabuleiro *b1 = b;
    Tabuleiro *b2;
	
    int	melhor_pontuacao = 0,melhos_pos = 0,fr = 0,ret = 0,init = 0;
	int i;
 
    *pos    = -1;
    *pontos =  0;
	
    if (ply == 0) /* Se o nível de iteração chegou a zero */
    {
		*pontos = ContaPontuacao(b1); /* Computa a pontução do tabuleiro corrente */
		
		if (jogador == 1)  /* Se for o jogador 1 */
			*pontos *= -1; /* Inverte a pontuação */
				
		return POSICAO_NORMAL;
    }
    
    b2 = b1->proximo; /* Pega o próximo tabuleiro da lista */
    if (b2 == NULL)
    {
		/* Se ply != 0 e não tem mais tabuleiros na lista ligada, alguém está chamado EncontreMelhorPosicao errado */
		return -1;		
    }
    
    /* percorre por todas as posições */
    for (i = 0; i < 65; i++)
    {
		/* Copia o tabuleiro original do jogo */
		CopiaTabuleiro(b2,b1);
		
		if (i < 64)
		{
            /* Verifica se existe o movimento */
			if (AdicionaPedra(b2,jogador, i) <= 0)
				continue;
			else
				fr++;
	
			/* Verifica qual pe melhor jogada para o outro jogador (jogada futura) */
			ret = EncontreMelhorPosicao(b2, ply - 1, 3 - jogador, pontos, pos, 0 , -pontuacao_maxima, -pontuacao_minima);
		}
		else
		{
			/* Após varrer todas as posições */
			/* Se já foi encontrada movimentos válidos,não continua */
			if (fr)
				break;

			/*Senão verifica se foi passado a impossibilidade de movimento 
			  para parar a avaliação */
            if (sem_movimento)
			{
				/* Fim de jogo,não há mais jogada a se fazer verifica vencedor e sai */
				*pontos = VericaVencedor(b2, jogador);
			}
			else
			{	
				/* Um sem_movimentor permitiro, logo o próximo jogador ainda pode jogar,
				   não diminui o nível de recursão, ao menos que já tenhamos tentado todos movimentos */
				/* Então verifica melhor posição, e diz que não temos mais movimentos possíveis */
				ret = EncontreMelhorPosicao(b2, ply, 3 - jogador, pontos, pos,1, -pontuacao_maxima, -pontuacao_minima);
			}
		}
		/* Compara a pontuacao atual com a melhor das pontuações,e opcionalmente,muda-a para
		   o movimento atual (caso seja melhor).
		   A primeiro movimento pode ser o melhor,mas pode ser utilizado o segundo..aleatoriamente
		   para que nem todas as jogadas sejam iguais */
		
		if (!init
			|| *pontos > melhor_pontuacao
			|| (*pontos == melhor_pontuacao && rand()%100 > 30 ))
		{
			melhor_pontuacao = *pontos;    // save score
			init = 1;              // we had a valid move
			
			/* Caso i seja 64,não poderemos mover,mas pos ainda tera uma jogada valida do EncontreMelhorPosicao
			   anterior		*/
			
			if (i == 64)
			{
				melhos_pos = *pos; /* Pega a jogada anterior */
			}
			else
			{
				melhos_pos = i;    /* Senão pega a jogada atual */
			}
		}
	
		/* Caso a pontuãção atual seja maior do que a pontuação mínima,marque a minima como a atual */
		if (*pontos > pontuacao_minima)
			pontuacao_minima = *pontos;
		
		/* Caso a pontuacao atual seja maior do que a máxima,saímos do looping*/
		if (*pontos > pontuacao_maxima)
			break;
    } 
    *pontos = -1 * melhor_pontuacao; /*Inverte a pontuação   */
    *pos = melhos_pos; /* Salva a melhor posição, para jogar */
	
    return POSICAO_NORMAL;
}
