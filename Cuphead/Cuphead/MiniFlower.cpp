#include "stdafx.h"
#include "MiniFlower.h"
#include "BmpMgr.h"
#include "MFBullet.h"
#include "ObjMgr.h"


// 1. 생성되면 위로 올라감

// 2. 일정 부분에 닿으면 좌우로 이동 반복

// 3. 3번정도 좌우이동 후 총알 생성

// 4. 죽으면 터지는 효과 애니메이션

CMiniFlower::CMiniFlower()
	: m_ePreScene(END), m_eCurScene(END), m_iFlyCnt(0), m_bFlyUp(false), m_bFirst(false)
{
}

CMiniFlower::~CMiniFlower()
{
	Release();
}

void CMiniFlower::Initialize()
{
	// 160 * 130
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Firing_Seeds/Mini_Flower/Mini_Flower.bmp", L"MF_Idle");
	// 200 * 200
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Firing_Seeds/Mini_Flower/MF_Die.bmp", L"MF_Die");

	m_tInfo.iCX = 150;
	m_tInfo.iCY = 150;

	m_eGroup = GROUPID::OBJECT;

	m_tInfo.iMaxHp = 70;
	m_tInfo.iHp = m_tInfo.iMaxHp;

	m_eCurScene = IDLE;
	m_ePreScene = IDLE;

	m_fSpeed = 1.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 29;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"MF_Idle";
}

int CMiniFlower::Update()
{
	if (m_bDead)
		return OBJ_DEAD;


	if (m_tInfo.fY >= 100.f && !m_bFlyUp)
		m_tInfo.fY -= m_fSpeed;

	if (m_bFlyUp)
	{
		if (m_tInfo.fX >= 200.f)
			m_tInfo.fX += m_fSpeed;
		else if (m_tInfo.fX <= 1000.f)
			m_tInfo.fX -= m_fSpeed;
	}

	if (m_tInfo.iHp <= 0)
	{
		m_pFrameKey = L"MF_Die";
		m_eCurScene = CMiniFlower::DIE;
	}

	Scene_Change();
	Frame_Move();
	Update_Rect(); 

	return OBJ_NOEVENT;
}

void CMiniFlower::Late_Update()
{
	//if (m_tRect.right <= 0 || m_tRect.bottom <= 0
	//	|| m_tRect.left >= WINCX || m_tRect.top >= WINCY)
	//	m_bDead = true;
}

void CMiniFlower::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	if (m_eCurScene == CMiniFlower::IDLE)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 160, m_tFrame.iFrameScene * 130
			, 160, 130
			, RGB(40, 40, 40));
	}

	else if (m_eCurScene == CMiniFlower::DIE)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 200, m_tFrame.iFrameScene * 200
			, 200, 200
			, RGB(40, 40, 40));
	}
}

void CMiniFlower::Release()
{
}

void CMiniFlower::Frame_Move()
{

	//if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	//{
	//	if (m_eCurScene == CMiniFlower::IDLE)
	//	{
	//		
	//	}
	//}


	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		if (m_eCurScene == CMiniFlower::IDLE)
		{
			// 4 ~ 12
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart == 26)
			{
				CObj* pObj = CAbstractFactory<CMFBullet>::Create(m_tInfo.fX, m_tInfo.fY);
				CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_BACK, pObj);
			}

			else if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_eCurScene = CMiniFlower::DIE;
				m_pFrameKey = L"MF_Die";
			}
		}

		else if (m_eCurScene == CMiniFlower::DIE)
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

void CMiniFlower::Scene_Change()
{
	if (m_eCurScene != m_ePreScene)
	{
		switch (m_eCurScene)
		{
		case CMiniFlower::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 29;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"MF_Idle";
			break;

		case CMiniFlower::DIE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"MF_Die";
			break;
		}
		m_ePreScene = m_eCurScene;
	}
}
