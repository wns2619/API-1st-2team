#include "stdafx.h"
#include "MainGame.h"
#include "CMonster.h"

CMainGame::CMainGame()
	: m_dwTime(GetTickCount()), m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
	ZeroMemory(&rect_Collision, sizeof(RECT));
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize(void)
{
	srand((unsigned)time(nullptr));
	m_DC = GetDC(g_hWnd);

	m_ObjList[OBJ_PLAYER].push_back(CAbstractFactory<CPlayer>::Create());

	for (int i = 0; i < 10; ++i)
	{
		m_ObjList[OBJ_MONSTER].push_back(CAbstractFactory<CMonster>::Create());
	}

}

void CMainGame::Update(void)
{
	if (m_ObjList[OBJ_PLAYER].front() == nullptr)
	{
		m_ObjList[OBJ_PLAYER].push_back(CAbstractFactory<CPlayer>::Create());
	}
	list<CObj*>::iterator Monsteriter = m_ObjList[OBJ_MONSTER].begin();
	// 0번째 플레이어 상태에서, 그 다음 몬스터 얘만 순회를 돌린다.
	/*for (size_t i = OBJ_PLAYER; i < OBJ_MONSTER; ++i) {
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end();) {

			int iEvent = (*iter)->Update();*/
	CObj* pTemp = m_ObjList[OBJ_PLAYER].front();
	for (; Monsteriter != m_ObjList[OBJ_MONSTER].end();)
	{
		pTemp->Update();
		int iEvent = (*Monsteriter)->Update();

		//TODO :: 충돌 처리
		if (IntersectRect(&rect_Collision, pTemp->GetPos(), (*Monsteriter)->GetPos()))
		{
			Safe_Delete<CObj*>(*Monsteriter);
			Monsteriter = m_ObjList[OBJ_MONSTER].erase(Monsteriter);
			m_ObjList[OBJ_MONSTER].push_back(CAbstractFactory<CMonster>::Create());
		}
		else if (iEvent == OBJ_DEAD)
		{
			Safe_Delete<CObj*>(*Monsteriter);
			Monsteriter = m_ObjList[OBJ_MONSTER].erase(Monsteriter);
			m_ObjList[OBJ_MONSTER].push_back(CAbstractFactory<CMonster>::Create());
		}
		else
		{
			++Monsteriter;
		}
	}
}
			// 몬스터의 각 자의 pos 값을 받아야된다.
			// 첫 번째부터 끝까지 순회해야하는데..
			//if (i == OBJ_PLAYER  && m_ObjList[OBJ_MONSTER].size() > 0 && IntersectRect(&rect_Collision, m_ObjList[OBJ_PLAYER].front()->GetPos(), (*Monsteriter)->GetPos()))
			//{
			//	Safe_Delete(*iter);
			//	iter = m_ObjList[i].erase(iter);
			//	++Monsteriter;
			//}
			//else if (iEvent == OBJ_DEAD) {
			//	Safe_Delete<CObj*>(*iter);
			//	iter = m_ObjList[i].erase(iter);
			//	for (int i = 0; i < 1; ++i)
			//	{
			//		m_ObjList[OBJ_MONSTER].push_back(CAbstractFactory<CMonster>::Create());
			//	}
			//}
			//else {
			//	++iter;
			//}

void CMainGame::Late_Update(void)
{
	for (size_t i = OBJ_PLAYER; i < OBJ_END; ++i) {
		for (auto& iter : m_ObjList[i]) {
			iter->Late_Update();
		}
	}
}

void CMainGame::Render()
{
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount()) {
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}

	Rectangle(m_DC, 0, 0, WINCX, WINCY);
	Rectangle(m_DC, 100, 0, WINCX - 100, WINCY);
	TextOut(m_DC, WINCX - 75, 10, m_szFPS, lstrlen(m_szFPS));

	for (size_t i = OBJ_PLAYER; i < OBJ_END; ++i) {
		for (auto& iter : m_ObjList[i]) {
			iter->Render(m_DC);
		}
	}
	
}

void CMainGame::Release(void)
{
	for (size_t i = OBJ_PLAYER; i < OBJ_END; ++i) {
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), CDeleteObj());
		m_ObjList[i].clear();
	}

	ReleaseDC(g_hWnd, m_DC);
}

