#include "stdafx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "MainMap.h"
#include "MyMenu.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "TestStage.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr()
	: m_pScene(nullptr), m_ePreScene(END), m_eCurScene(END)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Update()
{
	m_pScene->Update();
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC _DC)
{
	m_pScene->Render(_DC);
}

void CSceneMgr::Release()
{
	SAFE_DELETE(m_pScene);
}

void CSceneMgr::Scene_Change(SCENEID _eID)
{
	m_eCurScene = _eID;

	if (m_ePreScene != m_eCurScene)
	{
		SAFE_DELETE(m_pScene);

		switch (m_eCurScene)
		{
		case LOGO:
			m_pScene = new CLogo;
			break;
		case MENU:
			m_pScene = new CMyMenu;
			break;
		case MAP:
			m_pScene = new CMainMap;
			break;
		case STAGE1:
			m_pScene = new CStage1;
			break;
		case STAGE2:
			m_pScene = new CStage2;
			break;
		case STAGE3:
			m_pScene = new CStage3;
			break;
		case TEST:
			m_pScene = new CTestStage;
			break;
		case EDIT:
			break;
		}
		m_pScene->Initialize();
		m_ePreScene = m_eCurScene;
	}
}
