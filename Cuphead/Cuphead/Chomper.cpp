#include "stdafx.h"
#include "Chomper.h"
#include "BmpMgr.h"


CChomper::CChomper()
	: m_eCurScene(END), m_ePreScene(END), m_bDeath(false)
{
}


CChomper::~CChomper()
{
	Release();
}

void CChomper::Initialize()
{
	// 70 * 95
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Firing_Seeds/Chomper/Chomper.bmp", L"Chomper");
	// 280 * 280
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Firing_Seeds/Chomper/Chomper_Die.bmp", L"Chomper_Die");

	m_tInfo.iCX = 80;
	m_tInfo.iCY = 100;

	m_eGroup = GROUPID::OBJECT;


	m_tInfo.iMaxHp = 70;
	m_tInfo.iHp = m_tInfo.iMaxHp;

	m_eCurScene = IDLE;
	m_ePreScene = IDLE;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Chomper";
}

int CChomper::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Scene_Change();
	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CChomper::Late_Update()
{
}

void CChomper::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	if (m_eCurScene == CChomper::IDLE)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 70, m_tFrame.iFrameScene * 95
			, 70, 95
			, RGB(40, 40, 40));
	}

	else if (m_eCurScene == CChomper::DIE)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 280, m_tFrame.iFrameScene * 280
			, 280, 280
			, RGB(40, 40, 40));
	}
}

void CChomper::Release()
{
}

void CChomper::Frame_Move()
{
	switch (m_eCurScene)
	{
	case CChomper::IDLE:
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				if (m_tInfo.iHp <= 0)
				{
					m_pFrameKey = L"Chomper_Die";
					m_eCurScene = CChomper::DIE;
				}
				else
					m_tFrame.iFrameStart = 0;

			}
			m_tFrame.dwFrameTime = GetTickCount();
		}
		break;

	case CChomper::DIE:
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_bDead = true;

			m_tFrame.dwFrameTime = GetTickCount();
		}
		break;
	}
}

void CChomper::Scene_Change()
{
	if (m_eCurScene != m_ePreScene)
	{
		switch (m_eCurScene)
		{
		case CChomper::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Chomper"; 
			break;
		case CChomper::DIE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Chomper_Die";
			m_tInfo.iCX = 100;
			m_tInfo.iCY = 100;

			break;

		}

		m_ePreScene = m_eCurScene;
	}
}
