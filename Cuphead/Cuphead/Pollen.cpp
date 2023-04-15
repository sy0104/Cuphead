#include "stdafx.h"
#include "Pollen.h"
#include "BmpMgr.h"


CPollen::CPollen()
	: m_iColor(0), m_fSpeedY(0.f), m_fFirstY(0.f)
{
}


CPollen::~CPollen()
{
	Release();
}

void CPollen::Initialize()
{
	// Èò»ö - 50 * 50
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Final_Form/Firing_Pollen/Pollen_White.bmp", L"Pollen_White");
	// ºÐÈ«»ö - 50 * 50
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Final_Form/Firing_Pollen/Pollen_Pink.bmp", L"Pollen_Pink");


	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	m_fSpeed = 5.f;
	m_fSpeedY = 1.f;

	m_eGroup = GROUPID::OBJECT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
}

int CPollen::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX -= m_fSpeed;

	//if (m_tInfo.fY >= 305.f)
	//	m_tInfo.fY -= m_fSpeedY;
	//else if (m_tInfo.fY <= 295.f)
	//	m_tInfo.fY += m_fSpeedY;
	//else
	//	m_tInfo.fY += m_fSpeedY;

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CPollen::Late_Update()
{
	if (m_tRect.right <= 0)
		m_bDead = true;
}

void CPollen::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC;

	if (m_iColor == 1)
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Pollen_White");
	else
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Pollen_Pink");


	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 50, m_tFrame.iFrameScene * 50
		, 50, 50
		, RGB(40, 40, 40));
}

void CPollen::Release()
{
}

void CPollen::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}