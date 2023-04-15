#include "stdafx.h"
#include "PlayerHp.h"
#include "BmpMgr.h"
#include "ObjMgr.h"


CPlayerHp::CPlayerHp()
	: m_iDrawID(2)
{
}


CPlayerHp::~CPlayerHp()
{
	Release();
}

void CPlayerHp::Initialize()
{
	// 120 * 50 (2)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Player_Hp.bmp", L"Player_Hp");

	m_eGroup = GROUPID::BACKGROUND;

	m_tInfo.fX = 80.f;
	m_tInfo.fY = 650.f;

	m_tInfo.iCX = 70;
	m_tInfo.iCY = 30;
}

int CPlayerHp::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	switch (CObjMgr::Get_Instance()->Get_Player()->Get_Hp())
	{
	case 1:
		m_iDrawID = 0;
		break;
	case 2:
		m_iDrawID = 1;
		break;
	case 3:
		m_iDrawID = 2;
		break;
	default:
		m_iDrawID = 2;
	}

	Update_Rect();

	return OBJ_NOEVENT;
}

void CPlayerHp::Late_Update()
{
}

void CPlayerHp::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Hp");

	GdiTransparentBlt(_DC, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * 120, 0
		, 120, 50
		, RGB(240, 240, 240));
}

void CPlayerHp::Release()
{
}
