#include "stdafx.h"
#include "Soil.h"
#include "BmpMgr.h"


CSoil::CSoil()
	: m_ePreScene(END), m_eCurScene(END)
{
}


CSoil::~CSoil()
{
	Release();
}

void CSoil::Initialize()
{
	// Idle: 134 * 139 (7)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Potato/Soil.bmp", L"Soil");
	// Death: 290 * 278 (9)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Potato/Soil_Death.bmp", L"Soil_Death");


	m_tInfo.iCX = 100;
	m_tInfo.iCY = 100;

	m_eGroup = GROUPID::OBJECT;

	m_fSpeed = 7.f;

	m_tInfo.iMaxHp = 10;
	m_tInfo.iHp = m_tInfo.iMaxHp;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();

	m_ePreScene = IDLE;
	m_eCurScene = IDLE;

	m_pFrameKey = L"Soil";

}

int CSoil::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_tInfo.iHp > 0)
		m_tInfo.fX -= m_fSpeed;

	else if (m_tInfo.iHp <= 0)
	{
		m_tInfo.iCX = 200;
		m_tInfo.iCY = 200;
		m_eCurScene = DEATH;
	}

	Scene_Change();
	Frame_Move();
	Update_Rect();
	
	return OBJ_NOEVENT;
}

void CSoil::Late_Update()
{
	if (m_tRect.right <= 0)
		m_bDead = true;
}

void CSoil::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	if (m_eCurScene == IDLE)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 134, m_tFrame.iFrameScene * 139
			, 134, 139
			, RGB(40, 40, 40));
	}

	else if (m_eCurScene == DEATH)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 290, m_tFrame.iFrameScene * 278
			, 290, 278
			, RGB(40, 40, 40));
	}
}

void CSoil::Release()
{
}

void CSoil::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			if (m_eCurScene == DEATH)
				m_bDead = true;
			else
				m_tFrame.iFrameStart = 0;
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CSoil::Scene_Change()
{
	if (m_eCurScene != m_ePreScene)
	{
		switch (m_eCurScene)
		{
		case CSoil::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Soil_Idle";
			break;
		case CSoil::DEATH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Soil_Death";
			break;
		}
		m_ePreScene = m_eCurScene;
	}
}
