#include "stdafx.h"
#include "Logo.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"


CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Logo.bmp", L"Logo");

	CSoundMgr::Get_Instance()->PlayBGM(L"MenuBgm.mp3");

}

void CLogo::Update()
{
}

void CLogo::Late_Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::MENU);
}

void CLogo::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Logo");

	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLogo::Release()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}
