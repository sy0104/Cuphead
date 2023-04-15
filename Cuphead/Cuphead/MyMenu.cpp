#include "stdafx.h"
#include "MyMenu.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"


CMyMenu::CMyMenu()
{
}


CMyMenu::~CMyMenu()
{
	Release();
}

void CMyMenu::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Menu.bmp", L"Menu");
}

void CMyMenu::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CMyMenu::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();

	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::MAP);
}

void CMyMenu::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Menu");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(_DC);
}

void CMyMenu::Release()
{
}
