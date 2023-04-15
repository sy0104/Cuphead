#pragma once

#ifndef __BRAVO_H__
#define __BRAVO_H__

#include "Obj.h"
class CBravo : public CObj
{
public:
	CBravo();
	virtual ~CBravo();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;
};

#endif