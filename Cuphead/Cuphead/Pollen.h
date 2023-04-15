#pragma once

#ifndef __POLLEN_H__
#define __POLLEN_H__

#include "Obj.h"
class CPollen : public CObj
{
public:
	CPollen();
	virtual ~CPollen();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

public:
	// 1: Whilte, 2: Pink
	void Set_Color(int _iColor) { m_iColor = _iColor; }

private:
	int		m_iColor;
	float	m_fFirstY;
	float	m_fSpeedY;
};


#endif