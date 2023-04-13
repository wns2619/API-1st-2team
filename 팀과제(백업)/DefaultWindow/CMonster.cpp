#include "stdafx.h"
#include "CMonster.h"

void CMonster::Initialize(void)
{
	int iRandomVal = rand() % 570;
	int iRandomSpeed = 1 + rand() % 10;
	m_tInfo = { 115.f + iRandomVal, 15.f, 20.f, 20.f };
	m_fSpeed = 1.f + iRandomSpeed;
}
//Rectangle(m_DC, 100, 0, WINCX - 100, WINCY);
int CMonster::Update(void)
{

	m_tInfo.fY += m_fSpeed;


	if (m_tRect.bottom >= WINCY)
		return OBJ_DEAD;


	CObj::Update_Rect();


	return OBJ_NOEVENT;
}

void CMonster::Late_Update(void)
{

}

void CMonster::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CMonster::Release(void)
{

}
