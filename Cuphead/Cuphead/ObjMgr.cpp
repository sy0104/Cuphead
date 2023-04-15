#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "CollisionMgr.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;


CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Add_Object(OBJID::ID _eID, CObj * _pObj)
{
	m_listObj[_eID].emplace_back(_pObj);
}

void CObjMgr::Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		auto& iter = m_listObj[i].begin();

		for (; iter != m_listObj[i].end(); )
		{
			int iEvent = (*iter)->Update();
			if (iEvent == OBJ_DEAD)
			{
				SAFE_DELETE(*iter);
				iter = m_listObj[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CObjMgr::Late_Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			pObj->Late_Update();
			if (m_listObj[i].empty())
			{
				for (int i = 0; i < GROUPID::END; ++i)
					m_listRender[i].clear();
				break;
			}

			GROUPID::ID      eID = pObj->Get_GroupID();
			m_listRender[eID].emplace_back(pObj);
		}
	}
	
}

void CObjMgr::Render(HDC _DC)
{
	for (int i = 0; i < GROUPID::END; ++i)
	{
		if (i == GROUPID::OBJECT)
			m_listRender[i].sort(CompareX<CObj*>);

		for (auto& pObj : m_listRender[i])
			pObj->Render(_DC);

		m_listRender[i].clear();
	}
}

void CObjMgr::Release()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for_each(m_listObj[i].begin(), m_listObj[i].end(), Safe_Delete<CObj*>);
		m_listObj[i].clear();
	}
}

CObj* CObjMgr::Get_Player()
{
	if (m_listObj[OBJID::PLAYER].empty())
		return m_listObj[OBJID::WORLD_PLAYER].front();

	else
		return m_listObj[OBJID::PLAYER].front();
}


CObj* CObjMgr::Get_Target(OBJID::ID _eID, CObj * _pObj)
{
	if (m_listObj[_eID].empty())
		return nullptr;

	CObj*	pTarget = nullptr;
	float	fDis = 0.f;

	for (auto& obj : m_listObj[_eID])
	{
		float fX = _pObj->Get_Info().fX - obj->Get_Info().fX;
		float fY = _pObj->Get_Info().fY - obj->Get_Info().fY;
		float fDia = sqrtf(fX * fX + fY * fY);

		if (!pTarget || fDis > fDia)
		{
			pTarget = obj;
			fDis = fDia;
		}
	}

	return pTarget;
}

void CObjMgr::Delete_ID(OBJID::ID _eID)
{
	for_each(m_listObj[_eID].begin(), m_listObj[_eID].end(), Safe_Delete<CObj*>);
	m_listObj[_eID].clear();
}
