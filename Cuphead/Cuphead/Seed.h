#pragma once

#ifndef __SEED_H__
#define __SEED_H__

#include "Obj.h"
class CSeed : public CObj
{
public:
	CSeed();
	virtual ~CSeed();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

public:
	void Set_Monster(int _iMonster) { m_iMonster = _iMonster; }

private:
	float	m_fSpeedX;
	bool	m_bCheck;

	DWORD	m_dwTime;

	int		m_iMonster;

};


#endif