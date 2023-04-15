#include "stdafx.h"
#include "Card.h"
#include "BmpMgr.h"


CCard::CCard()
	: m_eCardDir(MONSTER::END)
{
}


CCard::~CCard()
{
	Release();
}

void CCard::Initialize()
{
	// Club - 230 * 220 (20)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Card/Card_Club.bmp", L"Card_Club");
	// Diamond - 230 * 220 (20)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Card/Card_Diamond.bmp", L"Card_Diamond");
	// Heart - 230 * 220 (20)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Card/Card_Heart.bmp", L"Card_Heart");
	// Space - 230 * 220 (20)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Card/Card_Space.bmp", L"Card_Space");
	// Stretch
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Card/Stretch_Card.bmp", L"Stretch_Card");


	m_tInfo.iCX = 200;
	m_tInfo.iCY = 190;

	m_fSpeed = 2.f;

	m_eGroup = GROUPID::OBJECT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 20;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
}

int CCard::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	// 왼쪽에서 생성 -> 오른쪽으로 걸어감 +=
	if (m_eCardDir == MONSTER::LEFT)
		m_tInfo.fX += m_fSpeed;

	// 오른쪽에서 생성 -> 왼쪽으로 걸어감 -=
	else if (m_eCardDir == MONSTER::RIGHT)
		m_tInfo.fX -= m_fSpeed;

	Frame_Move();
	Update_Rect();
	Update_CollisionRect();

	return OBJ_NOEVENT;
}

void CCard::Late_Update()
{
	// 오른쪽으로 이동
	if (m_eCardDir == MONSTER::LEFT && m_tRect.left >= WINCX)
		m_bDead = true;
	// 왼쪽으로 이동
	else if (m_eCardDir == MONSTER::RIGHT && m_tRect.right <= 0)
		m_bDead = true;
}

void CCard::Render(HDC _DC)
{
	Update_Rect();


	// 원래 이미지는 왼쪽에서 오른쪽으로 걸어감

	// 오른쪽 -> 왼쪽일때 이미지 반대로
	if (m_eCardDir == MONSTER::RIGHT)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
		HDC hStretch = CBmpMgr::Get_Instance()->Find_Image(L"Stretch_Card");

		StretchBlt(hStretch
			, 0, 0
			, 230, 220
			, hMemDC
			, m_tFrame.iFrameStart * 230 + 230, m_tFrame.iFrameScene * 220
			, -230, 220
			, SRCCOPY);

		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hStretch
			, 0, 0
			, 230, 220
			, RGB(40, 40, 40));
	}

	// 왼쪽 -> 오른쪽일땐 그대로 출력
	else
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 230, m_tFrame.iFrameScene * 220
			, 230, 220
			, RGB(40, 40, 40));
	}
}

void CCard::Release()
{
}

void CCard::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CCard::Update_CollisionRect()
{
	m_tCollisionRect.left = (LONG)(m_tInfo.fX - 10.f);
	m_tCollisionRect.top = (LONG)(m_tInfo.fY - 70.f);
	m_tCollisionRect.right = (LONG)(m_tInfo.fX + 10.f);
	m_tCollisionRect.bottom = (LONG)(m_tInfo.fY + 70.f);
}
