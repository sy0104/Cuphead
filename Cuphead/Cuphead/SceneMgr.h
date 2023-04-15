#pragma once

#ifndef __SCENEMGR_H__
#define __SCENEMGR_H__

class CScene;

class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();

public:
	enum SCENEID { LOGO, MENU, MAP, STAGE1, STAGE2, STAGE3, TEST, EDIT, END };

public:
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Scene_Change(SCENEID _eID);

public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}


private:
	static CSceneMgr*	m_pInstance;
	CScene*				m_pScene;
	SCENEID				m_ePreScene;
	SCENEID				m_eCurScene;
};


#endif