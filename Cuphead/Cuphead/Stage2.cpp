#include "stdafx.h"
#include "Stage2.h"
#include "Player.h"
#include "Cagney.h"
#include "FinalCagney.h"
#include "Platform.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "PlayerHp.h"
#include "SoundMgr.h"


CStage2::CStage2()
	: m_pCagney(nullptr), m_pFinalCagney(nullptr), m_bCreate(false), m_pPlayer(nullptr), m_bCollision(false)
{
}


CStage2::~CStage2()
{
	Release();
}

void CStage2::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cagney_Carnation/Background.bmp", L"Stage2_Background");

	m_pPlayer = CAbstractFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, m_pPlayer);

	m_pCagney = CAbstractFactory<CCagney>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::BOSS, m_pCagney);

	CSoundMgr::Get_Instance()->PlaySound(L"StageIntro.mp3", CSoundMgr::EFFECT);
	CSoundMgr::Get_Instance()->PlayBGM(L"CagneyBgm.mp3");


	// ÇÃ·§Æû
	CObj* pObj = CAbstractFactory<CPlatform>::Create(150.f, 400.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLATFORM, pObj);

	pObj = CAbstractFactory<CPlatform>::Create(400.f, 420.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLATFORM, pObj);

	pObj = CAbstractFactory<CPlatform>::Create(650.f, 400.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLATFORM, pObj);

	pObj = CAbstractFactory<CPlayerHp>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::BACK, pObj);
}

void CStage2::Update()
{
	CObjMgr::Get_Instance()->Update();

	if (m_pCagney->Get_Dead() && !m_bCreate)
	{
		m_pFinalCagney = CAbstractFactory<CFinalCagney>::Create();
		CObjMgr::Get_Instance()->Add_Object(OBJID::BOSS, m_pFinalCagney);

		m_bCreate = true;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_BACK))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::MAP);
	}

	else if (CKeyMgr::Get_Instance()->Key_Down('Y'))
		m_bCollision = true;
	else if (CKeyMgr::Get_Instance()->Key_Down('T'))
		m_bCollision = false;

	CCollisionMgr::Collision_Platform2(CObjMgr::Get_Instance()->Get_List(OBJID::PLATFORM), CObjMgr::Get_Instance()->Get_List(OBJID::PLAYER));


	// Bullet & Cagney Ãæµ¹Ã¼Å©
	if (m_pCagney && !m_pFinalCagney)
	{
		CCollisionMgr::Collision_Bullet_To_Cagney(CObjMgr::Get_Instance()->Get_List(OBJID::BULLET), m_pCagney);
		//CCollisionMgr::Collision_Player_To_Boss(m_pPlayer, m_pCagney);
	}
	else if (m_pFinalCagney)
	{
		CCollisionMgr::Collision_Bullet_To_Cagney2(CObjMgr::Get_Instance()->Get_List(OBJID::BULLET), m_pFinalCagney);
		//CCollisionMgr::Collision_Player_To_Boss(m_pPlayer, m_pFinalCagney);
	}

	CCollisionMgr::Collision_Bullet_To_Monster(CObjMgr::Get_Instance()->Get_List(OBJID::MONSTER), CObjMgr::Get_Instance()->Get_List(OBJID::BULLET));

	if (!m_bCollision)
	{
		CCollisionMgr::Collision_Player_To_Monster(CObjMgr::Get_Instance()->Get_List(OBJID::MONSTER), CObjMgr::Get_Instance()->Get_List(OBJID::PLAYER));
		CCollisionMgr::Collision_Player_To_MonsterBack(CObjMgr::Get_Instance()->Get_List(OBJID::MONSTER_BACK), CObjMgr::Get_Instance()->Get_List(OBJID::PLAYER));
	}
	

}


void CStage2::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage2::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Stage2_Background");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(_DC);
}

void CStage2::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJID::PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::BOSS);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::PLATFORM);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER_BACK);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::BACK);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}
