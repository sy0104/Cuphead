#pragma once

#ifndef __PANG_H__
#define __PANG_H__

#include "Obj.h"
class CPang : public CObj
{
public:
	CPang();
	virtual ~CPang();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

private:
	bool	m_bStart;
};


#endif