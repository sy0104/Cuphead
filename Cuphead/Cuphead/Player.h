#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Obj.h"
class CPlayer : public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	enum STATE { IDLE, INTRO, WALK_RIGHT, WALK_LEFT, IDLE_DOWN
		, JUMP, DASH, ATTACK, ATTACK_DOWN, SKILL, RUN_ATTACK, RUN_DIA_ATTACK
		, AIM, HIT, DEAD, GHOST, END };

	enum DIR { LEFT, RIGHT, UP, DOWN, DIR_END };

	enum WEAPON { NORMAL, SPREAD, WEAPON_END };

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

private:
	void Key_Check();
	void Jumping();
	void Scene_Change();

public:
	bool Get_Move() { return m_bMove; }
	float Get_Speed() { return m_fSpeed; }
	float Get_JumpSpeed() { return m_fJumpSpeed; }
	bool Get_Jump() { return m_bJump; }
	bool Get_Aim() { return m_bAim; }
	STATE Get_State() { return m_eCurScene; }
	bool Get_ParryCheck() { return m_bParryCheck; }
	bool Get_HitEffect() { return m_bHitEffect; }

public:
	void Set_Jump(bool _bJump) { m_bJump = _bJump; }
	void Set_Jump2() { m_bJump = false; m_fJumpTime = 0.f; }
	void Set_FreeJump() { m_tInfo.fY += 7.f; }
	void Set_JumpTime() { m_fJumpTime = 0.f; }		// 패리할때 점프타임 0으로
	void Set_Hit();
	void Set_ParryCheck() { m_bParryCheck = true; }

private:
	// Normal_Bullet
	template <typename T>
	CObj* Create_Bullet(BULLET::DIR _eDir, float _fAngle)
	{
		CObj* pObj = nullptr;

		if(LEFT == m_ePlayerDir)
			pObj = CAbstractFactory<T>::Create((float)(m_tPosin.x - 95), (float)m_tPosin.y, _fAngle);
		// 여기 안돼
		else if (DOWN == m_ePlayerDir)
			pObj = CAbstractFactory<T>::Create(m_tInfo.fX, m_tInfo.fY + 100);
		else if (UP == m_ePlayerDir)
			pObj = CAbstractFactory<T>::Create(m_tInfo.fX, m_tInfo.fY - 100);
		//////
		else
			pObj = CAbstractFactory<T>::Create((float)m_tPosin.x, (float)m_tPosin.y);

		pObj->Set_Dir(_eDir);
		pObj->Set_Angle(_fAngle);

		return pObj;
	}


private:
	POINT	m_tPosin;
	float	m_fPosinDis;

	float	m_fJumpPower;
	float	m_fJumpTime;
	bool	m_bJump;
	float	m_fJumpSpeed;

	STATE	m_ePreScene;
	STATE	m_eCurScene;
	DIR		m_ePlayerDir;
	WEAPON	m_eBullet;

	DWORD	m_dwTime;

	bool	m_bIntro;
	bool	m_bAttack;
	bool	m_bMove;
	bool	m_bAim;
	bool	m_bHit;
	bool	m_bHitTime;
	bool	m_bParryCheck;
	bool	m_bJumpEffect;
	bool	m_bHitEffect;

};


#endif