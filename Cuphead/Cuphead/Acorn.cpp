#include "stdafx.h"
#include "Acorn.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "AcornBoostEffect.h"


CAcorn::CAcorn()
	: m_bCheck(false), m_bBoostEffect(false)
{
}


CAcorn::~CAcorn()
{
	Release();
}


void CAcorn::Initialize()
{
	// 90 * 60
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Creating_Object/Acorn.bmp", L"Acorn");

	m_tInfo.iCX = 130;
	m_tInfo.iCY = 100;

	m_eGroup = GROUPID::OBJECT;

	m_fSpeed = 10.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Acorn";
}

int CAcorn::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_tFrame.iFrameStart >= 8)
	{
		if (!m_bBoostEffect)
		{
			CObj* pObj = CAbstractFactory<CAcornBoostEffect>::Create(m_tInfo.fX + 50.f, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);
			
			m_bBoostEffect = true;
		}

		m_tInfo.fX -= m_fSpeed;
	}

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CAcorn::Late_Update()
{
	if (m_tRect.right <= 0)
		m_bDead = true;
}

void CAcorn::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Acorn");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 90, m_tFrame.iFrameScene * 60
		, 90, 60
		, RGB(40, 40, 40));
}

void CAcorn::Release()
{
}

void CAcorn::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (!m_bCheck && m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_bCheck = true;
			m_tFrame.iFrameStart = 8;
		}
		else if (m_bCheck && m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameStart = 8;
			m_bBoostEffect = false;
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}
