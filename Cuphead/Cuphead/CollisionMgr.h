#pragma once

#ifndef __COLLISIONMGR_H__
#define __COLLISIONMGR_H__

class CObj;

class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_Platform2(list<CObj*>& _Dst, list<CObj*>& _Src);		// Platform & Player
	static void Collision_Player_To_Monster(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_Player_To_MonsterBack(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_Player_To_Card(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_Player_To_Boss(CObj* _pObj1, CObj * _pObj2);

	// Bullet
	static void Collision_Bullet_To_Cagney(list<CObj*>& _Dst, CObj* _Src);	// Bullet & Cagney
	static void Collision_Bullet_To_Cagney2(list<CObj*>& _Dst, CObj* _Src);	// Bullet & Cagney
	static void Collision_Bullet_To_Monster(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_Bullet_To_Potato(list<CObj*>& _Dst, list<CObj*>& _Src);	// Potato & Bullet
	static void Collision_Bullet_To_Carrot(list<CObj*>& _Dst, list<CObj*>& _Src);	// Carrot & Bullet


	static bool Check_Sphere(CObj* _Dst, CObj* _Src);
	static bool Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y);

private:
	static bool	m_bCheck;
};


#endif