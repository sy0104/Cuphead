#include "stdafx.h"
#include "Carrot.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "CarrotBullet.h"
#include "PreBeam.h"
#include "Beam.h"
#include "BackSoil.h"
#include "Bravo.h"
#include "BossDeathEffect.h"
#include "SoundMgr.h"


CCarrot::CCarrot()
	: m_bIntro1(false), m_bIntro2(false), m_ePreScene(END), m_eCurScene(END)
	, m_bDieMotion(false), m_iDeathCnt(0), m_dwCarrotTime(GetTickCount()), m_dwBeamTime(GetTickCount())
	, m_dwCreateBeam(GetTickCount()), m_bPreBeam(false), m_iBeamCnt(0), m_dwTotalTime(GetTickCount()), m_pBackSoil(nullptr)
	, m_bBravo(false), m_dwDeathEffect(GetTickCount()), m_bDeathSound(false)
{
}


CCarrot::~CCarrot()
{
	Release();
}

void CCarrot::Initialize()
{
	// Earth_Intro - 336 * 252
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Carrot/Intro_Earth.bmp", L"Earth_Intro");
	// Intro - 594 * 549
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Carrot/Intro.bmp", L"Carrot_Intro");
	// Attack_Carrot - 466 * 509
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Carrot/Attack_Carrot.bmp", L"Attack_Carrot");
	// Attack_Beam  458 * 522
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Carrot/Attack_Beam.bmp", L"Attack_Beam");
	// Death 323 * 456
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Carrot/Death.bmp", L"Carrot_Death");


	m_tInfo.fX = 600.f;
	m_tInfo.fY = 250.f;

	m_tInfo.iCX = 500;
	m_tInfo.iCY = 400;

	m_eGroup = GROUPID::OBJECT;


	m_tInfo.iMaxHp = 150;
	m_tInfo.iHp = m_tInfo.iMaxHp;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_eCurScene = CCarrot::INTRO_EARTH;
	m_pFrameKey = L"Earth_Intro";
}

int CCarrot::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (GetAsyncKeyState('E'))
		m_tInfo.iHp = 10;

	// Attack_Carrot & Attack_Beam 한번씩
	if (m_bIntro1 && m_bIntro2
		&& m_tInfo.iHp >= 100 && m_tInfo.iHp <= 150)
		Attack_Carrot();

	else if (m_tInfo.iHp >= 50 && m_tInfo.iHp < 100)
		Attack_Beam();

	else if (m_tInfo.iHp > 0 && m_tInfo.iHp < 50)
		m_eCurScene = ATTACK_CARROT;
	
	else if (m_tInfo.iHp <= 0)
	{

		if (m_dwDeathEffect + 300 < GetTickCount())
		{
			CObj* pObj = CAbstractFactory<CBossDeathEffect>::Create((float)(rand() % 700 + 200), (float)(rand() % 300 + 200));
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);

			m_dwDeathEffect = GetTickCount();
		}
		
		m_eCurScene = CCarrot::DEATH;
		m_pFrameKey = L"Carrot_Death";

		if (!m_bBravo)
		{
		//	CSoundMgr::Get_Instance()->PlaySound(L"Knockout.mp3", CSoundMgr::EFFECT);

			CObj* pObj = CAbstractFactory<CBravo>::Create(600.f, 350.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);
			m_bBravo = true;
		}
	}


	Scene_Change();
	Frame_Move();
	Update_Rect();
	Update_CollisionRect();

	return OBJ_NOEVENT;
}

void CCarrot::Late_Update()
{
	if (!m_bDeathSound && m_eCurScene == DEATH)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Knockout.mp3", CSoundMgr::EFFECT);
		m_bDeathSound = true;
	}

}

void CCarrot::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	/////////////////////////////// 인트로 //////////////////////////////////////////////////////
	if (!m_bIntro1 && !m_bIntro2)
	{
		// Earth_Intro 336 * 252
		if (m_eCurScene == CCarrot::INTRO_EARTH)
		{
			GdiTransparentBlt(_DC
				, m_tRect.left, m_tRect.top
				, m_tInfo.iCX, m_tInfo.iCY
				, hMemDC
				, m_tFrame.iFrameStart * 336, m_tFrame.iFrameScene * 252
				, 336, 252
				, RGB(40, 40, 40));
		}

		if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
		{
			m_bIntro1 = true;
			m_eCurScene = CCarrot::INTRO;
			m_pFrameKey = L"Carrot_Intro";
		}
		return;
	}

	else if (m_bIntro1 && !m_bIntro2)
	{
		// Intro - 594 * 549
		if (m_eCurScene == CCarrot::INTRO)
		{
			GdiTransparentBlt(_DC
				, m_tRect.left, m_tRect.top
				, m_tInfo.iCX, m_tInfo.iCY
				, hMemDC
				, m_tFrame.iFrameStart * 594, m_tFrame.iFrameScene * 549
				, 594, 549
				, RGB(40, 40, 40));
		}
		if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
		{
			m_bIntro2 = true;
			m_eCurScene = CCarrot::ATTACK_CARROT;
			m_pFrameKey = L"Attack_Carrot";
		}
		return;
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	
	else if (m_eCurScene == CCarrot::ATTACK_CARROT)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 466, m_tFrame.iFrameScene * 509
			, 466, 509
			, RGB(40, 40, 40));
	}

	else if (m_eCurScene == CCarrot::ATTACK_BEAM)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 458, m_tFrame.iFrameScene * 522
			, 458, 522
			, RGB(40, 40, 40));
	}

	else if (m_eCurScene == CCarrot::DEATH)
	{
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 323, m_tFrame.iFrameScene * 456
			, 323, 456
			, RGB(40, 40, 40));
	}
}

void CCarrot::Release()
{
}

void CCarrot::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		if (m_eCurScene == CCarrot::DEATH)
		{
			// 3번 반복
			if (m_iDeathCnt <= 40)
			{
				if (!m_bDieMotion)
				{
					++m_tFrame.iFrameStart;
					++m_iDeathCnt;
					if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
						m_bDieMotion = true;
				}

				else if (m_bDieMotion)
				{
					--m_tFrame.iFrameStart;
					++m_iDeathCnt;
					if (m_tFrame.iFrameStart == 0)
						m_bDieMotion = false;
				}
			}

			else if (m_iDeathCnt > 33)
			{
				m_pBackSoil->Set_Dead();
				m_bDead = true;
			}
		}

		else if (m_eCurScene == CCarrot::ATTACK_BEAM)
		{
			// 7부터 반복
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 7;
		}

		else
		{
			if (m_eCurScene == INTRO && m_tFrame.iFrameStart == 0)
			{
				m_pBackSoil = CAbstractFactory<CBackSoil>::Create(m_tInfo.fX, m_tInfo.fY + 300.f);
				CObjMgr::Get_Instance()->Add_Object(OBJID::BACK, m_pBackSoil);
			}

			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CCarrot::Scene_Change()
{
	if (m_eCurScene != m_ePreScene)
	{
		switch (m_eCurScene)
		{
		case CCarrot::INTRO_EARTH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 11;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Earth_Intro";
			m_tInfo.iCX = 300;
			m_tInfo.iCY = 100;
			m_tInfo.fY = 500.f;
			break;
		case CCarrot::INTRO:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 24;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Carrot_Intro";
			m_tInfo.iCX = 550;
			m_tInfo.iCY = 650;
			m_tInfo.fY = 230.f;
			break;
		case CCarrot::ATTACK_CARROT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 21;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Attack_Carrot";
			m_tInfo.iCX = 500;
			m_tInfo.iCY = 600; 
			m_tInfo.fY = 250.f;
			break;
		case CCarrot::ATTACK_BEAM:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Attack_Beam";
			m_tInfo.iCX = 500;
			m_tInfo.iCY = 600; 
			m_tInfo.fY = 250.f;
			break;
		case CCarrot::DEATH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Carrot_Death";
			//m_tInfo.fY = 400.f;
			m_tInfo.iCX = 400;
			m_tInfo.iCY = 600;
			m_tInfo.fY = 250.f;
			break;
		}
		m_ePreScene = m_eCurScene;
	}

}

void CCarrot::Update_CollisionRect()
{
	m_tCollisionRect.left = (LONG)(m_tInfo.fX - 10.f);
	m_tCollisionRect.top = (LONG)(m_tInfo.fY - 150.f);
	m_tCollisionRect.right = (LONG)(m_tInfo.fX + 10.f);
	m_tCollisionRect.bottom = (LONG)(m_tInfo.fY + 150.f);
}

void CCarrot::Attack_Carrot()
{
	m_eCurScene = CCarrot::ATTACK_CARROT;
	m_pFrameKey = L"Attack_Carrot";

	if (m_dwCarrotTime + 3000 < GetTickCount())
	{
		CObj* pObj = CAbstractFactory<CCarrotBullet>::Create((float)(rand() % 1000), 100.f);
		CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pObj);

		m_dwCarrotTime = GetTickCount();
	}	
}

void CCarrot::Attack_Beam()
{
	m_eCurScene = CCarrot::ATTACK_BEAM;
	m_pFrameKey = L"Attack_Beam";

	if (m_dwTotalTime + 2000 < GetTickCount())
	{
		if (m_dwBeamTime + 200 < GetTickCount())
		{
			if (!m_bPreBeam)
			{
				CObj* pObj1 = CAbstractFactory<CPreBeam>::Create(m_tInfo.fX, 100.f);
				CObjMgr::Get_Instance()->Add_Object(OBJID::BACK, pObj1);

				m_bPreBeam = true;
			}

			m_dwBeamTime = GetTickCount();
		}

		else if (m_bPreBeam && m_iBeamCnt < 5 && m_dwCreateBeam + 150 < GetTickCount())
		{

			CObj* pObj = CAbstractFactory<CBeam>::Create(m_tInfo.fX, 100.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_BACK, pObj);

			++m_iBeamCnt;

			m_dwCreateBeam = GetTickCount();
			if (5 == m_iBeamCnt)
			{
				m_iBeamCnt = 0;
				m_dwTotalTime = GetTickCount();
			}
		}
	}
}