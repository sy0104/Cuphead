#include "stdafx.h"
#include "CagneyCreateEffect.h"
#include "BmpMgr.h"


CCagneyCreateEffect::CCagneyCreateEffect()
{
}


CCagneyCreateEffect::~CCagneyCreateEffect()
{
	Release();
}

void CCagneyCreateEffect::Initialize()
{
	// 178 * 456 (14)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Cagney_Create_Effect.bmp", L"Cagney_Create_Effect");

	m_tInfo.iCX = 100;
	m_tInfo.iCY = 370;
	m_eGroup = GROUPID::EFFECT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 14;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Cagney_Create_Effect";
}

int CCagneyCreateEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CCagneyCreateEffect::Late_Update()
{
}

void CCagneyCreateEffect::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cagney_Create_Effect");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 178, m_tFrame.iFrameScene * 456
		, 178, 456
		, RGB(40, 40, 40));
}

void CCagneyCreateEffect::Release()
{
}

void CCagneyCreateEffect::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_bDead = true;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
