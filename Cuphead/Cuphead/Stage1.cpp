#include "stdafx.h"
#include "Stage1.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "Potato.h"
#include "Carrot.h"
#include "CollisionMgr.h"
#include "PlayerHp.h"
#include "SoundMgr.h"


CStage1::CStage1()
	: m_bWave1(false), m_bWave2(false), m_bCreate(false), m_pPotato(nullptr), m_pCarrot(nullptr)
	, m_pPlayer(nullptr), m_bCollision(false)
{
}


CStage1::~CStage1()
{
	Release();
}

void CStage1::Initialize()
{

	// ¹è°æ
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Background.bmp", L"Stage1_Background");
	// Èë - 415 * 126
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Roots/Potato/Back_Soil.bmp", L"Back_Soil");

	CSoundMgr::Get_Instance()->PlayBGM(L"RootBgm.mp3");
	CSoundMgr::Get_Instance()->PlaySound(L"StageIntro.mp3", CSoundMgr::EFFECT);


	m_pPlayer = CAbstractFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, m_pPlayer);

	CObj* pObj = CAbstractFactory<CPlayerHp>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::BACK, pObj);
}

void CStage1::Update()
{
	CObjMgr::Get_Instance()->Update();

	if (!m_bWave1)
	{
		m_pPotato = CAbstractFactory<CPotato>::Create();
		CObjMgr::Get_Instance()->Add_Object(OBJID::BOSS, m_pPotato);

		m_bWave1 = true;
	}

	if (m_pPotato->Get_Dead())
		m_bWave2 = true;

	if (m_bWave2)
	{
		if (!m_bCreate)
		{
			m_pCarrot = CAbstractFactory<CCarrot>::Create();
			CObjMgr::Get_Instance()->Add_Object(OBJID::BOSS, m_pCarrot);
		}

		m_bCreate = true;
		m_bWave2 = false;
	}


	if (CKeyMgr::Get_Instance()->Key_Pressing('F') || (m_bWave1 && m_bWave2))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::MAP);
	}

	else if (CKeyMgr::Get_Instance()->Key_Down('Y'))
		m_bCollision = true;
	else if (CKeyMgr::Get_Instance()->Key_Down('T'))
		m_bCollision = false;

	//if (m_bWave1 && !m_bWave2)
	if (m_pPotato)
	{
		CCollisionMgr::Collision_Bullet_To_Potato(CObjMgr::Get_Instance()->Get_List(OBJID::BOSS), CObjMgr::Get_Instance()->Get_List(OBJID::BULLET));
		if (!m_bCollision)
		{
			CCollisionMgr::Collision_Player_To_Monster(CObjMgr::Get_Instance()->Get_List(OBJID::MONSTER), CObjMgr::Get_Instance()->Get_List(OBJID::PLAYER));
			CCollisionMgr::Collision_Player_To_MonsterBack(CObjMgr::Get_Instance()->Get_List(OBJID::MONSTER_BACK), CObjMgr::Get_Instance()->Get_List(OBJID::PLAYER));
		}
	}
	else if (!m_pPotato && m_pCarrot)
	{
		CCollisionMgr::Collision_Bullet_To_Carrot(CObjMgr::Get_Instance()->Get_List(OBJID::BOSS), CObjMgr::Get_Instance()->Get_List(OBJID::BULLET));
		if (!m_bCollision)
		{
			CCollisionMgr::Collision_Player_To_Monster(CObjMgr::Get_Instance()->Get_List(OBJID::MONSTER), CObjMgr::Get_Instance()->Get_List(OBJID::PLAYER));
			CCollisionMgr::Collision_Player_To_MonsterBack(CObjMgr::Get_Instance()->Get_List(OBJID::MONSTER_BACK), CObjMgr::Get_Instance()->Get_List(OBJID::PLAYER));
		}
	}
	CCollisionMgr::Collision_Bullet_To_Monster(CObjMgr::Get_Instance()->Get_List(OBJID::MONSTER), CObjMgr::Get_Instance()->Get_List(OBJID::BULLET));
}

void CStage1::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage1::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Stage1_Background");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);


	CObjMgr::Get_Instance()->Render(_DC);
}

void CStage1::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJID::PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::BOSS);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::EFFECT);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::BACK);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}
