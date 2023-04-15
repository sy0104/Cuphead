#pragma once

#ifndef __REALDICE_H__
#define __REALDICE_H__

#include "Obj.h"
class CRealDice : public CObj
{
public:
	CRealDice();
	virtual ~CRealDice();

	enum STATE { IDLE, DICE_1, DICE_2, DICE_3, END };

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

public:
	const bool Get_Next() const { return m_bNext; }

private:
	void Scene_Change();

private:
	STATE	m_ePreScene;
	STATE	m_eCurScene;

	bool	m_bIntro;
	int		m_iNum;
	bool	m_bNext;
};


#endif