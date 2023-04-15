#include "stdafx.h"
#include "PreBeam.h"
#include "BmpMgr.h"


CPreBeam::CPreBeam()
{
}


CPreBeam::~CPreBeam()
{
	Release();
}

void CPreBeam::Initialize()
{
	// 357 * 343
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Carrot/Beam_Pre.bmp", L"Beam_Pre");

	m_tInfo.iCX = 250;
	m_tInfo.iCY = 250;

	m_eGroup = GROUPID::OBJECT;


	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.dwFrameTime = GetTickCount();
}

int CPreBeam::Update()
{
	if (m_bDead)
		return OBJ_DEAD;


	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CPreBeam::Late_Update()
{
	if (m_tRect.right <= 0 || m_tRect.bottom <= 0
		|| m_tRect.left >= WINCX || m_tRect.top >= WINCY)
		m_bDead = true;
}

void CPreBeam::Render(HDC _DC)
{
	Update_Rect();


	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Beam_Pre");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 357, m_tFrame.iFrameScene * 343
		, 357, 343
		, RGB(40, 40, 40));

}

void CPreBeam::Release()
{
}

void CPreBeam::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_bDead = true;
		
		m_tFrame.dwFrameTime = GetTickCount();
	}
}
