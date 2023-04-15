#include "stdafx.h"
#include "Bravo.h"
#include "BmpMgr.h"


CBravo::CBravo()
{
}


CBravo::~CBravo()
{
	Release();
}

void CBravo::Initialize()
{
	// 1280 * 720 (26)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Bravo.bmp", L"Bravo");

	m_tInfo.iCX = 800;
	m_tInfo.iCY = 400;

	m_eGroup = GROUPID::EFFECT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 23;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Bravo";
}

int CBravo::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CBravo::Late_Update()
{
}

void CBravo::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Bravo");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 1280, m_tFrame.iFrameScene * 720
		, 1280, 720
		, RGB(0, 0, 0));
}

void CBravo::Release()
{
}

void CBravo::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_bDead = true;
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
