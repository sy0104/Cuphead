#include "stdafx.h"
#include "Vine.h"
#include "BmpMgr.h"
#include "Chomper.h"
#include "MiniFlower.h"
#include "VenusFlytrap.h"
#include "ObjMgr.h"


CVine::CVine()
	: m_bGrowUp(false), m_bCreate(false), m_iMonster(1)
{
}


CVine::~CVine()
{
	Release();
}

void CVine::Initialize()
{
	// ¾¾¾Ñ 40 * 50
	// 60 * 206
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Firing_Seeds/Vine.bmp", L"Seed_Vine");

	m_tInfo.iCX = 60;
	m_tInfo.iCY = 200;

	m_eGroup = GROUPID::OBJECT;


	m_iMonster = CVine::CHOMPER;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 24;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 80;
	m_tFrame.dwFrameTime = GetTickCount();
	m_pFrameKey = L"Seed_Vine";
}

int CVine::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CVine::Late_Update()
{

}

void CVine::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Seed_Vine");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 60, m_tFrame.iFrameScene * 206
		, 60, 206
		, RGB(40, 40, 40));
}

void CVine::Release()
{
}

void CVine::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		// ¿Ã¶ó¿È
		if (!m_bGrowUp)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				// MF
				if (m_iMonster == 2)
				{
					CObj* pObj = CAbstractFactory<CMiniFlower>::Create(m_tInfo.fX, m_tInfo.fY - 100.f);
					CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_BACK, pObj);
				}
				// VF
				else if (m_iMonster == 3)
				{
					CObj* pObj = CAbstractFactory<CVenusFlytrap>::Create(m_tInfo.fX, m_tInfo.fY - 100.f);
					CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pObj);
				}

				m_bGrowUp = true;
			}		
			m_tFrame.dwFrameTime = GetTickCount();
		}

		// ³»·Á°¨
		else if (m_bGrowUp)
		{
			--m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart == 6)
			{
				if (m_iMonster == 1)
				{
					// Chomper
					CObj* pObj = CAbstractFactory<CChomper>::Create(m_tInfo.fX, m_tInfo.fY + 100.f);
					CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pObj);
				}
				m_bDead = true;
			}

			m_tFrame.dwFrameTime = GetTickCount();
		}
	}
}
