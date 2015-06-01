#define _USE_MATH_DEFINES // for M_PI
#include <cmath> // for cos, sin

#include "header.h"
#include "templates.h"
#include "Ball.h"
#include "utils.h"

Ball::Ball(BaseEngine *pEngine, int iStartX, int iStartY, Racket *pPlayer1, Racket *pPlayer2)
	: DisplayableObject(pEngine)
	, m_iStartX(iStartX)
	, m_iStartY(iStartY)
{
	Reset();

	int iBallDiam = GetEngine()->GetScreenHeight()/40;
	m_iDrawWidth = iBallDiam;
	m_iDrawHeight = iBallDiam;

	SetVisible(true);

	// Store Rackets for collision check
	m_pPlayer1 = pPlayer1;
	m_pPlayer2 = pPlayer2;
}

Ball::~Ball(void)
{
}

// Sets the ball as stationary in the starting position
void Ball::Reset(void)
{
	m_iCurrentScreenX = m_iPreviousScreenX = m_iStartX;
	m_iCurrentScreenY = m_iPreviousScreenY = m_iStartY;
	// Set velocity vector to zero
	memset(m_adVelocity, 0, sizeof(m_adVelocity));
}

void Ball::Draw(void)
{
	GetEngine()->DrawScreenOval(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0xffffff);

	// This will store the position at which the object was drawn
	// so that the background can be drawn over the top.
	// This will then remove the object from the screen.
	StoreLastScreenPositionAndUpdateRect();
}

void Ball::DoUpdate(int iCurrentTime)
{
	m_iCurrentScreenX += m_adVelocity[0];
	m_iCurrentScreenY += m_adVelocity[1];

	// Bounce the ball off the top or bottom screen border
	if (m_iCurrentScreenY < 0) {
		// Change movement to opposite direction
		m_adVelocity[1] *= -1;
		m_iCurrentScreenY = 1;
	} else if (m_iCurrentScreenY >= GetEngine()->GetScreenHeight() - m_iDrawHeight) {
		m_adVelocity[1] *= -1;
		m_iCurrentScreenY = GetEngine()->GetScreenHeight() - m_iDrawHeight;
	}

	// Detect collision with rackets
	if (IsCollideRect(this, m_pPlayer1)) {
		// Bounce ball off racket
		m_adVelocity[0] *= -1;
		m_iCurrentScreenX += m_adVelocity[0];
		// Pass some racket velocity to the ball
		m_adVelocity[1] += 0.5 * m_pPlayer1->GetVelocity();
	} else if (IsCollideRect(this, m_pPlayer2)) {
		m_adVelocity[0] *= -1;
		m_iCurrentScreenX += m_adVelocity[0];
		m_adVelocity[1] += 0.5 * m_pPlayer2->GetVelocity();
	}

	// Goal detection is in Pong.cpp

	RedrawObjects();
}

void Ball::Serve(void)
{
	if (m_adVelocity[0] != 0)
		return;

	// Random angle in radians (between 0 and 60 degrees)
	double dAngle = RandDouble(0, M_PI/3.0);
	dAngle *= RandParity();

	// Choose serving side randomly
	int iSide = RandParity();
	int iHeight = GetEngine()->GetScreenHeight();
	m_adVelocity[0] = iSide * iHeight/48.0 * cos(dAngle);
	m_adVelocity[1] = iHeight/48.0 * sin(dAngle);
}
