#include "stdafx.h"
#include "Potato.h"
#include "BmpMgr.h"
#include "Soil.h"
#include "Earthworm.h"
#include "BackSoil.h"
#include "ObjMgr.h"
#include "PotatoCreateEffect.h"
#include "BossDeathEffect.h"
#include "SoundMgr.h"


CPotato::CPotato()
	: m_ePreScene(END), m_eCurScene(END), m_bIntro1(false), m_bIntro2(false)
	, m_bCreate(false), m_iCreateCnt(0), m_bDieMotion(false), m_iDeathCnt(0), m_pBackSoil(nullptr)
	, m_bCreateEffect(false), m_dwDeathEffect(GetTickCount())
{
}


CPotato::~CPotato()
{
	Release();
}

void CPotato::Initialize()
{
	// 흙 인트로 - 557 * 461
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Potato/Intro_Earth.bmp", L"Earth_Intro");
	// Intro - 526 * 512
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Potato/Intro.bmp", L"Potato_Intro");
	// Idle - 526 * 512
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Potato/Idle.bmp", L"Potato_Idle");
	// Attack - 526 * 512
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Potato/Attack.bmp", L"Potato_Attack");
	// Death - 303 * 439 (12개, iFrameEnd 11개)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Potato/Death.bmp", L"Potato_Death");


	m_tInfo.fX = 1050.f;
	m_tInfo.fY = 350.f;

	m_tInfo.iCX = 450;
	m_tInfo.iCY = 500;

	m_eGroup = GROUPID::OBJECT;


	m_tInfo.iMaxHp = 150;
	m_tInfo.iHp = m_tInfo.iMaxHp;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 15;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_eCurScene = CPotato::INTRO_EARTH;
	m_pFrameKey = L"Earth_Intro";
}

int CPotato::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (GetAsyncKeyState('W'))
		m_tInfo.iHp = 10;

	// 체력 500

	// 5. Idle
	Set_Idle(100, 150);

	// 6: Attack
	if (m_tInfo.iHp >= 50 && m_tInfo.iHp < 100)
		Attack();

	Set_Idle(0, 50);

	// 7. 0: Death
	if (m_tInfo.iHp <= 0)
	{
		if (m_dwDeathEffect + 300 < GetTickCount())
		{

			CObj* pObj = CAbstractFactory<CBossDeathEffect>::Create((float)(rand() % 700 + 600), (float)(rand() % 300 + 200));
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);

			m_dwDeathEffect = GetTickCount();
		}

		m_eCurScene = CPotato::DEATH;
		m_pFrameKey = L"Potato_Death";
	}


	Scene_Change();
	Frame_Move();
	Update_Rect();
	Update_CollisionRect();

	return OBJ_NOEVENT;
}

void CPotato::Late_Update()
{
}

void CPotato::Render(HDC _DC)
{
	Update_Rect();

	/////////////////////////////// 인트로 //////////////////////////////////////////////////////
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	if (!m_bIntro1)
	{
		// Earth_Intro 557 * 461
		if (m_eCurScene == CPotato::INTRO_EARTH)
		{
			GdiTransparentBlt(_DC
				, m_tRect.left, m_tRect.top
				, m_tInfo.iCX, m_tInfo.iCY
				, hMemDC
				, m_tFrame.iFrameStart * 557, m_tFrame.iFrameScene * 461
				, 557, 461
				, RGB(40, 40, 40));
		}

		if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
		{
			m_bIntro1 = true;
			m_eCurScene = CPotato::INTRO;
			m_pFrameKey = L"Potato_Intro";
		}
		return;

	}

	else if (m_bIntro1 && !m_bIntro2)
	{
		// Earth_Intro 557 * 461
		if (m_eCurScene == CPotato::INTRO)
		{
			GdiTransparentBlt(_DC
				, m_tRect.left, m_tRect.top
				, m_tInfo.iCX, m_tInfo.iCY
				, hMemDC
				, m_tFrame.iFrameStart * 526, m_tFrame.iFrameScene * 512
				, 526, 512
				, RGB(40, 40, 40));
		}
		if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
		{
			m_bIntro2 = true;
			m_eCurScene = CPotato::IDLE;
			m_pFrameKey = L"Potato_Idle";
		}
		return;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////

	// Death 303 * 439
	else if (m_eCurScene == CPotato::DEATH)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 303, m_tFrame.iFrameScene * 439
			, 303, 439
			, RGB(40, 40, 40));
	}

	// 나머지 526 * 512
	else
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 526, m_tFrame.iFrameScene * 512
			, 526, 512
			, RGB(40, 40, 40));
	}

}

void CPotato::Release()
{
}

void CPotato::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		if (m_eCurScene == CPotato::DEATH)
		{
			// 3번 반복
			if (m_iDeathCnt <= 40)
			{
				if (!m_bDieMotion)
				{
					++m_tFrame.iFrameStart;
					++m_iDeathCnt;
					if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
					{
						m_bDieMotion = true;
						//m_bBossDeathEffect = false;
					}
				}

				else if (m_bDieMotion)
				{
					--m_tFrame.iFrameStart;
					++m_iDeathCnt;
					if (m_tFrame.iFrameStart == 0)
					{
						m_bDieMotion = false;
						//m_bBossDeathEffect = false;
					}
				}
			}

			else
			{
				m_bDead = true;
				m_pBackSoil->Set_Dead();
			}

			//if (m_iDeathCnt > 40)
			//{
			//	m_bDead = true;
			//	m_pBackSoil->Set_Dead();
			//	//SAFE_DELETE(m_pBackSoil);
			//}
		}

		else
		{
			++m_tFrame.iFrameStart;

			if (m_eCurScene == INTRO_EARTH && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				m_pBackSoil = CAbstractFactory<CBackSoil>::Create(m_tInfo.fX, m_tInfo.fY + 220.f);
				CObjMgr::Get_Instance()->Add_Object(OBJID::BACK, m_pBackSoil);
			}

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				if (m_eCurScene == ATTACK)
					m_bCreateEffect = false;

				m_tFrame.iFrameStart = 0;
			}
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CPotato::Scene_Change()
{
	if (m_eCurScene != m_ePreScene)
	{
		switch (m_eCurScene)
		{
		case CPotato::INTRO_EARTH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 18;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Earth_Intro";
			break;
		case CPotato::INTRO:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 16;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Potato_Intro";
			break;
		case CPotato::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Potato_Idle";
			break;
		case CPotato::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Potato_Attack";
			break;
		case CPotato::DEATH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 11;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Potato_Death";
			m_tInfo.fY = 400.f;
			m_tInfo.iCX = 300;
			m_tInfo.iCY = 400;
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}

void CPotato::Update_CollisionRect()
{
	m_tCollisionRect.left = (LONG)(m_tInfo.fX - 100.f);
	m_tCollisionRect.top = (LONG)(m_tInfo.fY - 240.f);
	m_tCollisionRect.right = (LONG)(m_tInfo.fX + 100.f);
	m_tCollisionRect.bottom = (LONG)(m_tInfo.fY + 240.f);
}

void CPotato::Attack()
{
	m_eCurScene = CPotato::ATTACK;
	m_pFrameKey = L"Potato_Attack";


	if (m_tFrame.iFrameStart == 10 && !m_bCreate)
	{
		if (m_iCreateCnt <= 2)
		{
			if (!m_bCreateEffect)
			{
				CObj* pObj = CAbstractFactory<CPotatoCreateEffect>::Create(m_tInfo.fX - 300.f, m_tInfo.fY + 200.f);
				CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);

				m_bCreateEffect = true;
			}

			CObj* pObj = CAbstractFactory<CSoil>::Create(m_tInfo.fX - 200.f, m_tInfo.fY + 200.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pObj);

			m_bCreate = true;
		}
		else if (m_iCreateCnt >= 3)
		{
			if (!m_bCreateEffect)
			{
				CObj* pObj = CAbstractFactory<CPotatoCreateEffect>::Create(m_tInfo.fX - 300.f, m_tInfo.fY + 200.f);
				CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);

				m_bCreateEffect = true;
			}

			CObj* pObj = CAbstractFactory<CEarthworm>::Create(m_tInfo.fX - 200.f, m_tInfo.fY + 200.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pObj);

			m_bCreate = true;
			m_iCreateCnt = 0;
		}

		++m_iCreateCnt;

	}

	else if (m_tFrame.iFrameStart == 11)
		m_bCreate = false;

}

void CPotato::Set_Idle(int _iFirst, int _iEnd)
{
	if (m_eCurScene != CPotato::IDLE
		&& m_bIntro1 && m_bIntro2 && m_tInfo.iHp >= _iFirst && m_tInfo.iHp < _iEnd)
	{
		m_eCurScene = CPotato::IDLE;
		m_pFrameKey = L"Potato_Idle";
	}
}
