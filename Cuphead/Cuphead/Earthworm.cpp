#include "stdafx.h"
#include "Earthworm.h"
#include "BmpMgr.h"


CEarthworm::CEarthworm()
	: m_ePreScene(END), m_eCurScene(END)
{
}


CEarthworm::~CEarthworm()
{
	Release();
}

void CEarthworm::Initialize()
{
	// Idle: 132 * 139
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Potato/Earthworm.bmp", L"Earthworm");
	// Death: 285 * 241 (5)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Potato/Earthworm_Death.bmp", L"Earthworm_Death");


	m_tInfo.iCX = 100;
	m_tInfo.iCY = 100;

	m_eGroup = GROUPID::OBJECT;

	m_fSpeed = 7.f;

	m_tInfo.iMaxHp = 10;
	m_tInfo.iHp = m_tInfo.iMaxHp;

	m_ePreScene = IDLE;
	m_eCurScene = IDLE;

	m_pFrameKey = L"Earthworm";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
}

int CEarthworm::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_tInfo.iHp > 0)
		m_tInfo.fX -= m_fSpeed;
	else if (m_tInfo.iHp >= 0)
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

void CEarthworm::Late_Update()
{
	if (m_tRect.right <= 0)
		m_bDead = true;
}

void CEarthworm::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	if (m_eCurScene == IDLE)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 132, m_tFrame.iFrameScene * 139
			, 132, 139
			, RGB(40, 40, 40));
	}

	else
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 285, m_tFrame.iFrameScene * 241
			, 285, 241
			, RGB(40, 40, 40));
	}
}

void CEarthworm::Release()
{
}

void CEarthworm::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			if (m_eCurScene == DEATH)
				m_bDead = true;
			else
				m_tFrame.iFrameStart = 0;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CEarthworm::Scene_Change()
{
	if (m_eCurScene != m_ePreScene)
	{
		switch (m_eCurScene)
		{
		case CEarthworm::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Earthworm";
			break;
		case CEarthworm::DEATH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Earthworm_Death";
			break;
		}
		m_ePreScene = m_eCurScene;
	}
}
