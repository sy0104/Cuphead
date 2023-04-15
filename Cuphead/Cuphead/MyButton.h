#pragma once

#ifndef __MYBUTTON_H__
#define __MYBUTTON_H__

#include "Obj.h"
class CMyButton : public CObj
{
public:
	CMyButton();
	virtual ~CMyButton();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};


#endif