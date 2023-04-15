#pragma once

#ifndef __CAGNEYCREATEEFFECT_H__
#define __CAGNEYCREATEEFFECT_H__

#include "Obj.h"
class CCagneyCreateEffect : public CObj
{
public:
	CCagneyCreateEffect();
	virtual ~CCagneyCreateEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;
};


#endif