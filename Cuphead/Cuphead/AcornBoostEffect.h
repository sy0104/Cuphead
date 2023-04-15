#pragma once

#ifndef __ACORNBOOSTEFFECT_H__
#define __ACORNBOOSTEFFECT_H__

#include "Obj.h"
class CAcornBoostEffect : public CObj
{
public:
	CAcornBoostEffect();
	virtual ~CAcornBoostEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;
};


#endif