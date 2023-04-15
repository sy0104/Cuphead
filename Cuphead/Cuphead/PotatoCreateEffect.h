#pragma once

#ifndef __POTATOCREATEEFFECT_H__
#define __POTATOCREATEEFFECT_H__

#include "Obj.h"
class CPotatoCreateEffect : public CObj
{
public:
	CPotatoCreateEffect();
	virtual ~CPotatoCreateEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;
};


#endif