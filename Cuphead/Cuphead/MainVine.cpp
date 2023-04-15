#include "stdafx.h"
#include "MainVine.h"
#include "BmpMgr.h"


CMainVine::CMainVine()
{
}


CMainVine::~CMainVine()
{
	Release();
}

void CMainVine::Initialize()
{
	// Vine - 900 * 120
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Final_Form/Vines/Vine_Main.bmp", L"Main_Vine");

	m_tInfo.fX = 270.f;
	m_tInfo.fY = 570.f;

	m_tInfo.iCX = 1100;
	m_tInfo.iCY = 150;

	m_eGroup = GROUPID::BACKGROUND;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 22;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
	m_pFrameKey = L"Main_Vine";
}

int CMainVine::Update()
{
	
	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CMainVine::Late_Update()
{
}

void CMainVine::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Main_Vine");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 900, m_tFrame.iFrameScene * 120
		, 900, 120
		, RGB(40, 40, 40));
}

void CMainVine::Release()
{
}

void CMainVine::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameStart = 21;
			m_tFrame.dwFrameSpeed = 200;
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
