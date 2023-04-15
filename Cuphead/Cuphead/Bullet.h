#pragma once

#ifndef __BULLET_H__
#define __BULLET_H__

#include "Obj.h"
class CBullet : public CObj
{
public:
	CBullet();
	virtual ~CBullet();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	bool	m_bCheck;
	bool	m_bBulletEffect;
};

#endif