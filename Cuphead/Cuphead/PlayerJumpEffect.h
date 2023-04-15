#pragma once

#ifndef __PLAYERJUMPEFFECT_H__
#define __PLAYERJUMPEFFECT_H__

#include "Obj.h"
class CPlayerJumpEffect : public CObj
{
public:
	CPlayerJumpEffect();
	virtual ~CPlayerJumpEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;
};


#endif