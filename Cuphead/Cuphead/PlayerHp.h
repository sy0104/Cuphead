#pragma once

#ifndef __PLAYERHP_H__
#define __PLAYERHP_H__

#include "Obj.h"
class CPlayerHp : public CObj
{
public:
	CPlayerHp();
	virtual ~CPlayerHp();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	int		m_iDrawID;
};


#endif