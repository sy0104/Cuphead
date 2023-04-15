#pragma once

#ifndef __CAGNEY_H__
#define __CAGNEY_H__

#include "Obj.h"
class CCagney : public CObj
{
public:
	CCagney();
	virtual ~CCagney();

public:
	enum STATE { INTRO, IDLE
	/*공격패턴*/	, FACE_ATTACK_LOW, FACE_ATTACK_HIGH, FIRING_SEEDS, CREATE_OBJECT
				, END };

	enum OBJECT { BOOMERANG, ACORN, OBJECT_END };

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

private:
	void Face_Attack_Low();
	void Face_Attack_High();
	void Firing_Seeds();
	void Create_Object();

private:
	void Scene_Change();
	void Update_CollisionRect();

private:
	void Set_Idle(int _iFirst, int _iEnd);

private:
	STATE	m_ePreScene;
	STATE	m_eCurScene;

	DWORD	m_dwCreateTime;

	bool	m_bIntro;
	bool	m_bFace_Low;
	bool	m_bFace_High;
	bool	m_bFiring_Seeds;

	bool	m_bCreating_Objects;
	bool	m_bCreateEnd;
	OBJECT	m_eObject;

	bool	m_bCreate;

	bool	m_bCreateSeeds;
	int		m_iSeedCnt;

	bool	m_bCreateEffect;
	bool	m_bAcornEffect;
};


#endif