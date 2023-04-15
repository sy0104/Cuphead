#pragma once

#ifndef __EARTHWORM_H__
#define __EARTHWORM_H__

#include "Obj.h"
class CEarthworm : public CObj
{
public:
	CEarthworm();
	virtual ~CEarthworm();

public:
	enum STATE { IDLE, DEATH, END };

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
	STATE	m_ePreScene;
	STATE	m_eCurScene;

};


#endif