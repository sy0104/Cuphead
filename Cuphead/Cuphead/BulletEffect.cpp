#include "stdafx.h"
#include "BulletEffect.h"
#include "BmpMgr.h"


CBulletEffect::CBulletEffect()
{
}


CBulletEffect::~CBulletEffect()
{
	Release();
}

void CBulletEffect::Initialize()
{
	// 462 * 426 (9)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Bullet_Effect.bmp", L"Bullet_Effect");

	m_tInfo.iCX = 150;
	m_tInfo.iCY = 150;
	m_eGroup = GROUPID::EFFECT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Bullet_Effect";
}

int CBulletEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CBulletEffect::Late_Update()
{
}

void CBulletEffect::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Bullet_Effect");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 462, m_tFrame.iFrameScene * 426
		, 462, 426
		, RGB(40, 40, 40));
}

void CBulletEffect::Release()
{
}

void CBulletEffect::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_bDead = true;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
