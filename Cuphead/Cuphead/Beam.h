#pragma once

#ifndef __BEAM_H__
#define __BEAM_H__

#include "Obj.h"
class CBeam : public CObj
{
public:
	CBeam();
	virtual ~CBeam();

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