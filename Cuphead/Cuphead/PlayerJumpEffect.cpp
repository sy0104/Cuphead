#include "stdafx.h"
#include "PlayerJumpEffect.h"
#include "BmpMgr.h"


CPlayerJumpEffect::CPlayerJumpEffect()
{
}


CPlayerJumpEffect::~CPlayerJumpEffect()
{
	Release();
}

void CPlayerJumpEffect::Initialize()
{
	// 285 * 152 (13)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Player_Jump.bmp", L"Player_Jump_Effect");

	m_tInfo.iCX = 200;
	m_tInfo.iCY = 100;
	m_eGroup = GROUPID::EFFECT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 13;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Player_Jump_Effect";
}

int CPlayerJumpEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CPlayerJumpEffect::Late_Update()
{
}

void CPlayerJumpEffect::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Jump_Effect");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 285, m_tFrame.iFrameScene * 152
		, 285, 152
		, RGB(40, 40, 40));
}

void CPlayerJumpEffect::Release()
{
}

void CPlayerJumpEffect::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_bDead = true;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
