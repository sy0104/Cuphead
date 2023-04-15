#pragma once

#ifndef __PREBEAM_H__
#define __PREBEAM_H__

#include "Obj.h"
class CPreBeam : public CObj
{
public:
	CPreBeam();
	virtual ~CPreBeam();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;
};


#endif