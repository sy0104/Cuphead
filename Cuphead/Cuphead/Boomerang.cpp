#include "stdafx.h"
#include "Boomerang.h"
#include "BmpMgr.h"


CBoomerang::CBoomerang()
{
}


CBoomerang::~CBoomerang()
{
	Release();
}

void CBoomerang::Initialize()
{
	// 190 * 100
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Creating_Object/Boomerang.bmp", L"Boomerang");

	m_tInfo.iCX = 200;
	m_tInfo.iCY = 100;

	m_eGroup = GROUPID::OBJECT;


	m_fSpeed = 10.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Boomerang";
}

int CBoomerang::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX -= m_fSpeed;

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CBoomerang::Late_Update()
{
	if (m_tRect.right <= -200.f)
	{
		m_tInfo.fY += 200.f;
		m_fSpeed *= -1;
	}

	else if (m_tRect.left >= WINCX)
		m_bDead = true;
}

void CBoomerang::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomerang");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 190, m_tFrame.iFrameScene * 100
		, 190, 100
		, RGB(40, 40, 40));
}

void CBoomerang::Release()
{
}

void CBoomerang::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
