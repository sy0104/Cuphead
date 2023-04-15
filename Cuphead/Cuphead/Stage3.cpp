#include "stdafx.h"
#include "Stage3.h"
#include "Player.h"
#include "Dice.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "PlayerHp.h"
#include "SoundMgr.h"


CStage3::CStage3()
{
}


CStage3::~CStage3()
{
	Release();
}

void CStage3::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/King_Dice/Background.bmp", L"Stage3_Background");

	CObj* pObj = CAbstractFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pObj);

	pObj = CAbstractFactory<CPlayerHp>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::BACK, pObj);

	m_pDice = CAbstractFactory<CDice>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::BOSS, m_pDice);

	CSoundMgr::Get_Instance()->PlaySound(L"StageIntro.mp3", CSoundMgr::EFFECT);
	CSoundMgr::Get_Instance()->PlayBGM(L"DiceBgm.mp3");

}

void CStage3::Update()
{
	CObjMgr::Get_Instance()->Update();

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_BACK))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::MAP);
	}

	// 충돌체크
	CCollisionMgr::Collision_Player_To_Monster(CObjMgr::Get_Instance()->Get_List(OBJID::MONSTER), CObjMgr::Get_Instance()->Get_List(OBJID::PLAYER));
	CCollisionMgr::Collision_Player_To_Card(CObjMgr::Get_Instance()->Get_List(OBJID::CARD), CObjMgr::Get_Instance()->Get_List(OBJID::PLAYER));
	CCollisionMgr::Collision_Bullet_To_Monster(CObjMgr::Get_Instance()->Get_List(OBJID::MONSTER), CObjMgr::Get_Instance()->Get_List(OBJID::BULLET));
	CCollisionMgr::Collision_Player_To_MonsterBack(CObjMgr::Get_Instance()->Get_List(OBJID::MONSTER_BACK), CObjMgr::Get_Instance()->Get_List(OBJID::PLAYER));

	//CCollisionMgr::Collision_Player_To_Dice(CObjMgr::Get_Instance()->Get_List(OBJID::DICE), CObjMgr::Get_Instance()->Get_List(OBJID::PLAYER));
}

void CStage3::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage3::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Stage3_Background");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(_DC);
}

void CStage3::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJID::PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::BOSS);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::BACK);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER_BACK);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::CARD);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}
