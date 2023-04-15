#include "stdafx.h"
#include "VenusFlytrap.h"
#include "BmpMgr.h"
#include "ObjMgr.h"


CVenusFlytrap::CVenusFlytrap()
	: m_ePreScene(END), m_eCurScene(END), m_eDir(DIR_END), m_iSizeX(0), m_iSizeY(0)
{
}


CVenusFlytrap::~CVenusFlytrap()
{
	Release();
}

void CVenusFlytrap::Initialize()
{
	// 100 * 60
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Firing_Seeds/Venus_Flytrap/Venus_Flytrap.bmp", L"Venus_Flytrap");

	// 292 * 298
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Firing_Seeds/Venus_Flytrap/Venus_Die.bmp", L"Venus_Die");

	m_tInfo.iCX = 100;
	m_tInfo.iCY = 60;

	m_eGroup = GROUPID::OBJECT;

	m_fSpeed = 1.f;

	m_tInfo.iHp = 20;

	m_iSizeX = 100;
	m_iSizeY = 60;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();

	m_eCurScene = CVenusFlytrap::IDLE;
	m_ePreScene = CVenusFlytrap::IDLE;

	m_pFrameKey = L"Venus_Flytrap";
}

int CVenusFlytrap::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

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


	if (pTarget->Get_Info().fX < m_tInfo.fX)
		m_eDir = LEFT;
	else if (pTarget->Get_Info().fX >= m_tInfo.fX)
		m_eDir = RIGHT;

	Scene_Change();
	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CVenusFlytrap::Late_Update()
{
}

void CVenusFlytrap::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	// 오른쪽방향 출력
	if (m_eDir == RIGHT)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
		HDC hStretch = CBmpMgr::Get_Instance()->Find_Image(L"Stretch");

		StretchBlt(hStretch
			, 0, 0
			, m_iSizeX, m_iSizeY
			, hMemDC
			, m_tFrame.iFrameStart * m_iSizeX + m_iSizeX, m_tFrame.iFrameScene * m_iSizeY
			, -m_iSizeX, m_iSizeY
			, SRCCOPY);

		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hStretch
			, 0, 0
			, m_iSizeX, m_iSizeY
			, RGB(40, 40, 40));

	}

	// 왼쪽방향 출력
	else
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * m_iSizeX, m_tFrame.iFrameScene * m_iSizeY
			, m_iSizeX, m_iSizeY
			, RGB(40, 40, 40));
	}


}

void CVenusFlytrap::Release()
{
}

void CVenusFlytrap::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		if (m_eCurScene == CVenusFlytrap::IDLE)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_tFrame.iFrameStart = 5;
				if (m_tInfo.iHp <= 0)
				{
					m_eCurScene = CVenusFlytrap::DEAD;
					m_pFrameKey = L"Venus_Die";
				}
			}
		}

		else if (m_eCurScene == CVenusFlytrap::DEAD)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_bDead = true;
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CVenusFlytrap::Scene_Change()
{
	if (m_eCurScene != m_ePreScene)
	{
		switch (m_eCurScene)
		{
		case CVenusFlytrap::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 11;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Venus_Flytrap";
			m_iSizeX = 100;
			m_iSizeY = 60;
			break;

		case CVenusFlytrap::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Venus_Die";
			m_iSizeX = 292;
			m_iSizeY = 298;
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}
