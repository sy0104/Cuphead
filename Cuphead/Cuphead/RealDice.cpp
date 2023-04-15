#include "stdafx.h"
#include "RealDice.h"
#include "BmpMgr.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"



CRealDice::CRealDice()
	: m_bIntro(false), m_iNum(0), m_bNext(false)
{
}


CRealDice::~CRealDice()
{
	Release();
}

void CRealDice::Initialize()
{
	// Idle -  122 * 106 (23)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Dice/Idle.bmp", L"RealDice_Idle");
	// Dice_01 - 190 * 200 (21)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Dice/Dice_01.bmp", L"RealDice_1");
	// Dice_02 - 190 * 200 (20)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Dice/Dice_02.bmp", L"RealDice_2");
	// Dice_03 - 190 * 200 (19)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Dice/Dice_03.bmp", L"RealDice_3");

	m_tInfo.fX = WINCX >> 1;
	m_tInfo.fY = 450.f;

	m_tInfo.iCX = 110;
	m_tInfo.iCY = 110;

	m_bParry = true;

	m_eGroup = GROUPID::OBJECT;

	m_iNum = rand() % 5;

	m_ePreScene = IDLE;
	m_eCurScene = IDLE;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 23;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 40;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"RealDice_Idle";
}

int CRealDice::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Scene_Change();
	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CRealDice::Late_Update()
{
}

void CRealDice::Render(HDC _DC)
{
	Update_Rect();

	if (!m_bIntro)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"RealDice_Idle");

		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 122, m_tFrame.iFrameScene * 106
			, 122, 106
			, RGB(40, 40, 40));
	}

	else
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 190, m_tFrame.iFrameScene * 200
			, 190, 200
			, RGB(40, 40, 40));
	}
}

void CRealDice::Release()
{
}

void CRealDice::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		if (m_eCurScene == IDLE)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_bIntro = true;

				if (m_iNum == 1)
				{
					m_bNext = true;
					m_eCurScene = DICE_1;
					m_pFrameKey = L"RealDice_1";
				}
				else if (m_iNum == 2)
				{
					m_bNext = true;
					m_eCurScene = DICE_2;
					m_pFrameKey = L"RealDice_2";
				}
				else
				{
					m_bNext = true;
					m_eCurScene = DICE_3;
					m_pFrameKey = L"RealDice_3";
				}
			}
		}

		else
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > 13)
			{
				m_tInfo.iCX = 200;
				m_tInfo.iCY = 200;
			}
			else if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_bDead = true;
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CRealDice::Scene_Change()
{
	if (m_eCurScene != m_ePreScene)
	{
		switch (m_eCurScene)
		{
		case CRealDice::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 23;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"RealDice_Idle";
			break;
		case CRealDice::DICE_1:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 21;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 30;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"RealDice_1";
			m_tInfo.iCX = 200;
			m_tInfo.iCY = 200;
			break;
		case CRealDice::DICE_2:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 20;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 30;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"RealDice_2";
			m_tInfo.iCX = 200;
			m_tInfo.iCY = 200;
			break;
		case CRealDice::DICE_3:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 19;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 30;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"RealDice_3";
			m_tInfo.iCX = 200;
			m_tInfo.iCY = 200;
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}
