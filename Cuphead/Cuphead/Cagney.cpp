#include "stdafx.h"
#include "Cagney.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Seed.h"
#include "Boomerang.h"
#include "Acorn.h"
#include "CagneyCreateEffect.h"
#include "AcornBoostEffect.h"


CCagney::CCagney()
	: m_ePreScene(END), m_eCurScene(END), m_bIntro(false), m_bFace_Low(false), m_bFace_High(false)
	, m_bCreateSeeds(false), m_bFiring_Seeds(false), m_bCreating_Objects(false)
	, m_dwCreateTime(GetTickCount()), m_bCreate(false), m_eObject(BOOMERANG), m_iSeedCnt(0)
	, m_bCreateEffect(false), m_bAcornEffect(false)
{
}


CCagney::~CCagney()
{
	Release();
}

void CCagney::Initialize()
{
	// 보스 Cagney - 600 * 700
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Intro/Intro.bmp", L"Cagney_Intro");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Idle/Idle.bmp", L"Cagney_Idle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Firing_Seeds/Firing_Seeds.bmp", L"Firing_Seeds");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Creating_Object/Create.bmp", L"Creating_Object");
	// Face_Attack_Low - 860 * 450
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Face_Attack/FA_Low.bmp", L"Face_Attack_Low");
	// Face_Attack_High - 860 * 600
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Face_Attack/FA_High.bmp", L"Face_Attack_High");


	m_tInfo.fX = 1000.f;
	m_tInfo.fY = 400.f;

	m_tInfo.iMaxHp = 500;
	m_tInfo.iHp = m_tInfo.iMaxHp;

	m_eGroup = GROUPID::OBJECT;

	m_tInfo.iCX = 450;
	m_tInfo.iCY = 550;

	m_ePreScene = INTRO;
	m_eCurScene = INTRO;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
	m_pFrameKey = L"Cagney_Intro";
}

int CCagney::Update()
{
	if (m_bDead)
		return OBJ_DEAD;


	if (GetAsyncKeyState('W'))
		m_tInfo.iHp = 10;
	

	// Face Attack Low
	if (m_bIntro && m_tInfo.iHp >= 400 && m_tInfo.iHp < 480)
	{
		if (m_bFace_Low)
		{
			m_eCurScene = CCagney::IDLE;
			m_pFrameKey = L"Cagney_Idle";
		}
		else
			Face_Attack_Low();
	}

	// Firing Seeds
	else if (m_tInfo.iHp >= 300 && m_tInfo.iHp < 400)
	{
		if (m_bFiring_Seeds)
		{
			m_eCurScene = CCagney::IDLE;
			m_pFrameKey = L"Cagney_Idle";
		}
		else
			Firing_Seeds();
	}

	// Creating Object
	else if (m_tInfo.iHp >= 200 && m_tInfo.iHp < 300)
		Create_Object();
	
	// Face Attack High
	else if (m_tInfo.iHp >= 150 && m_tInfo.iHp < 200)
	{
		if (m_bFace_High)
		{
			m_eCurScene = CCagney::IDLE;
			m_pFrameKey = L"Cagney_Idle";
		}
		else
			Face_Attack_High();
	}

	// 암것도 아니면 Idle
	else
	{
		m_eCurScene = CCagney::IDLE;
		m_pFrameKey = L"Cagney_Idle";
	}

	if (m_tInfo.iHp < 150)
		m_bDead = true;

	Scene_Change();
	Frame_Move();
	Update_Rect();
	Update_CollisionRect();

	return OBJ_NOEVENT;
}

void CCagney::Late_Update()
{
}

void CCagney::Render(HDC _DC)
{
	Update_Rect();

	if (!m_bIntro)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cagney_Intro");

		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 600, m_tFrame.iFrameScene * 700
			, 600, 700
			, RGB(40, 40, 40));

		if (m_tFrame.iFrameScene == 3 && m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
		{
			m_bIntro = true;
			m_eCurScene = CCagney::IDLE;
			m_pFrameKey = L"Cagney_Idle";
		}

		return;
	}

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	switch (m_eCurScene)
	{
	case CCagney::FACE_ATTACK_LOW:
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 860, m_tFrame.iFrameScene * 450
			, 860, 450
			, RGB(40, 40, 40));
		break;
	case CCagney::FACE_ATTACK_HIGH:
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 860, m_tFrame.iFrameScene * 600
			, 860, 600
			, RGB(40, 40, 40));
		break;
	default:
		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 600, m_tFrame.iFrameScene * 700
			, 600, 700
			, RGB(40, 40, 40));
		break;
	}
}


void CCagney::Release()
{
}

void CCagney::Frame_Move()
{
	switch (m_eCurScene)
	{
	case CCagney::IDLE:
	case CCagney::INTRO:
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				if (m_tFrame.iFrameScene >= 3)
				{
					m_tFrame.iFrameScene = 0;
					m_tFrame.iFrameStart = 0;
				}
				else
				{
					++m_tFrame.iFrameScene;
					m_tFrame.iFrameStart = 0;
				}
			}

			m_tFrame.dwFrameTime = GetTickCount();
		}
		break;
		
	case CCagney::FACE_ATTACK_LOW:
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_bFace_Low = true;
				m_eCurScene = CCagney::IDLE;
				m_pFrameKey = L"Cagney_Idle";
			}
			//else
			//	m_bCollision = false;

			m_tFrame.dwFrameTime = GetTickCount();
		}
		break;

	case CCagney::FACE_ATTACK_HIGH:
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_bFace_High = true;
				m_eCurScene = CCagney::IDLE;
				m_pFrameKey = L"Cagney_Idle";
			}
			//else
			//	m_bCollision = false;

			m_tFrame.dwFrameTime = GetTickCount();
		}
		break;

	case CCagney::FIRING_SEEDS:
		// 5 ~ 14 반복
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
		{
			if (m_iSeedCnt < 6)
			{
				++m_tFrame.iFrameStart;
				if (m_tFrame.iFrameStart > 14)
				{
					m_tFrame.iFrameStart = 5;
					++m_iSeedCnt;
				}
			}
			else
			{
				++m_tFrame.iFrameStart;
				if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				{
					m_bFiring_Seeds = true;
					m_eCurScene = IDLE;
				}
			}

			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_bFiring_Seeds = true;
				m_eCurScene = CCagney::IDLE;
				m_pFrameKey = L"Cagney_Idle";
			}

			m_tFrame.dwFrameTime = GetTickCount();
		}
		break;
		
	case CCagney::CREATE_OBJECT:
		// 만드는거 10 ~ 23
		if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				if (!m_bCreating_Objects)
				{
					m_tFrame.iFrameStart = 10;
					m_bCreate = false;
				}
				m_bCreateEffect = false;
			}

			m_tFrame.dwFrameTime = GetTickCount();
		}
		break;
	}

}


void CCagney::Face_Attack_Low()
{
	// Firing_Seeds() 전에 LOW 한번
	m_eCurScene = CCagney::FACE_ATTACK_LOW;
	m_pFrameKey = L"Face_Attack_Low";


	// 16 ~ 24 크기 늘이기
	if (m_tFrame.iFrameStart >= 16 && m_tFrame.iFrameStart <= 24)
	{
		m_tInfo.fX = 600.f;
		m_tInfo.iCX = 1200;
		m_tInfo.fY = 450.f;
	}

	else
	{	
		m_tInfo.fX = 1000.f;
		m_tInfo.fY = 400.f;
		m_tInfo.iCX = 450;
	}
}

void CCagney::Face_Attack_High()
{
	// Firing_Seeds() 후에 HIGH 한번
	m_eCurScene = CCagney::FACE_ATTACK_HIGH;
	m_pFrameKey = L"Face_Attack_High";


	if (m_tFrame.iFrameStart >= 16 && m_tFrame.iFrameStart <= 24)
	{
		m_tInfo.fX = 600.f;
		m_tInfo.iCX = 1200;
		m_tInfo.fY = 450.f;
	}
	else
	{
		m_tInfo.fX = 1000.f;
		m_tInfo.fY = 400.f;
		m_tInfo.iCX = 450;
	}
}

void CCagney::Firing_Seeds()
{
	m_eCurScene = CCagney::FIRING_SEEDS;
	m_pFrameKey = L"Firing_Seeds";

	CObj* pObj = nullptr;

	// 1. 씨앗을 뿌림
	if (!m_bCreateSeeds && m_tFrame.iFrameStart == 10)
	{
		// Chomper
		pObj = CAbstractFactory<CSeed>::Create(100.f, -100.f);
		CObjMgr::Get_Instance()->Add_Object(OBJID::BACK, pObj);
		dynamic_cast<CSeed*>(pObj)->Set_Monster(1);

		// Mini Flower
		pObj = CAbstractFactory<CSeed>::Create(600.f, -700.f);
		CObjMgr::Get_Instance()->Add_Object(OBJID::BACK, pObj);
		dynamic_cast<CSeed*>(pObj)->Set_Monster(2);

		pObj = CAbstractFactory<CSeed>::Create(700.f, -500.f);
		CObjMgr::Get_Instance()->Add_Object(OBJID::BACK, pObj);
		dynamic_cast<CSeed*>(pObj)->Set_Monster(1);

		// Venus Flytrap
		pObj = CAbstractFactory<CSeed>::Create(400.f, -300.f);
		CObjMgr::Get_Instance()->Add_Object(OBJID::BACK, pObj);
		dynamic_cast<CSeed*>(pObj)->Set_Monster(2);

		pObj = CAbstractFactory<CSeed>::Create(200.f, -800.f);
		CObjMgr::Get_Instance()->Add_Object(OBJID::BACK, pObj);
		dynamic_cast<CSeed*>(pObj)->Set_Monster(3);

		m_bCreateSeeds = true;
	}
}

void CCagney::Create_Object()
{
	m_eCurScene = CCagney::CREATE_OBJECT;
	m_pFrameKey = L"Creating_Object";

	// 10 ~ 12 느리게
	if (m_tFrame.iFrameStart >= 10 && m_tFrame.iFrameStart <= 12)
		m_tFrame.dwFrameSpeed = 200;
	else
		m_tFrame.dwFrameSpeed = 80;


	if (m_tFrame.iFrameStart == 17)
	{
		if (!m_bCreate)
		{
			if (m_eObject == CCagney::BOOMERANG)
			{
				CObj* pObj = CAbstractFactory<CBoomerang>::Create(m_tInfo.fX - 200.f, m_tInfo.fY - 100.f);
				CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_BACK, pObj);

				m_eObject = ACORN;
				m_bCreate = true;
				
			}

			else
			{
				CObj* pObj = CAbstractFactory<CAcorn>::Create(m_tInfo.fX - 200.f, m_tInfo.fY - 100.f);
				CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_BACK, pObj);

				m_eObject = BOOMERANG;
				m_bCreate = true;
			}

		}

		if (!m_bCreateEffect)
		{
			CObj* pObj = CAbstractFactory<CCagneyCreateEffect>::Create(m_tInfo.fX - 150.f, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);

			m_bCreateEffect = true;
		}
	}
} 


void CCagney::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case CCagney::INTRO:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Cagney_Intro";
			break;
		case CCagney::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Cagney_Idle";
			break;
		// 공격 패턴
		case CCagney::FACE_ATTACK_LOW:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 29;		// 19
			m_tFrame.iFrameScene = 0;	// 3
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Face_Attack_Low";
			break;
		case CCagney::FACE_ATTACK_HIGH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 29;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Face_Attack_High";
			break;
		case CCagney::FIRING_SEEDS:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 21;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Firing_Seeds";
			break;
		case CCagney::CREATE_OBJECT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 26;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Creating_Object";
			break;
		}
		m_ePreScene = m_eCurScene;
	}
}

void CCagney::Update_CollisionRect()
{
	m_tCollisionRect.left = (LONG)(m_tInfo.fX - 30.f);
	//m_tCollisionRect.top = m_tInfo.fY - 250.f;
	//m_tCollisionRect.right = m_tInfo.fX + 30;
	//m_tCollisionRect.bottom = m_tInfo.fY + 250.f;
}

void CCagney::Set_Idle(int _iFirst, int _iEnd)
{
	if (m_eCurScene != CCagney::IDLE 
		&& m_bIntro && m_tInfo.iHp >= _iFirst && m_tInfo.iHp < _iEnd)
	{
		m_eCurScene = CCagney::IDLE;
		m_pFrameKey = L"Cagney_Idle";
	}
}
