#pragma once

#ifndef __DICE_H__
#define __DICE_H__

#include "Obj.h"
class CDice : public CObj
{
public:
	CDice();
	virtual ~CDice();

public:
	enum STATE { IDLE, CLAP, ARM_LEFT, ARM_RIGHT, DEATH, END };

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

private:
	void Scene_Change();

private:
	void Attack_Left();
	void Attack_Right();
	void Attack_Chalice();
	

private:
	STATE	m_ePreScene;
	STATE	m_eCurScene;

	CObj*	m_pRealDice;

	int		m_iSizeX;
	int		m_iSizeY;

	bool	m_bIntro;
	bool	m_bClap;
	bool	m_bCreateCard_Left;
	bool	m_bCreateCard_Right;
	bool	m_bCreateChalice;
	bool	m_bBravo;

	DWORD	m_dwAttackTime;
	int		m_iAttackNum;

	DWORD	m_dwDeathEffect;
	bool	m_bDeathSound;
};


#endif