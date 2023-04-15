#include "stdafx.h"
#include "Chalice.h"
#include "Pang.h"
#include "BmpMgr.h"
#include "ObjMgr.h"


CChalice::CChalice()
	: m_ePreScene(END), m_eCurScene(END), m_bCreate(false), m_dwAttackTime(GetTickCount())
{
}


CChalice::~CChalice()
{
	Release();
}

void CChalice::Initialize()
{
	// Idle - 231 * 249 (23)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Legendary_Chalice/Idle.bmp", L"Chalice_Idle");
	// Attack - 254 * 263 (23)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Legendary_Chalice/Attack.bmp", L"Chalice_Attack");
	// Twinkle - 269 * 330 (16)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Legendary_Chalice/Twinkle.bmp", L"Chalice_Twinkle");


	m_tInfo.iCX = 190;
	m_tInfo.iCY = 200;

	m_eGroup = GROUPID::OBJECT;

	m_ePreScene = IDLE;
	m_eCurScene = IDLE;

	m_tInfo.iHp = 50;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 23;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
	m_pFrameKey = L"Chalice_Idle";
}

int CChalice::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_eCurScene == ATTACK)
		Attack();

	else if (m_tInfo.iHp <= 0)
		m_eCurScene = TWINKLE;

	Scene_Change();
	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CChalice::Late_Update()
{
}

void CChalice::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	
	if (m_eCurScene == IDLE)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 231, m_tFrame.iFrameScene * 249
			, 231, 249
			, RGB(240, 240, 240));
	}

	else if (m_eCurScene == ATTACK)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 254, m_tFrame.iFrameScene * 263
			, 254, 263
			, RGB(240, 240, 240));
	}

	else if (m_eCurScene == TWINKLE)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 269, m_tFrame.iFrameScene * 330
			, 269, 330
			, RGB(240, 240, 240));
	}
}

void CChalice::Release()
{
}

void CChalice::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			if (m_eCurScene == IDLE)
				m_eCurScene = ATTACK;
			else if (m_eCurScene == ATTACK)
			{
				m_bCreate = false;
				m_tFrame.iFrameStart = 0;
			}
			else if (m_eCurScene == TWINKLE)
				m_bDead = true;
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CChalice::Scene_Change()
{
	if (m_eCurScene != m_ePreScene)
	{
		switch (m_eCurScene)
		{
		case CChalice::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 23;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_pFrameKey = L"Chalice_Idle";
			break;
		case CChalice::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 23;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_pFrameKey = L"Chalice_Attack";
			break;			
		case CChalice::TWINKLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 16;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_pFrameKey = L"Chalice_Twinkle";
			break;
		}

		m_ePreScene = m_eCurScene;
	}

}

void CChalice::Attack()
{
	// 18¿¡ »ý¼º
	if (m_tFrame.iFrameStart == 18 && !m_bCreate)
	{
		CObj* pObj = CAbstractFactory<CPang>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_BACK, pObj);

		m_bCreate = true;
	}

	if (m_tInfo.iHp <= 0)
		m_eCurScene = TWINKLE;
}
