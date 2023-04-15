#include "stdafx.h"
#include "AcornBoostEffect.h"
#include "BmpMgr.h"



CAcornBoostEffect::CAcornBoostEffect()
{
}


CAcornBoostEffect::~CAcornBoostEffect()
{
	Release();
}

void CAcornBoostEffect::Initialize()
{
	// 94 * 146 (7)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Acorn_Boost.bmp", L"Acorn_Boost_Effect");

	m_tInfo.iCX = 50;
	m_tInfo.iCY = 100;
	m_eGroup = GROUPID::EFFECT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Acorn_Boost_Effect";
}

int CAcornBoostEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CAcornBoostEffect::Late_Update()
{
}

void CAcornBoostEffect::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Acorn_Boost_Effect");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 94, m_tFrame.iFrameScene * 146
		, 94, 146
		, RGB(40, 40, 40));
}

void CAcornBoostEffect::Release()
{
}

void CAcornBoostEffect::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_bDead = true;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
