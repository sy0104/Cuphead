#pragma once

#ifndef __CARROTBULLET_H__
#define __CARROTBULLET_H__

#include "Obj.h"
class CCarrotBullet : public CObj
{
public:
	CCarrotBullet();
	virtual ~CCarrotBullet();

public:
	enum STATE { IDLE, DEAD, END };

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
	POINT	m_tPos[3];		// 이미지 회전 좌표

	STATE	m_ePreScene;
	STATE	m_eCurScene;
};


#endif