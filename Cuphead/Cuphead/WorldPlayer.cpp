#include "stdafx.h"
#include "WorldPlayer.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"



CWorldPlayer::CWorldPlayer()
	: m_ePreScene(END), m_eCurScene(END)
{
}


CWorldPlayer::~CWorldPlayer()
{
	Release();
}

void CWorldPlayer::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Cuphead_Overworld.bmp", L"Player_Overworld");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Stretch.bmp", L"Stretch");

	m_tInfo.fX = 400.f;
	m_tInfo.fY = 400.f;

	m_tInfo.iCX = 100;
	m_tInfo.iCY = 100;

	m_fSpeed = 5.f;

	m_ePreScene = IDLE;
	m_ePreScene = IDLE;

	m_eGroup = GROUPID::OBJECT;


	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iFrameScene = IDLE;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.fSizeX = 103.0625;
	m_tFrame.fSizeY = 113.125;
	m_tFrame.iIndexY = 5;
	m_pFrameKey = L"Player_Overworld";
}

int CWorldPlayer::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Key_Check();
	OffSet();
	Scene_Change();
	Frame_Move();
	Update_Rect();

	return 0;
}

void CWorldPlayer::Late_Update()
{
}

void CWorldPlayer::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (CWorldPlayer::WALK_DIA_LEFTUP == m_eCurScene || CWorldPlayer::IDLE_LEFT == m_eCurScene
		|| CWorldPlayer::WALK_LEFT == m_eCurScene || CWorldPlayer::WALK_DIA_LEFTDOWN == m_eCurScene)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Overworld");
		HDC hStretch = CBmpMgr::Get_Instance()->Find_Image(L"Stretch");

		StretchBlt(hStretch
			, 0, 0
			, 100, 100
			, hMemDC
			, (int)(m_tFrame.iFrameStart * m_tFrame.fSizeX + 100), (int)(m_tFrame.iIndexY * m_tFrame.fSizeY)
			, -100, 100
			, SRCCOPY);

		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, m_tInfo.iCX, m_tInfo.iCY
			, hStretch
			, 0, 0
			, 100, 100
			, RGB(40, 40, 40));
	}

	else 
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, (int)m_tFrame.iFrameStart * m_tFrame.fSizeX, (int)m_tFrame.iIndexY * m_tFrame.fSizeY
			, 100, 100
			, RGB(40, 40, 40));
	}
}

void CWorldPlayer::Release()
{
}

void CWorldPlayer::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CWorldPlayer::Key_Check()
{
	// 왼쪽 이동
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_pFrameKey = L"Player_Overworld";
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		{
			m_eCurScene = WALK_DIA_LEFTUP;
			m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
			m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		{
			m_eCurScene = WALK_DIA_LEFTDOWN;
			m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
			m_tInfo.fY += m_fSpeed / sqrtf(2.f);
		}
		else
		{
			m_eCurScene = WALK_LEFT;
			m_tInfo.fX -= m_fSpeed;
		}
	}

	// 오른쪽 이동
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_pFrameKey = L"Player_Overworld";
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		{
			m_eCurScene = WALK_DIA_RIGHTUP;
			m_tInfo.fX += m_fSpeed / sqrtf(2.f);
			m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		{
			m_eCurScene = WALK_DIA_RIGHTDOWN;
			m_tInfo.fX += m_fSpeed / sqrtf(2.f);
			m_tInfo.fY += m_fSpeed / sqrtf(2.f);
		}
		else
		{
			m_eCurScene = WALK_RIGHT;
			m_tInfo.fX += m_fSpeed;
		}
	}

	// 위 이동
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		m_eCurScene = WALK_UP;
		m_pFrameKey = L"Player_Overworld";
		m_tInfo.fY -= m_fSpeed;
	}

	// 아래 이동
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		m_eCurScene = WALK_DOWN;
		m_pFrameKey = L"Player_Overworld";
		m_tInfo.fY += m_fSpeed;
	}

	// 가만히
	else
		m_eCurScene = IDLE;
}

void CWorldPlayer::OffSet()
{
	int iOffSetX = WINCX >> 1;
	int iOffSetY = WINCY >> 1;

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (iOffSetX < m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffSetX - (m_tInfo.fX + iScrollX));
	if (iOffSetX > m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffSetX - (m_tInfo.fX + iScrollX));

	if (iOffSetY < m_tInfo.fY + iScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(iOffSetY - (m_tInfo.fY + iScrollY));
	if (iOffSetY > m_tInfo.fY + iScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(iOffSetY - (m_tInfo.fY + iScrollY));

}

void CWorldPlayer::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case CWorldPlayer::WALK_UP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.iFrameScene = WALK_UP;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.iIndexY = 0;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CWorldPlayer::WALK_DIA_RIGHTUP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 14;
			m_tFrame.iFrameScene = WALK_DIA_RIGHTUP;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.iIndexY = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CWorldPlayer::IDLE_RIGHT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = IDLE_RIGHT;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.iIndexY = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CWorldPlayer::WALK_RIGHT:
			m_tFrame.iFrameScene = 0;
			m_tFrame.iFrameEnd = 13;
			m_tFrame.iFrameScene = WALK_RIGHT;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.iIndexY = 3;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CWorldPlayer::WALK_DIA_RIGHTDOWN:
			m_tFrame.iFrameScene = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.iFrameScene = WALK_DIA_RIGHTDOWN;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.iIndexY = 4;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CWorldPlayer::WALK_DIA_LEFTUP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 14;
			m_tFrame.iFrameScene = WALK_DIA_LEFTUP;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.iIndexY = 1;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CWorldPlayer::IDLE_LEFT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = IDLE_LEFT;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.iIndexY = 2;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CWorldPlayer::WALK_LEFT:
			m_tFrame.iFrameScene = 0;
			m_tFrame.iFrameEnd = 13;
			m_tFrame.iFrameScene = WALK_LEFT;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.iIndexY = 3;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CWorldPlayer::WALK_DIA_LEFTDOWN:
			m_tFrame.iFrameScene = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.iFrameScene = WALK_DIA_LEFTDOWN;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.iIndexY = 4;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CWorldPlayer::IDLE:
			m_tFrame.iFrameScene = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iFrameScene = IDLE;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.iIndexY = 5;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CWorldPlayer::WALK_DOWN:
			m_tFrame.iFrameScene = 0;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.iFrameScene = WALK_DOWN;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.iIndexY = 6;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CWorldPlayer::ENTER:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 11;
			m_tFrame.iFrameScene = ENTER;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.iIndexY = 7;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}
		m_ePreScene = m_eCurScene;
	}
}
