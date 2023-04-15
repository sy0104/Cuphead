#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "ObjMgr.h"
#include "Player.h"
#include "Cagney.h"
#include "KeyMgr.h"
#include "BulletEffect.h"

bool CCollisionMgr::m_bCheck = false;


CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &(Dst->Get_Rect()), &(Src->Get_Rect())))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

void CCollisionMgr::Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			float fX = 0.f, fY = 0.f;
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				if (fX < fY)		// 좌우
				{
					if (Dst->Get_Info().fX > Src->Get_Info().fX)
						Src->Set_PosX(-fX);
					else
						Src->Set_PosX(fX);
				}
				else		// 상하
				{
					if (Dst->Get_Info().fY > Src->Get_Info().fY)
						Src->Set_PosY(-fY);
					else
						Src->Set_PosY(fY);
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dst, Src))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

void CCollisionMgr::Collision_Platform2(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	bool bCheck = true;
	// Dst: Platform
	for (auto& Dst : _Dst)
	{
		if (!bCheck)
			continue;

		// Src: Player
		for (auto& Src : _Src)
		{
			if (Dst->Get_Rect().left < Src->Get_Info().fX
				&& Dst->Get_Rect().right > Src->Get_Info().fX)
			{
				bCheck = false;
				if (dynamic_cast<CPlayer*>(Src)->Get_Jump()
					&& dynamic_cast<CPlayer*>(Src)->Get_JumpSpeed() < 0)
				{
					if (Src->Get_Rect().bottom > Dst->Get_Rect().top
						&& Src->Get_Rect().bottom < Dst->Get_Rect().bottom)
					{
						Src->Set_PosY(Dst->Get_Rect().top - 75.f);
						dynamic_cast<CPlayer*>(Src)->Set_Jump2();
					}
				}
				else if (!dynamic_cast<CPlayer*>(Src)->Get_Jump()
					&& Src->Get_Rect().bottom > Dst->Get_Rect().top
					&& Src->Get_Rect().bottom < Dst->Get_Rect().bottom)
				{
					Src->Set_PosY(Dst->Get_Rect().top - 75.f);
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Bullet_To_Cagney(list<CObj*>& _Dst, CObj* _Src)
{
	RECT rc = {};

	// Dst: Bullet
	for (auto& Dst : _Dst)
	{
		// Src: Boss(Cagney)
		//for (auto& Src : _Src)
		//{
			//if (Dst->Get_Rect().right >= Src->Get_CollisionRect().left)
			//{
			//	Src->Set_Hp(Dst->Get_Att());
			//	Dst->Set_Dead();
			//}

			if (Dst->Get_Rect().right >= _Src->Get_CollisionRect().left)
			{
				_Src->Set_Hp(Dst->Get_Att());
				Dst->Set_Dead();
			}
		//}
	}
}

void CCollisionMgr::Collision_Bullet_To_Cagney2(list<CObj*>& _Dst, CObj* _Src)
{
	RECT rc = {};

	// Dst: Bullet
	for (auto& Dst : _Dst)
	{
		// Src: Boss(Cagney)
		if (Dst->Get_Rect().right >= 1000.f)
		{
			_Src->Set_Hp(Dst->Get_Att());
			Dst->Set_Dead();
		}	
	}
}

void CCollisionMgr::Collision_Bullet_To_Monster(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	// Dst: Monster
	for (auto& Dst : _Dst)
	{
		// Src: Bullet
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &(Dst->Get_Rect()), &(Src->Get_Rect())))
			{
				Dst->Set_Hp(Src->Get_Att());
				Src->Set_Dead();
			}
		}
	}
}

void CCollisionMgr::Collision_Bullet_To_Potato(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	// Dst: Potato & Src: Bullet
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_CollisionRect(), &Src->Get_Rect()))
			{
				if (Dst->Get_Hp() > 0)
				{
					Dst->Set_Hp(Src->Get_Att());
					Src->Set_Dead();
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Bullet_To_Carrot(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	// Dst: Boss
	for (auto& Dst : _Dst)
	{
		// Src: Bullet
		for (auto& Src : _Src)
		{
			if (Src->Get_Rect().right >= 600.f)
			{
				if (Dst->Get_Hp() > 0)
				{
					Dst->Set_Hp(Src->Get_Att());
					Src->Set_Dead();
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Player_To_Monster(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	// Dst: Monster
	for (auto& Dst : _Dst)
	{
		// Src: Player
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				// 패리 가능한 몬스터도 충돌했는데 패리를 하지 않았을 경우 충돌체크 해야댐
				// 패리 가능한 몬스터에 닿았고 / 플레이어가 점프중인데 / 또 점프를 하면 -> 패리

				// 패리 O 몬스터
				if (Dst->Get_Parry())
				{
					if (dynamic_cast<CPlayer*>(Src)->Get_Jump() && !dynamic_cast<CPlayer*>(Src)->Get_ParryCheck())
					{
						if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
						{
							dynamic_cast<CPlayer*>(Src)->Set_JumpTime();	// 점프타임 0으로
							dynamic_cast<CPlayer*>(Src)->Set_ParryCheck();
						}
					}

				}

				// 패리 X 몬스터
				else
				{
					if (Src->Get_Hp() > 0)
					{
						Src->Set_Hp(1);	// 플레이어 hp 1만큼 감소
						dynamic_cast<CPlayer*>(Src)->Set_Hit();	// Hit 모션 수행
						Dst->Set_Dead();
					}
				}
			}
			
		}
	}
}

void CCollisionMgr::Collision_Player_To_MonsterBack(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	// Dst: Monster
	for (auto& Dst : _Dst)
	{
		// Src: Player
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				if (Src->Get_Hp() > 0)
				{
					Src->Set_Hp(1);	// 플레이어 hp 1만큼 감소
					dynamic_cast<CPlayer*>(Src)->Set_Hit();	// Hit 모션 수행
					//Dst->Set_Dead();
				
				}
			}

		}
	}

}


void CCollisionMgr::Collision_Player_To_Card(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	// Dst: Monster
	for (auto& Dst : _Dst)
	{
		// Src: Player
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_CollisionRect(), &Src->Get_Rect()))
			{
				// 패리 가능한 몬스터도 충돌했는데 패리를 하지 않았을 경우 충돌체크 해야댐
				// 패리 가능한 몬스터에 닿았고 / 플레이어가 점프중인데 / 또 점프를 하면 -> 패리

				// 패리 O 몬스터
				if (Dst->Get_Parry())
				{
					if (dynamic_cast<CPlayer*>(Src)->Get_Jump() && !dynamic_cast<CPlayer*>(Src)->Get_ParryCheck())
					{
						if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
						{
							dynamic_cast<CPlayer*>(Src)->Set_JumpTime();	// 점프타임 0으로
							dynamic_cast<CPlayer*>(Src)->Set_ParryCheck();
						}
					}

				}

				// 패리 X 몬스터
				else
				{
					if (Src->Get_Hp() > 0)
					{
						Src->Set_Hp(1);	// 플레이어 hp 1만큼 감소
						dynamic_cast<CPlayer*>(Src)->Set_Hit();	// Hit 모션 수행
					}
				}
			}

		}
	}

}

void CCollisionMgr::Collision_Player_To_Boss(CObj* _pObj1, CObj * _pObj2)
{
	RECT rc = {};

	// pObj1: Player, pObj2 = Boss
	if (IntersectRect(&rc, &_pObj1->Get_Rect(), &_pObj2->Get_Rect()))
	{
		_pObj1->Set_Hp(1);
		dynamic_cast<CPlayer*>(_pObj1)->Set_Hit();
	}
}


bool CCollisionMgr::Check_Sphere(CObj* _Dst, CObj* _Src)
{
	float fX = _Dst->Get_Info().fX - _Src->Get_Info().fX;
	float fY = _Dst->Get_Info().fY - _Src->Get_Info().fY;
	float fDia = sqrtf(fX * fX + fY * fY);

	float fDis = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCX) >> 1);

	if (fDia <= fDis)
		return true;

	return false;
}

bool CCollisionMgr::Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y)
{
	float fX = _Dst->Get_Info().fX - _Src->Get_Info().fX;
	float fY = _Dst->Get_Info().fY - _Src->Get_Info().fY;

	float fXDis = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fYDis = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	if (fX > fXDis && fY > fYDis)
	{
		*_x = fX - fXDis;
		*_y = fY - fYDis; 
		return true;
	}

	return false;
}
