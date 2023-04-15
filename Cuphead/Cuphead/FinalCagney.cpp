#include "stdafx.h"
#include "FinalCagney.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "MainVine.h"
#include "SubVine.h"
#include "Pollen.h"
#include "Bravo.h"
#include "BossDeathEffect.h"
#include "SoundMgr.h"


CFinalCagney::CFinalCagney()
	: m_ePreScene(END), m_eCurScene(END), m_bIntro(false), m_bCreateVine(false)
	, m_bAttackVine(false) , m_bCreatePollen(false), m_iDeathCnt(0), m_dwCreateVine(GetTickCount())
	, m_iVinePos(0), m_iPollenColor(1), m_dwCreatePollen(GetTickCount()), m_bBravo(false), m_dwDeathEffect(GetTickCount())
	, m_bDeathSound(false)
{
}


CFinalCagney::~CFinalCagney()
{
	Release();
}

void CFinalCagney::Initialize()
{
	// º¸½º Final Form - 600 * 700
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Final_Form/Intro.bmp", L"Final_Intro");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Final_Form/Idle.bmp", L"Final_Idle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Final_Form/Firing_Pollen/Firing_Pollen.bmp", L"Final_Attack");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Death/Death.bmp", L"Death");


	m_tInfo.fX = 1000.f;
	m_tInfo.fY = 400.f;

	m_tInfo.iMaxHp = 400;
	m_tInfo.iHp = m_tInfo.iMaxHp;

	m_tInfo.iCX = 450;
	m_tInfo.iCY = 550;

	m_ePreScene = INTRO;
	m_eCurScene = INTRO;

	m_eGroup = GROUPID::OBJECT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 32;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
	m_pFrameKey = L"Final_Intro";
}

int CFinalCagney::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bIntro)
	{
		m_eCurScene = INTRO;
		m_pFrameKey = L"Final_Intro";
	}

	else if (m_bIntro && (m_tInfo.iHp >= 350 && m_tInfo.iHp < 400))
	{
		m_eCurScene = IDLE;
		m_pFrameKey = L"Final_Idle";
	}

	else if (m_tInfo.iHp >= 250 && m_tInfo.iHp < 350)
	{
		Attack_Vine();
		if (m_dwCreatePollen + 6000 < GetTickCount())
		{
			Attack_Pollen();
			//m_dwCreatePollen = GetTickCount();
		}
	}


	else if (m_tInfo.iHp < 250)
	{
		m_eCurScene = DEAD;
		m_pFrameKey = L"Death";

		if (m_dwDeathEffect + 300 < GetTickCount())
		{
			CObj* pObj = CAbstractFactory<CBossDeathEffect>::Create((float)(rand() % 500 + 700), (float)(rand() % 300 + 200));
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);

			m_dwDeathEffect = GetTickCount();
		}

		else if (!m_bBravo)
		{
			//CSoundMgr::Get_Instance()->PlaySound(L"Knockout.mp3", CSoundMgr::EFFECT);

			CObj* pObj = CAbstractFactory<CBravo>::Create(600.f, 350.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);
			
			m_bBravo = true;
		}
	}


	Scene_Change();
	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;;
}

void CFinalCagney::Late_Update()
{
	//if (!m_bDeathSound && m_eCurScene == DEAD)
	//{
	//	CSoundMgr::Get_Instance()->PlaySound(L"Knockout.mp3", CSoundMgr::EFFECT);
	//	m_bDeathSound = true;
	//}
}

void CFinalCagney::Render(HDC _DC)
{
	Update_Rect();

	if (!m_bIntro)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Final_Intro");

		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 600, m_tFrame.iFrameScene * 700
			, 600, 700
			, RGB(40, 40, 40));
		return;
	}


	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 600, m_tFrame.iFrameScene * 700
		, 600, 700
		, RGB(40, 40, 40));

}

void CFinalCagney::Release()
{
}

void CFinalCagney::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		if (m_eCurScene == CFinalCagney::INTRO)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart == 19)
				Create_Vine();

			else if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_bIntro = true;
				m_eCurScene = IDLE;
				m_pFrameKey = L"Final_Idle";
			}
		}

		else if (m_eCurScene == CFinalCagney::IDLE)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;
			
		}

		else if (m_eCurScene == CFinalCagney::DEAD)
		{
			if (!m_bDeathSound)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Knockout.mp3", CSoundMgr::EFFECT);

				m_bDeathSound = true;
			}
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				++m_iDeathCnt;
				m_tFrame.iFrameStart = 0;
				if (m_iDeathCnt > 7)
					m_bDead = true;
			}
		}

		else
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				if (m_eCurScene == ATTACK)
					m_bCreatePollen = false;

				m_tFrame.iFrameStart = 0;
			}
			
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CFinalCagney::Scene_Change()
{
	if (m_eCurScene != m_ePreScene)
	{
		switch (m_eCurScene)
		{
		case CFinalCagney::INTRO:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 32;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Final_Intro";
			break;
		case CFinalCagney::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 18;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Final_Idle";
			break;
		case CFinalCagney::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 21;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Final_Attack";
			break;
		case CFinalCagney::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 11;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Death";
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}

void CFinalCagney::Update_CollisionRect()
{
	m_tCollisionRect.left = (LONG)(m_tInfo.fX - 30.f);
	m_tCollisionRect.top = (LONG)(m_tInfo.fY - 250.f);
	m_tCollisionRect.right = (LONG)(m_tInfo.fX + 30);
	m_tCollisionRect.bottom = (LONG)(m_tInfo.fY + 250.f);
	
}

void CFinalCagney::Create_Vine()
{
	m_eCurScene = IDLE;
	m_pFrameKey = L"Final_Idle";

	if (!m_bCreateVine)
	{
		CObj* pObj = CAbstractFactory<CMainVine>::Create();
		CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_BACK, pObj);

		m_bCreateVine = true;
	}
}

void CFinalCagney::Attack_Vine()
{
	// ÇÃ·§Æû xÀ§Ä¡: 150, 400, 650
	// 1 ~ 3 ¹øÈ£
	m_iVinePos = rand() % 3 + 1;


	if (m_dwCreateVine + 4000 < GetTickCount())
	{
		if (m_iVinePos == 1)
		{
			CObj* pObj = CAbstractFactory<CSubVine>::Create(150.f, 400.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_BACK, pObj);
		}
		else if (m_iVinePos == 2)
		{
			CObj* pObj = CAbstractFactory<CSubVine>::Create(400.f, 400.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_BACK, pObj);
		}
		else
		{
			CObj* pObj = CAbstractFactory<CSubVine>::Create(650.f, 400.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_BACK, pObj);
		}

		m_dwCreateVine = GetTickCount();
	}
}

void CFinalCagney::Attack_Pollen()
{
	m_eCurScene = ATTACK;
	m_pFrameKey = L"Final_Attack";

	// 1: White, 2: Pink
	// 15¿¡ »ý¼º

	if (!m_bCreatePollen && m_tFrame.iFrameStart == 15)
	{
		if (m_iPollenColor == 1)
		{
			// ÇÏ¾á»ö
			CObj* pObj = CAbstractFactory<CPollen>::Create(m_tInfo.fX - 200.f, m_tInfo.fY - 100.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_BACK, pObj);
			dynamic_cast<CPollen*>(pObj)->Set_Color(1);

			m_iPollenColor = 2;
			m_bCreatePollen = true;
		}
		else
		{
			// ºÐÈ«»ö
			CObj* pObj = CAbstractFactory<CPollen>::Create(m_tInfo.fX - 200.f, m_tInfo.fY - 100.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_BACK, pObj);
			dynamic_cast<CPollen*>(pObj)->Set_Color(2);

			m_iPollenColor = 1;
			m_bCreatePollen = true;
		}
	}


}
