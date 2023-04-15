#include "stdafx.h"
#include "MainGame.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"


CMainGame::CMainGame()
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	CSoundMgr::Get_Instance()->Initialize();
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackBuffer.bmp", L"BackBuffer");


	m_DC = GetDC(g_hWnd);

	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::LOGO);
}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Key_Update();
	CScrollMgr::Get_Instance()->Scroll_Lock();
}

void CMainGame::Render()
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	HDC hBackBuffer = CBmpMgr::Get_Instance()->Find_Image(L"BackBuffer");

	BitBlt(hBackBuffer, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CSceneMgr::Get_Instance()->Render(hBackBuffer);

	BitBlt(m_DC, 0, 0, WINCX, WINCY, hBackBuffer, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_DC);

	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CSoundMgr::Destroy_Instance();

	CObjMgr::Destroy_Instance();
}
