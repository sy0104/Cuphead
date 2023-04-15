#pragma once

#ifndef __CHOMPER_H__
#define __CHOMPER_H__

#include "Obj.h"
class CChomper : public CObj
{
public:
	CChomper();
	virtual ~CChomper();

public:
	enum STATE { IDLE, DIE, END };

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
	STATE	m_eCurScene;
	STATE	m_ePreScene;

	bool	m_bDeath;
};

#endif
