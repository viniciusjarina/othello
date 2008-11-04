
#include <troll.h>

#include "GameScene.h"
#include "Stone.h"

using Troll::Point;

using Troll::Color;

using Troll::Surface;

using Troll::Key;
using Troll::KeyInput;

using Troll::MouseInput;

using Troll::Screen;

using Troll::DrawFlags;

GameScene::GameScene():
m_squareMouse(-1,-1),
m_player(1),
m_canAddStone(false),
m_nState(MENU)
{
	m_arrStonesSprites.LoadSprites();
	m_stones.InitStones(m_arrStonesSprites);
	
	AlocaTabuleiros(&m_temp_tabuleiros);
	IniciaTabuleiro(&m_tabuleiro);

	m_board.LoadImage("images/back.png");
	m_logoImage.LoadImage("images/othello.png");
	m_newGame.LoadImage("images/new_game.png");
	m_quitImage.LoadImage("images/quit.png");
	
	UpdateStonesColors();
}

GameScene::~GameScene()
{
	DesalocaTabuleiros(&m_temp_tabuleiros);
}

bool GameScene::Logic()
{
	bool quit = false;

	if(VerificaFimDeJogo(&m_tabuleiro,m_player))
	{
		IniciaTabuleiro(&m_tabuleiro);
		m_player = 1;
		UpdateStonesColors();
	}

	if(m_nState == MENU)
	{
		Point ptMouse = MouseInput::GetPosition();
		if(MouseInput::IsButtonReleased(0))
		{
			Surface & screen = Screen::GetSurface();
			const Surface & s = m_newGame.GetSurface();
			Troll::Rect rc(Point((screen.GetWidth() - s.GetWidth())/2,130),s.GetSize());
			if(rc.Contains(ptMouse))
			{
				m_nState = PLAYING;
				IniciaTabuleiro(&m_tabuleiro);
				m_player = 1;
				UpdateStonesColors();
			}
		}

		if(KeyInput::IsKeyPressed(Key::ESCAPE))
			quit = true;

		if(MouseInput::IsButtonReleased(0))
		{
			Surface & screen = Screen::GetSurface();
			const Surface & s = m_quitImage.GetSurface();
			Troll::Rect rc(Point((screen.GetWidth() - s.GetWidth())/2,190),s.GetSize());
			if(rc.Contains(ptMouse))
				quit = true;
		}

	}
	else
	{
		if(KeyInput::IsKeyPressed(Key::ESCAPE))
			m_nState = MENU;
	}
		

	if(m_nState == PLAYING && m_player == 1)
	{
		Point pt;
		pt = MouseInput::GetRelativePosition();
		
		if(pt.x != 0 || pt.y != 0) // if mouse position changed 
		{
			pt = MouseInput::GetPosition();
			m_squareMouse.x = pt.x/40;
			m_squareMouse.y = (pt.y)/40;
			m_canAddStone = PodeMoverPedra(m_squareMouse.x + m_squareMouse.y*8,&m_tabuleiro,m_player) != 0;
		}

		if(MouseInput::IsButtonPressed(0))
		{
			if(m_canAddStone)
			{
				Stone::StoneColor color;

				if(m_player == 1)
					color = Stone::BLACK;
				else
					color = Stone::WHITE;

				PutStone(m_squareMouse.x,m_squareMouse.y,color);

				MovePedra(m_squareMouse.x + m_squareMouse.y*8,&m_tabuleiro,&m_player);
				
				UpdateStonesColors();
			}
			
		}
	}
	else 
	{
		if(m_stones.AnimationDone())
		{
			int	pos = -1, n1, n2, ntot,	nply = 6;
			int ret;
			int pontos;

			ContaTotalPedras(&m_tabuleiro,&n1, &n2);/* Conta o total de peÁas no tabuleiro */
			
			ntot = n1 + n2 + nply;
				
			if (ntot > 50) /* Caso haja mais pedras, aumenta o nÌvel de recurs„o */
				nply++;
			if (ntot > 52)
				nply++;
			if (ntot > 54)
				nply++;
			if (ntot > 56)
				nply++;
			if (ntot > 58)
				nply++;
			if (nply > 20)
				nply = 20;			
			
			CopiaTabuleiro(m_temp_tabuleiros,&m_tabuleiro);		
			
			/* Deixa o computador,verificar melhor jogada */
			
			ret = EncontreMelhorPosicao(m_temp_tabuleiros, nply, m_player, &pontos, &pos,0, -OTH_INFINITO, OTH_INFINITO);

			if(ret != -1 && pos >= 0 && pos < 64)
			{
				int x = pos%8;
				int y = pos/8;

				Stone::StoneColor color;

				if(m_player == 1)
					color = Stone::BLACK;
				else
					color = Stone::WHITE;
				
				PutStone(x,y,color);

				MovePedra(pos,&m_tabuleiro,&m_player);

				UpdateStonesColors();
			}
		}
	}

	m_stones.Logic();

	
	return quit;
}

void GameScene::Draw()
{
	Surface & screen = Screen::GetSurface();
	
	DrawBoard();
	DrawStones();

	if(m_nState != PLAYING)
	{
		Troll::Graphics g(screen);
		g.DrawRectFill(Troll::Rect(screen.GetSize()),Color(0,0,120,160));

		const Surface & sLogo = m_logoImage.GetSurface();
		screen.Draw(sLogo,Point((screen.GetWidth() - sLogo.GetWidth())/2,20));

		const Surface & sNewGame = m_newGame.GetSurface();
		screen.Draw(sNewGame,Point((screen.GetWidth() - sNewGame.GetWidth())/2,130),DrawFlags::none, 192);

		const Surface & sQuitGame = m_quitImage.GetSurface();
		screen.Draw(sQuitGame,Point((screen.GetWidth() - sNewGame.GetWidth())/2,190),DrawFlags::none, 192);
	}
}


void GameScene::DrawBoard()
{
	Surface & screen = Screen::GetSurface();

	screen.DrawFast(m_board.GetSurface(),Point(0,0));

	Troll::Graphics g(screen);

	g.EnableAntiAlias(true);

	for(int i = 0; i < 8; i++)
	{
		g.DrawLine(Point(i*40,0), Point(i*40,screen.GetHeight()),Color(0,70,0));
		g.DrawLine(Point(0,i*40), Point(screen.GetWidth(),i*40),Color(0,70,0));
	}

	Troll::Rect rc(m_squareMouse.x*40 + 1,m_squareMouse.y*40 + 1,38,38);

	if(m_canAddStone)
		g.DrawRoundRectFill(rc,5,Color(128,255,128,128));
	else
		g.DrawRoundRectFill(rc,5,Color(255,128,128,128));

	

}

void GameScene::DrawStones()
{
	Surface & screen = Screen::GetSurface();

	for(int y = 0; y < 8; y++)
	{
		
		for(int x = 0; x < 8; x++)
		{
			if (get(m_tabuleiro.p1, x + y*8) || get(m_tabuleiro.p2, x + y*8))
			{
				const Stone & stone = m_stones.GetStone(x,y);
				stone.Draw(screen,Point(x*40,y*40));
			}
		}
    }
}

void GameScene::PutStone(int x, int y,Stone::StoneColor color)
{
	Stone & stone = m_stones.GetStone(x,y);
	stone.SetColorNow(color);
}

void GameScene::UpdateStonesColors()
{
	int nCount = 0;
	for(int y = 0; y < 8; y++)
	{		
		for(int x = 0; x < 8; x++)
		{
			Stone & stone = m_stones.GetStone(x,y);

			if (get(m_tabuleiro.p1, x + y*8) && stone.GetColor() != Stone::WHITE)
			{
				nCount = nCount + 7;
				stone.SetColor(Stone::WHITE,nCount);
			}
			if (get(m_tabuleiro.p2, x + y*8) && stone.GetColor() != Stone::BLACK)
			{
				nCount = nCount + 7;
				stone.SetColor(Stone::BLACK,nCount);
			}
		}
    }
}

