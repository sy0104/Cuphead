#pragma once

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "Obj.h"
class CPlatform : public CObj
{
public:
	CPlatform();
	virtual ~CPlatform();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

private:
	float	m_fFirstY;
	bool	m_bCollision;
};


#endif