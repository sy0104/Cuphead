#pragma once

#ifndef __FINALCAGNEY_H__
#define __FINALCAGNEY_H__

#include "Obj.h"
class CFinalCagney : public CObj
{
public:
	CFinalCagney();
	virtual ~CFinalCagney();

public:
	enum STATE { INTRO, IDLE, ATTACK, DEAD, END };

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;
	
private:
	void Scene_Change();
	void Update_CollisionRect();

private:
	void Create_Vine();
	void Attack_Vine();
	void Attack_Pollen();

private:
	STATE	m_ePreScene;
	STATE	m_eCurScene;

	bool	m_bIntro;
	bool	m_bCreateVine;
	bool	m_bAttackVine;
	bool	m_bCreatePollen;

	int		m_iDeathCnt;

	DWORD	m_dwCreateVine;
	DWORD	m_dwCreatePollen;
	int		m_iVinePos;
	int		m_iPollenColor;

	bool	m_bBravo;

	DWORD	m_dwDeathEffect;
	bool	m_bDeathSound;
};


#endif