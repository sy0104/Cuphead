#pragma once

#ifndef __BULLETEFFECT_H__
#define __BULLETEFFECT_H__

#include "Obj.h"
class CBulletEffect : public CObj
{
public:
	CBulletEffect();
	virtual ~CBulletEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;
};


#endif