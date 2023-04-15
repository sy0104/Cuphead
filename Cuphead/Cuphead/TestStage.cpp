#include "stdafx.h"
#include "TestStage.h"
#include "Player.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "ObjMgr.h"


CTestStage::CTestStage()
{
}


CTestStage::~CTestStage()
{
	Release();
}

void CTestStage::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background_Test.bmp", L"Test_Background");

	CObj* pObj = CAbstractFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pObj);
}

void CTestStage::Update()
{
	CObjMgr::Get_Instance()->Update();

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_BACK))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::MAP);
	}
}

void CTestStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CTestStage::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Test_Background");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(_DC);
}

void CTestStage::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJID::PLAYER);

}
