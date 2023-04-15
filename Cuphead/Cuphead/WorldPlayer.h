#pragma once

#ifndef __WORLDPLAYER_H__
#define __WORLDPLAYER_H__

#include "Obj.h"
class CWorldPlayer : public CObj
{
public:
	CWorldPlayer();
	virtual ~CWorldPlayer();

public:
	enum STATE { WALK_UP
		, WALK_DIA_RIGHTUP, IDLE_RIGHT, WALK_RIGHT, WALK_DIA_RIGHTDOWN
		, IDLE, WALK_DOWN
		, WALK_DIA_LEFTUP, IDLE_LEFT, WALK_LEFT, WALK_DIA_LEFTDOWN
		, ENTER, END };

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

private:
	void Key_Check();
	void OffSet();
	void Scene_Change();


private:
	STATE	m_ePreScene;
	STATE	m_eCurScene;
};


#endif