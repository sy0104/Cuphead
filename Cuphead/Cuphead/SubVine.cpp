#include "stdafx.h"
#include "SubVine.h"
#include "BmpMgr.h"


CSubVine::CSubVine()
	: m_bGrowUp(false)
{
}


CSubVine::~CSubVine()
{
	Release();
}

void CSubVine::Initialize()
{
	// 115 * 363
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Final_Form/Vines/Vine_A.bmp", L"Sub_Vine");

	m_tInfo.iCX = 150;
	m_tInfo.iCY = 500;

	m_eGroup = GROUPID::OBJECT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 20;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
	m_pFrameKey = L"Sub_Vine";
}

int CSubVine::Update()
{

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CSubVine::Late_Update()
{
}

void CSubVine::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Sub_Vine");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 115, m_tFrame.iFrameScene * 363
		, 115, 363
		, RGB(40, 40, 40));

}

void CSubVine::Release()
{
}

void CSubVine::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		// 올라옴
		if (!m_bGrowUp)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_bGrowUp = true;

			m_tFrame.dwFrameTime = GetTickCount();
		}

		// 내려감
		else if (m_bGrowUp)
		{
			--m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart == 0)
				m_bDead = true;

			m_tFrame.dwFrameTime = GetTickCount();
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

