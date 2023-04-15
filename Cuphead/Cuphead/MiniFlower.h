#pragma once

#ifndef __MINIFLOWER_H__
#define __MINIFLOWER_H__

#include "Obj.h"
class CMiniFlower : public CObj
{
public:
	CMiniFlower();
	virtual ~CMiniFlower();

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
	STATE		m_ePreScene;
	STATE		m_eCurScene;

	int			m_iFlyCnt;

	bool		m_bFlyUp;
	bool		m_bFirst;

};


#endif