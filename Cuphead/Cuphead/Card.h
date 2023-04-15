#pragma once

#ifndef __CARD_H__
#define __CARD_H__

#include "Obj.h"
class CCard : public CObj
{
public:
	CCard();
	virtual ~CCard();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

public:
	void Set_Dir(MONSTER::MON_DIR _eDir) { m_eCardDir = _eDir; }

private:
	void Update_CollisionRect();

private:
	MONSTER::MON_DIR	m_eCardDir;
};


#endif