#pragma once

#ifndef __BOOMERANG_H__
#define __BOOMERANG_H__

#include "Obj.h"
class CBoomerang : public CObj
{
public:
	CBoomerang();
	virtual ~CBoomerang();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;
};

#endif