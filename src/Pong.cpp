#include <ctime> // for time (to seed rand)
#include <cmath> // for _copysign

#include "header.h"
#include "templates.h"
#include "Pong.h"

const int Pong::m_iMaxNumObjects = 10;

Pong::Pong(void)
	: BaseEngine(m_iMaxNumObjects)
{
	srand(time(NULL));
	// Initialise scores as zero
	memset(m_iScore, 0, sizeof(m_iScore));
}

Pong::~Pong(void)
{
}

int Pong::InitialiseObjects(void)
{
	// Record the fact we're about to change the array - so it doesn't get used elsewhere without reloading it
	DrawableObjectsChanged();
	// Destroy any existing objects
	DestroyOldObjects();

	m_ppDisplayableObjects = new DisplayableObject*[m_iMaxNumObjects];

	int iSHeight = GetScreenHeight();
	int iSWidth = GetScreenWidth();
	m_ppDisplayableObjects[0] = m_pPlayer1 = new Racket(this, iSWidth/32, iSHeight/2, 0x00ff00);
	m_ppDisplayableObjects[1] = m_pPlayer2 = new Racket(this, iSWidth-(iSWidth/32), iSHeight/2, 0x00a5ff);
	m_ppDisplayableObjects[2] = m_pBall = new Ball(this, iSWidth/2, iSHeight/2, m_pPlayer1, m_pPlayer2);

	// The last entry in the array has to be NULL, indicates that it is the end.
	m_ppDisplayableObjects[3] = NULL;

	return 0;
}

void Pong::SetupBackgroundBuffer(void)
{
	FillBackground(0x000000);

	// Draw the line across the middle
	int iSHeight = GetScreenHeight();
	int iSWidth = GetScreenWidth();
	int iIncr = iSHeight/24;
	for (int y = 0; y < iSHeight; y += iIncr) {
		DrawBackgroundLine(iSWidth/2, y, iSWidth/2, y+iSHeight/80, 0xffffff);
	}
}

void Pong::GameAction(void)
{
	// If too early to act then do nothing
	if (!TimeToAct())
		return;

	// Don't act for another 30 ticks (approx. 30fps)
	SetTimeToAct(30);

	// Make the CPU move the left paddle
	CPU_Move(m_pPlayer1);

	// Detect goal
	if (m_pBall->GetXCentre() < 0 || m_pBall->GetXCentre() > GetScreenWidth())
	{
		// 0 if left side, 1 if right
		int side = (m_pBall->GetXCentre() < 0) ? 1 : 0;
		m_iScore[side]++;
		// Set the ball as stationary in the starting position
		m_pBall->Reset();

		printf("GOAL! %d-%d\n", m_iScore[0], m_iScore[1]);
	}

	// Tell all objects to update themselves.
	// If they need the screen to redraw then they should say so, so that GameRender() will
	// call the relevant function later.
	UpdateAllObjects(GetTime());
}

void Pong::KeyDown(int iKeyCode)
{
	switch (iKeyCode)
	{
	case SDLK_w:
		m_pPlayer1->Up();
		break;
	case SDLK_s:
		m_pPlayer1->Down();
		break;
	case SDLK_UP:
		m_pPlayer2->Up();
		break;
	case SDLK_DOWN:
		m_pPlayer2->Down();
		break;
	case SDLK_SPACE:
		m_pBall->Serve();
		break;
	case SDLK_ESCAPE: // End program when escape is pressed
		// Free array beforehand
		DrawableObjectsChanged();
		DestroyOldObjects();
		SetExitWithCode(0);
		break;
	default:
		break;
	}
}

void Pong::KeyUp(int iKeyCode)
{
	switch (iKeyCode)
	{
	case SDLK_w:
		m_pPlayer1->Down();
		break;
	case SDLK_s:
		m_pPlayer1->Up();
		break;
	case SDLK_UP:
		m_pPlayer2->Down();
		break;
	case SDLK_DOWN:
		m_pPlayer2->Up();
		break;
	default:
		break;
	}
}

void Pong::DrawChanges(void)
{
	// Remove objects from their old positions
	UndrawChangingObjects();
	// Draw the text for the user
	DrawStrings();
	// Draw objects at their new positions
	DrawChangingObjects();
}

void Pong::DrawStrings(void)
{
	CopyBackgroundPixels(0, 0, GetScreenWidth(), 60);

	char str[32];
	// Render the score
	sprintf(str, "%d  %d", m_iScore[0], m_iScore[1]);
	DrawScreenString((GetScreenWidth()/2)-55, 10, str, 0xffffff, NULL);

	SetNextUpdateRect(0, 0, GetScreenWidth(), 60);
}

void Pong::CPU_Move(Racket* pRacket)
{
	int iHeight = GetScreenHeight();
	// Calculate vertical distance
	double dDelta = pRacket->GetYCentre() - m_pBall->GetYCentre();
	// Limit velocity to the maximum movement speed, but keep sign
	double dVelocity = _copysign(__min(iHeight/48.0, abs(dDelta)), dDelta);
	// Move racket closer to the ball
	pRacket->Move_ip(0, -dVelocity);
}
