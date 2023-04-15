#include "stdafx.h"
#include "BackSoil.h"
#include "BmpMgr.h"


CBackSoil::CBackSoil()
{
}


CBackSoil::~CBackSoil()
{
	Release();
}

void CBackSoil::Initialize()
{
	// 캐릭터 배경 흙 - 415 * 126
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Potato/Back_Soil.bmp", L"Back_Soil");

	m_eGroup = GROUPID::OBJECT;


	m_tInfo.iCX = 380;
	m_tInfo.iCY = 130;
}

int CBackSoil::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CBackSoil::Late_Update()
{
}

void CBackSoil::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Back_Soil");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0, 0
		, 415, 126
		, RGB(40, 40, 40));

//	BitBlt(_DC, m_tInfo.fX, m_tInfo.fY, m_tInfo.iCX, m_tInfo.iCY, hMemDC, 0, 0, SRCCOPY);

}

void CBackSoil::Release()
{
}
