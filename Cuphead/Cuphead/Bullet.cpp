#include "stdafx.h"
#include "Bullet.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "BulletEffect.h"
#include "SoundMgr.h"


CBullet::CBullet()
	: m_bCheck(false), m_bBulletEffect(false)
{
}


CBullet::~CBullet()
{
	Release();
} 

void CBullet::Initialize()
{
	// 40 * 45
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/Lobber.bmp", L"Normal_Bullet");
	// 65 * 65
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bullet/Normal_Bullet_Special.bmp", L"Normal_Bullet_Special");

	CSoundMgr::Get_Instance()->PlaySound(L"BulletShoot.ogg", CSoundMgr::EFFECT);


	m_tInfo.iCX = 20;
	m_tInfo.iCY = 20;

	m_eGroup = GROUPID::OBJECT;

	m_tInfo.iAtt = 5;

	m_fSpeed = 10.f;
}

int CBullet::Update()
{


	if (m_bDead)
	{
		if (!m_bBulletEffect)
		{
			CObj* pObj = CAbstractFactory<CBulletEffect>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);

			m_bBulletEffect = true;
		}
		return OBJ_DEAD;
	}

	switch (m_eDir)
	{
	//////// 상하좌우
	case BULLET::LEFT:		// 180
		m_tInfo.fX -= m_fSpeed;
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Move())
			m_tInfo.fX -= 8.f;
		break;
	case BULLET::RIGHT:		// 0
		m_tInfo.fX += m_fSpeed;
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Move())
			m_tInfo.fX += 8.f;
		break;
	case BULLET::UP:		// 90
		m_tInfo.fY -= m_fSpeed;
		break;	
	case BULLET::DOWN:		// 270
		m_tInfo.fY += m_fSpeed;
		break;

	///////// 대각선
	case BULLET::LEFT_DIA_UP:		// 135
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Aim() && !m_bCheck)
		{
			m_tInfo.fX -= 10.f;
			m_tInfo.fY -= 45.f;
			m_bCheck = true;
		}
		m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
		m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Move())
		{
			m_tInfo.fX -= 8.f / sqrtf(2.f);
			m_tInfo.fY -= 8.f / sqrtf(2.f);
		}
		break;
	case BULLET::LEFT_DIA_DOWN:		// 225
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Aim() && !m_bCheck)
		{
			m_tInfo.fX -= 10.f;
			m_tInfo.fY += 45.f;
			m_bCheck = true;
		}
		m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
		m_tInfo.fY += m_fSpeed / sqrtf(2.f);
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Move())
		{
			m_tInfo.fX -= 8.f / sqrtf(2.f);
			m_tInfo.fY += 8.f / sqrtf(2.f);
		}
		break;
	case BULLET::RIGHT_DIA_UP:		// 45
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Aim() && !m_bCheck)
		{
			m_tInfo.fX += 10.f;
			m_tInfo.fY -= 45.f;
			m_bCheck = true;
		}
		m_tInfo.fX += m_fSpeed / sqrtf(2.f);
		m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Move())
		{
			m_tInfo.fX += 8.f / sqrtf(2.f);
			m_tInfo.fY -= 8.f / sqrtf(2.f);
		}
		break;
	case BULLET::RIGHT_DIA_DOWN:	// 315
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Aim() && !m_bCheck)
		{
			m_tInfo.fX += 10.f;
			m_tInfo.fY += 45.f;
			m_bCheck = true;
		}
		m_tInfo.fX += m_fSpeed / sqrtf(2.f);
		m_tInfo.fY += m_fSpeed / sqrtf(2.f);
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Move())
		{
			m_tInfo.fX += 8.f / sqrtf(2.f);
			m_tInfo.fY += 8.f / sqrtf(2.f);
		}
		break;
	}

	Update_Rect();

	return OBJ_NOEVENT;
}

void CBullet::Late_Update()
{
	if (m_tRect.left <= 0 || m_tRect.right >= WINCX
		|| m_tRect.top <= 0 || m_tRect.bottom >= WINCY)
		m_bDead = true;
}

void CBullet::Render(HDC _DC)
{
	Update_Rect();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Normal_Bullet");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0, 0
		, 40, 45
		, RGB(40, 40, 40));
}

void CBullet::Release()
{
}
