#pragma once

#ifndef __POTATO_H__
#define __POTATO_H__

#include "Obj.h"
class CPotato : public CObj
{
public:
	CPotato();
	virtual ~CPotato();

public:
	enum STATE { INTRO_EARTH, INTRO, IDLE, ATTACK, DEATH, END };

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
	void Attack();
	void Set_Idle(int _iFirst, int _iEnd);


private:
	STATE	m_ePreScene;
	STATE	m_eCurScene;

	bool	m_bIntro1;
	bool	m_bIntro2;

	bool	m_bCreate;
	int		m_iCreateCnt;

	int		m_iDeathCnt;
	bool	m_bDieMotion;

	CObj*	m_pBackSoil;

	bool	m_bCreateEffect;
	DWORD	m_dwDeathEffect;
};


#endif