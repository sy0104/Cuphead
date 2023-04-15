#include "stdafx.h"
#include "PlayerHitEffect.h"
#include "BmpMgr.h"


CPlayerHitEffect::CPlayerHitEffect()
{
}


CPlayerHitEffect::~CPlayerHitEffect()
{
	Release();
}

void CPlayerHitEffect::Initialize()
{
	// 383 * 388 (15)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Player_Hit.bmp", L"Player_Hit_Effect");

	m_tInfo.iCX = 200;
	m_tInfo.iCY = 200;
	m_eGroup = GROUPID::EFFECT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 15;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 30;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Player_Hit_Effect";
}

int CPlayerHitEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CPlayerHitEffect::Late_Update()
{
}

void CPlayerHitEffect::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Hit_Effect");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 383, m_tFrame.iFrameScene * 388
		, 383, 388
		, RGB(40, 40, 40));
}

void CPlayerHitEffect::Release()
{
}

void CPlayerHitEffect::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_bDead = true;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
