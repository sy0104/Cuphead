#pragma once

#ifndef __CHALICE_H__
#define __CHALICE_H__

#include "Obj.h"
class CChalice : public CObj
{
public:
	CChalice();
	virtual ~CChalice();

public:
	enum STATE { IDLE, ATTACK, TWINKLE, END };
	enum CH_DIR { LEFT, RIGHT, CH_END };

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

private:
	void Scene_Change();

private:
	void Attack();


private:
	STATE	m_ePreScene;
	STATE	m_eCurScene;

	DWORD	m_dwAttackTime;

	bool	m_bCreate;
};


#endif