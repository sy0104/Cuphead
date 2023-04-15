#pragma once

#ifndef __BACKSOIL_H__
#define __BACKSOIL_H__

#include "Obj.h"
class CBackSoil : public CObj
{
public:
	CBackSoil();
	virtual ~CBackSoil();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};


#endif