#include "stdafx.h"
#include "PotatoCreateEffect.h"
#include "BmpMgr.h"


CPotatoCreateEffect::CPotatoCreateEffect()
{
}


CPotatoCreateEffect::~CPotatoCreateEffect()
{
	Release();
}

void CPotatoCreateEffect::Initialize()
{
	// 94 * 146
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Potato_Create.bmp", L"Potato_Create_Effect");

	m_tInfo.iCX = 100;
	m_tInfo.iCY = 200;
	m_eGroup = GROUPID::EFFECT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Potato_Create_Effect";
}

int CPotatoCreateEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CPotatoCreateEffect::Late_Update()
{
}

void CPotatoCreateEffect::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Potato_Create_Effect");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 94, m_tFrame.iFrameScene * 146
		, 94, 146
		, RGB(40, 40, 40));
}

void CPotatoCreateEffect::Release()
{
}

void CPotatoCreateEffect::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_bDead = true;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
