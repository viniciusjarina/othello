#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "Scene.h"

#include "StonesSpriteArray.h"
#include "Stones.h"

#include "Othelo_Engine.h"

class GameScene: public Scene
{
	StonesSpriteArray m_arrStonesSprites;
	Troll::Point m_squareMouse;
	bool m_canAddStone;

	enum { MENU, PLAYING} m_nState;

	Stones m_stones; // Stones drawer

	Troll::Image m_logoImage;
	Troll::Image m_newGame;
	Troll::Image m_quitImage;
	Troll::Image m_board;

	Tabuleiro m_tabuleiro;
	Tabuleiro * m_temp_tabuleiros;
	int m_player;

public:
	GameScene();
	~GameScene();

public:
	bool Logic();
	void Draw();

protected:
	void DrawBoard();
	void DrawStones();

	void UpdateStonesColors();
	void PutStone(int x, int y,Stone::StoneColor color);
};

#endif //__GAME_SCENE_H__