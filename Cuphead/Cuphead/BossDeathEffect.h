#pragma once

#ifndef __BOSSDEATHEFFECT_H__
#define __BOSSDEATHEFFECT_H__

#include "Obj.h"
class CBossDeathEffect : public CObj
{
public:
	CBossDeathEffect();
	virtual ~CBossDeathEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;
};


#endif