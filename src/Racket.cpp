#include "header.h"
#include "templates.h"
#include "Racket.h"

Racket::Racket(BaseEngine *pEngine, int iX, int iY, unsigned int uiColour)
	: DisplayableObject(pEngine)
{
	m_iCurrentScreenX = m_iPreviousScreenX = iX;
	m_iCurrentScreenY = m_iPreviousScreenY = iY;

	m_iDrawWidth = GetEngine()->GetScreenWidth()/80;
	m_iDrawHeight = GetEngine()->GetScreenHeight()/12;

	m_iVelocity = 0;
	m_uiColour = uiColour;

	SetVisible(true);
}

Racket::~Racket(void)
{
}

void Racket::Draw(void)
{
	GetEngine()->DrawScreenRectangle(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		m_uiColour);

	StoreLastScreenPositionAndUpdateRect();
}

void Racket::DoUpdate(int iCurrentTime)
{
	m_iCurrentScreenY += m_iVelocity;

	// Prevent the racket from moving off the top and bottom of the screen
	if (m_iCurrentScreenY < 0) {
		m_iCurrentScreenY = 0;
	} else if (m_iCurrentScreenY >= GetEngine()->GetScreenHeight() - m_iDrawHeight) {
		m_iCurrentScreenY = GetEngine()->GetScreenHeight() - m_iDrawHeight;
	}
}

void Racket::Up(void)
{
	m_iVelocity -= GetEngine()->GetScreenHeight()/48;
}

void Racket::Down(void)
{
	m_iVelocity += GetEngine()->GetScreenHeight()/48;
}

// Move the object in-place
void Racket::Move_ip(int iX, int iY)
{
	m_iCurrentScreenX += iX;
	m_iCurrentScreenY += iY;
}
