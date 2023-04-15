#include "stdafx.h"
#include "MFBullet.h"
#include "BmpMgr.h"
#include "ObjMgr.h"


CMFBullet::CMFBullet()
	: m_ePreScene(END), m_eCurScene(END), m_bStart(false)
{
}


CMFBullet::~CMFBullet()
{
	Release();
}

void CMFBullet::Initialize()
{
	// 20 * 25
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Firing_Seeds/Mini_Flower/MF_Bullet.bmp", L"MF_Bullet");

	// 100 * 100
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Firing_Seeds/Mini_Flower/MF_Attack_Effect.bmp", L"MF_Effect");

	m_tInfo.iCX = 20;
	m_tInfo.iCY = 25;

	m_eGroup = GROUPID::OBJECT;


	m_fSpeed = 2.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();

	m_eCurScene = CMFBullet::IDLE;
	m_ePreScene = CMFBullet::DEAD;

	m_pFrameKey = L"MF_Bullet";
}

int CMFBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	CObj* pTarget = CObjMgr::Get_Instance()->Get_Target(OBJID::PLAYER, this);

	if (!m_bStart && pTarget)
	{
		float fX = pTarget->Get_Info().fX - m_tInfo.fX;
		float fY = pTarget->Get_Info().fY - m_tInfo.fY;
		float fDia = sqrtf(fX * fX + fY * fY);

		float fRad = acosf(fX / fDia);
		m_fAngle = fRad * 180.f / PI;

		if (m_tInfo.fY < pTarget->Get_Info().fY)
			m_fAngle *= -1.f;

		m_bStart = true;
	}

	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	Scene_Change();
	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CMFBullet::Late_Update()
{
	if (m_tRect.right <= 0 || m_tRect.bottom <= 0
		|| m_tRect.left >= WINCX || m_tRect.top >= WINCY)
		m_bDead = true;
}

void CMFBullet::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	if (m_eCurScene == CMFBullet::IDLE)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 20, m_tFrame.iFrameScene * 25
			, 20, 25
			, RGB(40, 40, 40));
	}

	else if (m_eCurScene == CMFBullet::DEAD)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 100, m_tFrame.iFrameScene * 100
			, 100, 100
			, RGB(40, 40, 40));
	}
}

void CMFBullet::Release()
{
}

void CMFBullet::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		if (m_eCurScene == CMFBullet::IDLE)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				//m_pFrameKey = L"MF_Effect";
				//m_eCurScene = CMFBullet::DEAD;
				m_tFrame.iFrameStart = 0;
			}
		}

		else if (m_eCurScene == CMFBullet::DEAD)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_bDead = true;
			}
		}


		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CMFBullet::Scene_Change()
{
	if (m_eCurScene != m_ePreScene)
	{
		switch (m_eCurScene)
		{
		case CMFBullet::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"MF_Bullet";
			break;

		case CMFBullet::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"MF_Effect";
			break;
		}

		m_ePreScene = m_eCurScene;
	}

}
