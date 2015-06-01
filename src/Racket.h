#pragma once
#include "DisplayableObject.h"

class Racket :
	public DisplayableObject
{
public:
	Racket(BaseEngine *pEngine, int iX, int iY, unsigned int uiColour);
	~Racket(void);
	void Draw(void);
	void DoUpdate(int iCurrentTime);
	void Up(void);
	void Down(void);
	void Move_ip(int iX, int iY);

	int GetVelocity() { return m_iVelocity; }

private:
	int m_iVelocity;
	unsigned int m_uiColour;
};
