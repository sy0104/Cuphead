#include "stdafx.h"
#include "Seed.h"
#include "BmpMgr.h"
#include "Vine.h"
#include "ObjMgr.h"


CSeed::CSeed()
	: m_bCheck(false), m_fSpeedX(0.f), m_dwTime(GetTickCount())
{
}


CSeed::~CSeed()
{
	Release();
}

void CSeed::Initialize()
{
	// 40 * 50
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Firing_Seeds/Seeds/Seed_Purple.bmp", L"Seed_Purple");

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 50;

	m_eGroup = GROUPID::OBJECT;

	m_fSpeed = 3.f;
	m_fSpeedX = 1.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 21;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 150;
	m_tFrame.dwFrameTime = GetTickCount();
	m_pFrameKey = L"Seed_Purple";
}

int CSeed::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fY += m_fSpeed;

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CSeed::Late_Update()
{
	if (m_tInfo.fY >= 600.f)
		m_bDead = true;
}

void CSeed::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Seed_Purple");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 40, m_tFrame.iFrameScene * 50
		, 40, 50
		, RGB(40, 40, 40));
}

void CSeed::Release()
{
	// Vine »ý¼º
	CObj* pObj = CAbstractFactory<CVine>::Create(m_tInfo.fX, m_tInfo.fY- 70.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::BACK, pObj);
	dynamic_cast<CVine*>(pObj)->Set_Monster(m_iMonster);
	
}

void CSeed::Frame_Move()
{
	// 0 ~ 12 ¶³¾îÁü
	// 13 ~ 21 ¶¥¿¡ ¹¯Èû
	if (m_tInfo.fY < 550.f)
	{
		m_tFrame.iFrameEnd = 12;
	}

	else if (m_tInfo.fY >= 550.f && !m_bCheck)
	{
		m_fSpeed = 0.f;
		m_tFrame.iFrameStart = 13;
		m_tFrame.iFrameEnd = 21;

		m_bCheck = true;
	}

	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwFrameTime = GetTickCount();
	}

	if (m_tInfo.fY >= 550.f && m_bCheck && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		m_bDead = true;
}
