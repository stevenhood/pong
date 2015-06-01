#pragma once
#include "DisplayableObject.h"
#include "Racket.h"

class Ball :
	public DisplayableObject
{
public:
	Ball(BaseEngine *pEngine, int iStartX, int iStartY, Racket *pPlayer1, Racket *pPlayer2);
	~Ball(void);
	void Reset(void);
	void Draw(void);
	void DoUpdate(int iCurrentTime);
	void Serve(void);

private:
	// Two-dimensional velocity vector {x,y} (0 = x, 1 = y)
	double m_adVelocity[2];
	Racket *m_pPlayer1, *m_pPlayer2;
	int m_iStartX, m_iStartY;
};
