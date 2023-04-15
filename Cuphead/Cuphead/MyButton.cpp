#include "stdafx.h"
#include "MyButton.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"


CMyButton::CMyButton()
{
}


CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize()
{
	m_tInfo.iCX = 100;
	m_tInfo.iCY = 120;

	m_eGroup = GROUPID::BACKGROUND;
}

int CMyButton::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CMyButton::Late_Update()
{
	RECT rc = {};

	if (IntersectRect(&rc, &m_tRect, &(CObjMgr::Get_Instance()->Get_Player()->Get_Rect())))
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing('Z'))
		{
			if (!lstrcmp(m_pFrameKey, L"Flag_Roots"))
			{
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::STAGE1);
				return;
			}
			else if (!lstrcmp(m_pFrameKey, L"Flag_Cagney"))
			{
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::STAGE2);
				return;
			}
			else if (!lstrcmp(m_pFrameKey, L"Flag_Dice"))
			{
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::STAGE3);
				return;
			}
			else if (!lstrcmp(m_pFrameKey, L"Flag_Test"))
			{
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::TEST);
				return;
			}
		}
	}
}

void CMyButton::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0, 0
		, 100, 120
		, RGB(40, 40, 40));
}

void CMyButton::Release()
{
}
