#pragma once

#ifndef __MAINVINE_H__
#define __MAINVINE_H__

#include "Obj.h"
class CMainVine : public CObj
{
public:
	CMainVine();
	virtual ~CMainVine();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;
};


#endif