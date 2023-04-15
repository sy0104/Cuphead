#include "stdafx.h"
#include "Beam.h"
#include "BmpMgr.h"
#include "ObjMgr.h"


CBeam::CBeam()
	: m_bStart(false)
{
}


CBeam::~CBeam()
{
	Release();
}

void CBeam::Initialize()
{
	// 169 * 165
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Carrot/Beam.bmp", L"Beam");


	m_tInfo.iCX = 100;
	m_tInfo.iCY = 100;

	m_eGroup = GROUPID::OBJECT;

	m_fSpeed = 5.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
}

int CBeam::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	CObj* pTarget = CObjMgr::Get_Instance()->Get_Target(OBJID::PLAYER, this);

	if (!m_bStart)
	{
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

		//m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
		//m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

		m_bStart = true;

	}

	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;



	Frame_Move();
	Update_Rect();


	return OBJ_NOEVENT;
}

void CBeam::Late_Update()
{
	if (m_tRect.right <= 0 || m_tRect.bottom <= 0
		|| m_tRect.left >= WINCX || m_tRect.top >= WINCY)
		m_bDead = true;
}

void CBeam::Render(HDC _DC)
{
	Update_Rect();


	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Beam");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 169, m_tFrame.iFrameScene * 165
		, 169, 165
		, RGB(40, 40, 40));
	
}

void CBeam::Release()
{
}

void CBeam::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 1;

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
