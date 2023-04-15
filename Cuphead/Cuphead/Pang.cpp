#include "stdafx.h"
#include "Pang.h"
#include "BmpMgr.h"
#include "ObjMgr.h"


CPang::CPang()
	: m_bStart(false)
{
}


CPang::~CPang()
{
	Release();
}

void CPang::Initialize()
{
	// 190 * 181 (12)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Legendary_Chalice/Pang.bmp", L"Pang");

	m_tInfo.iCX = 100;
	m_tInfo.iCY = 100;

	m_eGroup = GROUPID::OBJECT;


	m_fSpeed = 7.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 19;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
	m_pFrameKey = L"Pang";
}

int CPang::Update()
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

void CPang::Late_Update()
{
	if (m_tRect.left >= WINCX || m_tRect.right <= 0)
		m_bDead = true;
}

void CPang::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Pang");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 190, m_tFrame.iFrameScene * 181
		, 190, 181
		, RGB(240, 240, 240));
}

void CPang::Release()
{
}

void CPang::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{


		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_bDead = true;
			//m_tFrame.iFrameStart = 0;
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

