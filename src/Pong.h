#pragma once
#include "BaseEngine.h"
#include "Ball.h"
#include "Racket.h"

class Pong :
	public BaseEngine
{
public:
	Pong(void);
	~Pong(void);
	int InitialiseObjects(void);
	void SetupBackgroundBuffer(void);
	void GameAction(void);
	void KeyDown(int iKeyCode);
	void KeyUp(int iKeyCode);
	void DrawChanges(void);
	void DrawStrings(void);
	void CPU_Move(Racket *pRacket);

private:
	static const int m_iMaxNumObjects;
	int m_iScore[2];
	Ball *m_pBall;
	Racket *m_pPlayer1;
	Racket *m_pPlayer2;
};
