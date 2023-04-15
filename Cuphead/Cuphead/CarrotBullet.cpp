#include "stdafx.h"
#include "CarrotBullet.h"
#include "ObjMgr.h"
#include "BmpMgr.h"


CCarrotBullet::CCarrotBullet()
	: m_ePreScene(END), m_eCurScene(END)
{
}


CCarrotBullet::~CCarrotBullet()
{
	Release();
}

void CCarrotBullet::Initialize()
{
	// Carrot_Bullet - 58 * 97
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Carrot/Carrot_Bullet.bmp", L"Carrot_Bullet");
	// Carrot Death - 100 * 100
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Carrot/Carrot_Death_Effects.bmp", L"Death");
	// 200 * 200
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Carrot/Rotation.bmp", L"Rotation");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Carrot/Reset.bmp", L"Reset");

	m_tInfo.iCX = 150;
	m_tInfo.iCY = 150;

	m_eGroup = GROUPID::OBJECT;

	m_tInfo.iHp = 10;

	m_ePreScene = IDLE;
	m_eCurScene = IDLE;

	m_pFrameKey = L"Carrot_Bullet";

	m_fSpeed = 2.f;
}

int CCarrotBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_tInfo.iHp <= 0)
	{
		m_eCurScene = DEAD;
		m_pFrameKey = L"Death";
	}

	CObj* pTarget = CObjMgr::Get_Instance()->Get_Target(OBJID::PLAYER, this);

	if (pTarget)
	{
		float fX = pTarget->Get_Info().fX - m_tInfo.fX;
		float fY = pTarget->Get_Info().fY - m_tInfo.fY;
		float fDia = sqrtf(fX * fX + fY * fY);

		float fRad = acosf(fX / fDia);
		m_fAngle = fRad * 180.f / PI;

		if (m_tInfo.fY < pTarget->Get_Info().fY)
			m_fAngle *= -1.f;
	}

	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	Scene_Change();
	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CCarrotBullet::Late_Update()
{
	float fDis = sqrtf((float)((m_tInfo.iCX >> 1) * (m_tInfo.iCX >> 1) + (m_tInfo.iCY >> 1) * (m_tInfo.iCY >> 1)));

	m_tPos[0].x = (LONG)((m_tInfo.iCX >> 1) + cosf(((m_fAngle - 270) + 135.f) * PI / 180.f) * fDis);
	m_tPos[0].y = (LONG)((m_tInfo.iCY >> 1) - sinf(((m_fAngle - 270) + 135.f) * PI / 180.f) * fDis);

	m_tPos[1].x = (LONG)((m_tInfo.iCX >> 1) + cosf(((m_fAngle - 270) + 45.f) * PI / 180.f) * fDis);
	m_tPos[1].y = (LONG)((m_tInfo.iCY >> 1) - sinf(((m_fAngle - 270) + 45.f) * PI / 180.f) * fDis);

	m_tPos[2].x = (LONG)((m_tInfo.iCX >> 1) + cosf(((m_fAngle - 270) + 225.f) * PI / 180.f) * fDis);
	m_tPos[2].y = (LONG)((m_tInfo.iCY >> 1) - sinf(((m_fAngle - 270) + 225.f) * PI / 180.f) * fDis);
}

void CCarrotBullet::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	if (m_eCurScene == IDLE)
	{
		HDC hRotation = CBmpMgr::Get_Instance()->Find_Image(L"Rotation");
		HDC hReset = CBmpMgr::Get_Instance()->Find_Image(L"Reset");

		PlgBlt(hRotation, m_tPos
			, hMemDC
			, 0, 0
			, 150, 150
			, NULL, NULL, NULL);

		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hRotation
			, 0, 0
			, 150, 150
			, RGB(40, 40, 40));

		BitBlt(hRotation, 0, 0, 150, 150, hReset, 0, 0, SRCCOPY);
	}

	else
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

void CCarrotBullet::Release()
{
}

void CCarrotBullet::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		if (m_eCurScene == DEAD)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_bDead = true;
		}
		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CCarrotBullet::Scene_Change()
{
	if (m_eCurScene != m_ePreScene)
	{
		switch (m_eCurScene)
		{
		case CCarrotBullet::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Carrot_Bullet";
			m_tInfo.iCX = 150;
			m_tInfo.iCY = 150;
			break;

		case CCarrotBullet::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Death";
			m_tInfo.iCX = 100;
			m_tInfo.iCY = 100; 
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}
