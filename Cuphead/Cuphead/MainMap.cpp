#include "stdafx.h"
#include "MainMap.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "WorldPlayer.h"
#include "MyButton.h"
#include "SoundMgr.h"


CMainMap::CMainMap()
{
}


CMainMap::~CMainMap()
{
	Release();
}

void CMainMap::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/overworld_map.bmp", L"map");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Flag_01.bmp", L"Flag_Roots");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Flag_01.bmp", L"Flag_Cagney");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Flag_01.bmp", L"Flag_Dice");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Flag_02.bmp", L"Flag_Test");

	CSoundMgr::Get_Instance()->PlayBGM(L"OverworldBgm.mp3");

	CObj* pObj = CAbstractFactory<CWorldPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::WORLD_PLAYER, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(950.f, 700.f);
	dynamic_cast<CMyButton*>(pObj)->Set_FrameKey(L"Flag_Roots");
	CObjMgr::Get_Instance()->Add_Object(OBJID::FLAG, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(1720.f, 370.f);
	dynamic_cast<CMyButton*>(pObj)->Set_FrameKey(L"Flag_Cagney");
	CObjMgr::Get_Instance()->Add_Object(OBJID::FLAG, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(1850.f, 600.f);
	dynamic_cast<CMyButton*>(pObj)->Set_FrameKey(L"Flag_Dice");
	CObjMgr::Get_Instance()->Add_Object(OBJID::FLAG, pObj);

	pObj = CAbstractFactory<CMyButton>::Create(400.f, 350.f);
	dynamic_cast<CMyButton*>(pObj)->Set_FrameKey(L"Flag_Test");
	CObjMgr::Get_Instance()->Add_Object(OBJID::FLAG, pObj);

}

void CMainMap::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CMainMap::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CMainMap::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"map");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, -iScrollX, -iScrollY, SRCCOPY);

	CObjMgr::Get_Instance()->Render(_DC);
}

void CMainMap::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJID::FLAG);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::WORLD_PLAYER);
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	//CSoundMgr::StopAll();
}
