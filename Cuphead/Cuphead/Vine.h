#pragma once

#ifndef __VINE_H__
#define __VINE_H__

#include "Obj.h"
class CVine : public CObj
{
public:
	CVine();
	virtual ~CVine();

public:
	enum MONSTER { CHOMPER, MF, VF, END };

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

public:
	void Set_Monster(int  _iMonster) { m_iMonster = _iMonster; }

private:
	bool		m_bGrowUp;
	bool		m_bCreate;

	int			m_iMonster;
};


#endif