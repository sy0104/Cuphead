#pragma once

#ifndef __OBJMGR_H__
#define __OBJMGR_H__

class CObj;

class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	void Add_Object(OBJID::ID _eID, CObj* _pObj);
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	CObj* Get_Player();
	CObj* Get_Target(OBJID::ID _eID, CObj* _pObj);
	list<CObj*>& Get_List(OBJID::ID _eID) { return m_listObj[_eID]; }

public:
	void Delete_ID(OBJID::ID _eID);

public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	list<CObj*>			m_listObj[OBJID::END];
	list<CObj*>			m_listRender[GROUPID::END];
	static CObjMgr*		m_pInstance;

};


#endif