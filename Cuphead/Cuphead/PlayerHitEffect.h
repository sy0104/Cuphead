#pragma once

#ifndef __PLAYERHITEFFECT_H__
#define __PLAYERHITEFFECT_H__

#include "Obj.h"
class CPlayerHitEffect : public CObj
{
public:
	CPlayerHitEffect();
	virtual ~CPlayerHitEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;
};

#endif