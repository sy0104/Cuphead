#include "stdafx.h"
#include "Dice.h"
#include "RealDice.h"
#include "Card.h"
#include "Chalice.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "KeyMgr.h"
#include "Bravo.h"
#include "BossDeathEffect.h"
#include "SoundMgr.h"


CDice::CDice()
	: m_ePreScene(END), m_eCurScene(END), m_iSizeX(0), m_iSizeY(0), m_bIntro(false)
	, m_bCreateCard_Left(false), m_bCreateCard_Right(false), m_bCreateChalice(false)
	, m_dwAttackTime(GetTickCount()), m_pRealDice(nullptr), m_bClap(false), m_iAttackNum(1), m_bBravo(false)
	, m_dwDeathEffect(GetTickCount()), m_bDeathSound(false)
{
}


CDice::~CDice()
{
	Release();
}

void CDice::Initialize()
{
	//// Intro - 993 * 575 (47) 1000 * 570
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Intro.bmp", L"Dice_Intro");
	// Idle - 950 * 478 (32)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Idle.bmp", L"Dice_Idle");
	// Clap - 1170 * 645 (53)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Clap.bmp", L"Dice_Clap");
	// Death - 740 * 640 (15)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Death.bmp", L"Dice_Death");

	m_tInfo.fX = 630.f;
	m_tInfo.fY = 180.f;

	m_tInfo.iCX = 1000;
	m_tInfo.iCY = 400;
	m_eGroup = GROUPID::OBJECT;


	m_iSizeX = 950;
	m_iSizeY = 478;

	m_ePreScene = IDLE;
	m_eCurScene = IDLE;

	m_tInfo.iMaxHp = 150;
	m_tInfo.iHp = m_tInfo.iMaxHp;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 32;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Dice_Idle";
}

int CDice::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	else if (GetAsyncKeyState('W'))
	{
		m_eCurScene = DEATH;
		m_pFrameKey = L"Dice_Death";

		if (!m_bBravo)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Knockout.mp3", CSoundMgr::EFFECT);

			CObj* pObj = CAbstractFactory<CBravo>::Create(600.f, 350.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);
			m_bBravo = true;
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
		m_eCurScene = CLAP;

	Scene_Change();
	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CDice::Late_Update()
{
}

void CDice::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * m_iSizeX, m_tFrame.iFrameScene * m_iSizeY
		, m_iSizeX, m_iSizeY
		, RGB(40, 40, 40));
}

void CDice::Release()
{
}

void CDice::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		if (m_eCurScene == IDLE)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart == 0)
				m_bIntro = false;

			else if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				if (!m_bIntro)
				{
					m_eCurScene = CLAP;
					m_bIntro = true;
				}
				else
					m_tFrame.iFrameStart = 0;
			}
		}

		else if (m_eCurScene == CLAP)	// 33에 주사위 생성
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart == 33)
			{
				m_pRealDice = CAbstractFactory<CRealDice>::Create();
				CObjMgr::Get_Instance()->Add_Object(OBJID::DICE, m_pRealDice);
			}

			else if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_eCurScene = IDLE;
				m_pFrameKey = L"Dice_Idle";

				if (!m_bCreateCard_Left && !m_bCreateCard_Right && !m_bCreateChalice)
					Attack_Left();
				else if (m_bCreateCard_Left && !m_bCreateCard_Right && !m_bCreateChalice)
					Attack_Chalice();
				else if (m_bCreateCard_Left && !m_bCreateCard_Right && m_bCreateChalice)
					Attack_Right();
			}
		}

		else if (m_eCurScene == DEATH)
		{
			++m_tFrame.iFrameStart;
			if (m_dwDeathEffect + 300 < GetTickCount())
			{
				CObj* pObj = CAbstractFactory<CBossDeathEffect>::Create((float)(rand() % 1200), (float)(rand() % 300 + 200));
				CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);

				m_dwDeathEffect = GetTickCount();
			}

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;
		}

		else
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_tFrame.iFrameStart = 0;
			}
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CDice::Scene_Change()
{
	if (m_eCurScene != m_ePreScene)
	{
		switch (m_eCurScene)
		{
		case CDice::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 32;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Dice_Idle";
			m_tInfo.fX = 630.f;
			m_tInfo.fY = 180.f;
			m_tInfo.iCX = 1000;
			m_tInfo.iCY = 400;
			m_iSizeX = 950;
			m_iSizeY = 478;
			break;
		case CDice::CLAP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 62;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Dice_Clap";
			m_tInfo.fX = 610.f;
			m_tInfo.fY = 280.f;
			m_tInfo.iCX = 1000;
			m_tInfo.iCY = 500;
			m_iSizeX = 1000;
			m_iSizeY = 640;
			break;
		case CDice::DEATH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 14;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Dice_Death";
			m_tInfo.fX = 610.f;
			m_tInfo.fY = 325.f;
			m_tInfo.iCX = 900;
			m_tInfo.iCY = 500;
			m_iSizeX = 740;
			m_iSizeY = 640;
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}


void CDice::Attack_Left()
{
	// Card - 230 * 220
	// y는 550.f 고정
	// 다이어 -> 하트 -> 스페이드 -> 클러브

	m_eCurScene = IDLE;

	if (m_eCurScene == IDLE && m_tFrame.iFrameStart > 5)
	{

		CObj* pObj = nullptr;

		if (!m_bCreateCard_Left)
		{
			pObj = CAbstractFactory<CCard>::Create_Card(-0.f, 550.f, L"Card_Diamond");
			CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pObj);
			dynamic_cast<CCard*>(pObj)->Set_Dir(MONSTER::LEFT);

			pObj = CAbstractFactory<CCard>::Create_Card(-150.f, 550.f, L"Card_Heart");
			CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pObj);
			dynamic_cast<CCard*>(pObj)->Set_Dir(MONSTER::LEFT);
			pObj->Set_Parry();

			pObj = CAbstractFactory<CCard>::Create_Card(-300.f, 550.f, L"Card_Space");
			CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pObj);
			dynamic_cast<CCard*>(pObj)->Set_Dir(MONSTER::LEFT);

			pObj = CAbstractFactory<CCard>::Create_Card(-450.f, 550.f, L"Card_Heart");
			CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pObj);
			dynamic_cast<CCard*>(pObj)->Set_Dir(MONSTER::LEFT);
			pObj->Set_Parry();

			m_bCreateCard_Left = true;
		}
	}
}

void CDice::Attack_Right()
{
	// Card - 230 * 220
	// y는 550.f 고정
	// 다이어 -> 하트 -> 스페이드 -> 클러브

	m_eCurScene = IDLE;

	CObj* pObj = nullptr;

	if (!m_bCreateCard_Right)
	{
		pObj = CAbstractFactory<CCard>::Create_Card(1200.f, 550.f, L"Card_Space");
		CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pObj);
		dynamic_cast<CCard*>(pObj)->Set_Dir(MONSTER::RIGHT);

		pObj = CAbstractFactory<CCard>::Create_Card(1350.f, 550.f, L"Card_Heart");
		CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pObj);
		dynamic_cast<CCard*>(pObj)->Set_Dir(MONSTER::RIGHT);
		pObj->Set_Parry();

		pObj = CAbstractFactory<CCard>::Create_Card(1500.f, 550.f, L"Card_Club");
		CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pObj);
		dynamic_cast<CCard*>(pObj)->Set_Dir(MONSTER::RIGHT);

		pObj = CAbstractFactory<CCard>::Create_Card(1650.f, 550.f, L"Card_Heart");
		CObjMgr::Get_Instance()->Add_Object(OBJID::CARD, pObj);
		dynamic_cast<CCard*>(pObj)->Set_Dir(MONSTER::RIGHT);
		pObj->Set_Parry();

		m_bCreateCard_Right = true;
	}

}

void CDice::Attack_Chalice()
{
	m_eCurScene = IDLE;

	if (!m_bCreateChalice)
	{
		CObj* pObj = CAbstractFactory<CChalice>::Create(1100.f, 500.f);
		CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pObj);

		m_bCreateChalice = true;
	}
}
