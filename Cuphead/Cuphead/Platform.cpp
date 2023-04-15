#include "stdafx.h"
#include "Platform.h"
#include "BmpMgr.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"

CPlatform::CPlatform()
	: m_fFirstY(0.f), m_bCollision(false)
{
}


CPlatform::~CPlatform()
{
	Release();
}

void CPlatform::Initialize()
{
	// ÇÃ·§Æû - 120 * 70
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Platform/Platform.bmp", L"Platform");

	// ÇÃ·§Æû ÇÁ·ÎÆç·¯ - °¢ 110 * 35 -> ÀüÃ¼ 990 * 35
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Platform/Platform_Propellor.bmp", L"Platform_Propellor");

	// ÇÃ·§Æû ±×¸²ÀÚ - °¢ 90 * 20 -> ÀüÃ¼ 270 * 20
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Platform/Platform_Shadow.bmp", L"Platform_Shadow");


	m_tInfo.iCX = 150;
	m_tInfo.iCY = 80;

	m_fFirstY = m_tInfo.fY;

	m_fSpeed = 0.3f;

	m_eGroup = GROUPID::BACKGROUND;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 2;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
}

int CPlatform::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bCollision)
	{
		if (m_tInfo.fX > 300.f && m_tInfo.fX < 500.f)
			m_tInfo.fY -= m_fSpeed;
		else
			m_tInfo.fY += m_fSpeed;

		if (m_tInfo.fY >= m_fFirstY + 10.f || m_tInfo.fY <= m_fFirstY - 10.f)
			m_fSpeed *= -1.f;
	}

	Frame_Move();
	Update_Rect();

	//CCollisionMgr::Collision_Platform(this, CObjMgr::Get_Instance()->Get_Player());

	Update_Rect();

	return OBJ_NOEVENT;
}

void CPlatform::Late_Update()
{
}

void CPlatform::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Platform");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 120, m_tFrame.iFrameScene * 70
		, 120, 70
		, RGB(40, 40, 40));


}

void CPlatform::Release()
{
}

void CPlatform::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
