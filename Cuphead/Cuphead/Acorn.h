#pragma once

#ifndef __ACORN_H__
#define __ACORN_H__

#include "Obj.h"
class CAcorn : public CObj
{
public:
	CAcorn();
	virtual ~CAcorn();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

private:
	bool	m_bCheck;
	bool	m_bBoostEffect;

};


#endif