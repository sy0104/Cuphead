#pragma once

#ifndef __MFBULLET_H__
#define __MFBULLET_H__

#include "Obj.h"
class CMFBullet : public CObj
{
public:
	CMFBullet();
	virtual ~CMFBullet();

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
	STATE	m_ePreScene;
	STATE	m_eCurScene;

	bool	m_bStart;
};


#endif