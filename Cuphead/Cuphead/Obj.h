#pragma once

#ifndef __OBJ_H__
#define __OBJ_H__

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize() = 0;
	virtual int Update() = 0;
	virtual void Late_Update() = 0;
	virtual void Render(HDC _DC) = 0;
	virtual void Release() = 0;

public:
	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x; m_tInfo.fY = _y; }
	void Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void Set_Dead() { m_bDead = true; }
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }
	void Set_PosX(float _x) { m_tInfo.fX = _x; }
	void Set_PosY(float _y) { m_tInfo.fY = _y; }
	void Set_FrameKey(TCHAR* _pFrameKey) { m_pFrameKey = _pFrameKey; }
	void Set_Dir(BULLET::DIR _eDir) { m_eDir = _eDir; }
	void Set_Speed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void Set_Hp(int _iAtt) { m_tInfo.iHp -= _iAtt; }
	void Set_Parry() { m_bParry = true; }

public:
	const RECT& Get_Rect() const { return m_tRect; }
	const RECT& Get_CollisionRect() const { return m_tCollisionRect; }
	const INFO& Get_Info() const { return m_tInfo; }
	const FRAME& Get_Frame() const { return m_tFrame; }
	const BULLET::DIR Get_Dir() const { return m_eDir; }
	const int Get_Att() const { return m_tInfo.iAtt; }
	const int Get_Hp() const { return m_tInfo.iHp; }
	const int Get_Dead() const { return m_bDead; }
	const bool Get_Parry() const { return m_bParry; }
	const GROUPID::ID Get_GroupID() const { return m_eGroup; }
	

protected:
	void Update_Rect();
	virtual void Frame_Move() {};

protected:
	INFO	m_tInfo;
	RECT	m_tRect;
	FRAME	m_tFrame;
	RECT	m_tCollisionRect;

	float	m_fSpeed;
	float	m_fAngle;

	bool	m_bDead;
	bool	m_bParry;

	CObj*	m_pTarget;

	TCHAR*	m_pFrameKey;

	BULLET::DIR		m_eDir;

	GROUPID::ID		m_eGroup;
};


#endif