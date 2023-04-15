#pragma once

#ifndef __CARROT_H__
#define __CARROT_H__

#include "Obj.h"
class CCarrot : public CObj
{
public:
	CCarrot();
	virtual ~CCarrot();

public:
	enum STATE { INTRO_EARTH, INTRO, ATTACK_CARROT, ATTACK_BEAM, DEATH, END };

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
	void Attack_Carrot();
	void Attack_Beam();

	
private:
	bool	m_bIntro1;
	bool	m_bIntro2;

	STATE	m_ePreScene;
	STATE	m_eCurScene;

	DWORD	m_dwCarrotTime;
	DWORD	m_dwBeamTime;
	DWORD	m_dwCreateBeam;
	DWORD	m_dwTotalTime;

	bool	m_bPreBeam;
	int		m_iBeamCnt;

	int		m_iDeathCnt;
	bool	m_bDieMotion;
	bool	m_bBravo;

	CObj*	m_pBackSoil;
	DWORD	m_dwDeathEffect;

	bool	m_bDeathSound;
};


#endif