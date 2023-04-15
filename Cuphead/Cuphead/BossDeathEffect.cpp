#include "stdafx.h"
#include "BossDeathEffect.h"
#include "BmpMgr.h"


CBossDeathEffect::CBossDeathEffect()
{
}


CBossDeathEffect::~CBossDeathEffect()
{
	Release();
}

void CBossDeathEffect::Initialize()
{
	// 164 * 173 (8)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Boss_Death.bmp", L"Boss_Death_Effect");

	m_tInfo.iCX = 200;
	m_tInfo.iCY = 200;
	m_eGroup = GROUPID::EFFECT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 30;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Boss_Death_Effect";
}

int CBossDeathEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CBossDeathEffect::Late_Update()
{
}

void CBossDeathEffect::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Death_Effect");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 164, m_tFrame.iFrameScene * 173
		, 164, 173
		, RGB(40, 40, 40));
}

void CBossDeathEffect::Release()
{
}

void CBossDeathEffect::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_bDead = true;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
