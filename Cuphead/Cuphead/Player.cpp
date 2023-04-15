#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "PlayerJumpEffect.h"
#include "PlayerHitEffect.h"
#include "SoundMgr.h"


CPlayer::CPlayer()
	: m_fPosinDis(0.f), m_fJumpPower(0.f), m_fJumpTime(0.f), m_bJump(false)
	, m_ePreScene(STATE::END), m_eCurScene(STATE::END), m_dwTime(GetTickCount()), m_bIntro(false), m_bAttack(false)
	, m_ePlayerDir(DIR::DIR_END), m_bMove(false), m_eBullet(WEAPON::WEAPON_END), m_bAim(false), m_bHit(false)
	, m_bParryCheck(false), m_bJumpEffect(false), m_bHitEffect(false), m_bHitTime(false)
{
	ZeroMemory(&m_tPosin, sizeof(m_tPosin));

}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Idle.bmp", L"Player_Idle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Run.bmp", L"Player_Run");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Jump.bmp", L"Player_Jump");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Shoot.bmp", L"Player_Attack");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Intro.bmp", L"Player_Intro");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Straight_Run_Shooting.bmp", L"Player_Run_Attack");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Special_Attack.bmp", L"Player_Skill");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Diagonal_Up_Run_Shooting", L"Player_Run_Dia_Attack");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Aim.bmp", L"Player_Aim");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Dash.bmp", L"Player_Dash");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Hit.bmp", L"Player_Hit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Death.bmp", L"Player_Death");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cuphead/Ghost.bmp", L"Player_Ghost");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"..Image/Cuphead/Duck_Idle.bmp", L"Player_Down");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"..Image/Cuphead/Duck_Shoot.bmp", L"Player_Down_Shoot");

	
	m_tInfo.fX = 200.f;
	m_tInfo.fY = 550.f;

	m_tInfo.iCX = 100;
	m_tInfo.iCY = 150;

	m_tInfo.iMaxHp = 3;
	m_tInfo.iHp = 3;

	m_fSpeed = 8.f;

	m_fPosinDis = 50.f;

	m_fJumpPower = 20.f;
	m_fJumpTime = 0.f;
	m_bJump = false;

	m_ePreScene = INTRO;
	m_eCurScene = INTRO;

	m_ePlayerDir = RIGHT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 6;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
	m_pFrameKey = L"Player_Intro";

	m_eGroup = GROUPID::OBJECT;
}

int CPlayer::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	// 테스트용
	if (GetAsyncKeyState('Q'))
		m_tInfo.iHp = 1000;

	if (m_tInfo.iHp <= 0)
	{
		m_eCurScene = CPlayer::DEAD;
		m_pFrameKey = L"Player_Death";
	}

	// 이펙트 초기화
	if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
	{
		m_bJumpEffect = false;
		m_bHitEffect = false;
	}

	if (CKeyMgr::Get_Instance()->Get_Instance()->Key_Up(VK_SPACE))
		m_bParryCheck = false;

	// 인트로
	if (CPlayer::INTRO == m_eCurScene && !m_bIntro)
	{
		if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd) {

			++m_tFrame.iFrameScene;
			m_tFrame.iFrameStart = 0;
		}
	}


	// 조준중인지 체크
	if (m_bAim)
	{
		m_bMove = false;
		m_fSpeed = 0.f;
	}
	else
	{
		m_bMove = true;
		m_fSpeed = 8.f;
	}

	// 맞은 상태인지 체크
	if (m_bHit)
	{
		m_eCurScene = CPlayer::HIT;
		m_pFrameKey = L"Player_Hit";

		m_bHitTime = true;

		if (!m_bHitEffect)
		{
			CObj* pObj = CAbstractFactory<CPlayerHitEffect>::Create(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);

			m_bHitEffect = true;
		}
	}

	else if (!m_bHit)
		m_bHitTime = false;


	if (m_tRect.left <= 0)
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
			m_fSpeed = 0.f;
		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
			m_fSpeed = 8.f;
	}
	if (m_tRect.right >= WINCX)
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
			m_fSpeed = 0.f;
		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
			m_fSpeed = 8.f;
	}


	Key_Check();
	Jumping();
	Scene_Change();
	Frame_Move();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
	m_tPosin.x = (LONG)(m_tInfo.fX + cosf(m_fAngle * PI / 180.f) * m_fPosinDis);
	m_tPosin.y = (LONG)(m_tInfo.fY - sinf(m_fAngle * PI / 180.f) * m_fPosinDis);

	//if (m_tRect.left <= 0)
	//{
	//	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	//		m_fSpeed = 0.f;
	//	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	//		m_fSpeed = 8.f;
	//}
	//if (m_tRect.right >= WINCX)
	//{
	//	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	//		m_fSpeed = 0.f;
	//	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	//		m_fSpeed = 8.f;
	//}
	
}

void CPlayer::Render(HDC _DC)
{
	Update_Rect();


	// 인트로 애니메이션
	if (!m_bIntro)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Intro");

		GdiTransparentBlt(_DC
			, m_tRect.left, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 155, m_tFrame.iFrameScene * 175
			, 155, 175
			, RGB(40, 40, 40));

		if (m_tFrame.iFrameScene == 3 && m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
		{
			m_bIntro = true;
		}

		return;
	}


	// 왼쪽방향 출력
	if (m_ePlayerDir == LEFT)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
		HDC hStretch = CBmpMgr::Get_Instance()->Find_Image(L"Stretch");

		// 164 * 80
		if (m_eCurScene == IDLE_DOWN)
		{
			GdiTransparentBlt(_DC
				, m_tRect.left, m_tRect.top
				, m_tInfo.iCX, m_tInfo.iCY
				, hStretch
				, 0, 0
				, 164, 80
				, RGB(40, 40, 40));
		}

		else
		{
			StretchBlt(hStretch
				, 0, 0
				, 155, 175
				, hMemDC
				, m_tFrame.iFrameStart * 155 + 155, m_tFrame.iFrameScene * 175
				, -155, 175
				, SRCCOPY);

			GdiTransparentBlt(_DC
				, m_tRect.left, m_tRect.top
				, m_tInfo.iCX, m_tInfo.iCY
				, hStretch
				, 0, 0
				, 155, 175
				, RGB(40, 40, 40));
		}
		
	}
	
	// 오른쪽방향 출력
	else
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		if (m_eCurScene == IDLE_DOWN)
		{
			GdiTransparentBlt(_DC
				, m_tRect.left, m_tRect.top
				, m_tInfo.iCX, m_tInfo.iCY
				, hMemDC
				, m_tFrame.iFrameStart * 164, m_tFrame.iFrameScene * 80
				, 164, 80
				, RGB(40, 40, 40));
		}

		else
		{
			GdiTransparentBlt(_DC
				, m_tRect.left, m_tRect.top
				, m_tInfo.iCX, m_tInfo.iCY
				, hMemDC
				, m_tFrame.iFrameStart * 155, m_tFrame.iFrameScene * 175
				, 155, 175
				, RGB(40, 40, 40));
		}
	}
}

void CPlayer::Release()
{
}

void CPlayer::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		if (m_eCurScene == CPlayer::DEAD)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_eCurScene = CPlayer::GHOST;
				m_pFrameKey = L"Player_Ghost";
			}
		}

		else if (m_eCurScene == CPlayer::GHOST)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_bDead = true;
		}

		else if (m_eCurScene == CPlayer::IDLE_DOWN || m_eCurScene == CPlayer::ATTACK_DOWN)
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;
		}

		else
		{
			++m_tFrame.iFrameStart;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				if (m_bHit)
					m_bHit = false;
				if (m_eCurScene == JUMP)
				{
					m_bParryCheck = false;
					m_bJumpEffect = false;
				}
				else if (m_eCurScene == HIT)
					m_bHitEffect = false;

				m_eCurScene = CPlayer::IDLE;
				m_pFrameKey = L"Player_Idle";
				m_tFrame.iFrameStart = 0;
			}
		}

		m_tFrame.dwFrameTime = GetTickCount();
	}
}

void CPlayer::Key_Check()
{
	if (!m_bHit && m_bIntro)
	{
		// 에임 체크
		if (CKeyMgr::Get_Instance()->Key_Pressing('C'))
		{
			m_bAim = true;
			m_bMove = false;
			m_eCurScene = CPlayer::AIM;
			m_pFrameKey = L"Player_Aim";
		}
		else if (CKeyMgr::Get_Instance()->Key_Up('C'))
		{
			m_bAim = false;
			m_bMove = true;
		}


		///// 이동 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 왼쪽
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		{
			m_ePlayerDir = LEFT;
			m_tInfo.fX -= m_fSpeed;

			// 에임인지 체크
			if (CKeyMgr::Get_Instance()->Key_Pressing('C'))
			{
				m_bAim = true;
				m_bMove = false;
				m_eCurScene = CPlayer::AIM;
				m_pFrameKey = L"Player_Aim";
			}

			////// 대각선 /////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 왼쪽 대각선 위
			if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
			{
				// 에임 중이면
				if (CKeyMgr::Get_Instance()->Key_Pressing('C'))
				{
					m_bAim = true;
					m_bMove = false;
					m_eCurScene = CPlayer::AIM;
					m_pFrameKey = L"Player_Aim";
					m_tFrame.iFrameScene = 1;
				}
				else
				{
					m_eCurScene = RUN_DIA_ATTACK;
					m_pFrameKey = L"Player_Run_Dia_Attack";
				}


				if (m_dwTime + 200 < GetTickCount() && m_bAttack)
				{
					CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CBullet>(BULLET::LEFT_DIA_UP, 135.f));
					m_dwTime = GetTickCount();
				}

				// 왼쪽 대각선 아래
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
			{
				// 에임 중이면
				if (CKeyMgr::Get_Instance()->Key_Pressing('C'))
				{
					m_bAim = true;
					m_bMove = false;
					m_eCurScene = CPlayer::AIM;
					m_pFrameKey = L"Player_Aim";
					m_tFrame.iFrameScene = 0;
				}
				else
				{
					m_eCurScene = RUN_DIA_ATTACK;
					m_pFrameKey = L"Player_Run_Attack";
				}
				if (m_dwTime + 200 < GetTickCount() && m_bAttack)
				{
					CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CBullet>(BULLET::LEFT_DIA_DOWN, 225.f));
					m_dwTime = GetTickCount();
				}
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 점프
			if (m_bJump)
			{
				m_eCurScene = JUMP;
				m_pFrameKey = L"Player_Jump";
				// 대쉬는 이런식으로
				//m_tInfo.fX -= m_fSpeed;
			}

			// 공격 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if (m_bAttack)
			{
				if (!m_bAim)
				{
					m_eCurScene = RUN_ATTACK;
					m_pFrameKey = L"Player_Run_Attack";

				}
				if (m_dwTime + 200 < GetTickCount())
				{
					CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CBullet>(BULLET::LEFT, 180.f));
					m_dwTime = GetTickCount();
				}
			}

			// 왼쪽으로 걷기 /////////////////////////////////////////////////////////////////////////////////////////////////
			else
			{
				if (!m_bAim)
				{
					m_eCurScene = WALK_LEFT;
					m_pFrameKey = L"Player_Run";
				}
			}

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 오른쪽
		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		{
			m_tInfo.fX += m_fSpeed;
			m_ePlayerDir = RIGHT;

			// 에임인지 체크
			if (CKeyMgr::Get_Instance()->Key_Pressing('C'))
			{
				m_bAim = true;
				m_bMove = false;
				m_eCurScene = CPlayer::AIM;
				m_pFrameKey = L"Player_Aim";
			}

			/////// 대각선 /////////////////////////////////////////////////////////////////////////////////////////////////////
			// 오른쪽 대각선 위
			if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
			{
				if (CKeyMgr::Get_Instance()->Key_Pressing('C'))
				{
					m_bAim = true;
					m_bMove = false;
					m_eCurScene = CPlayer::AIM;
					m_pFrameKey = L"Player_Aim";
					m_tFrame.iFrameScene = 1;
				}
				else
				{
					m_eCurScene = RUN_DIA_ATTACK;
					m_pFrameKey = L"Player_Run_Dia_Attack";
				}

				if (m_dwTime + 200 < GetTickCount() && m_bAttack)
				{
					CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CBullet>(BULLET::RIGHT_DIA_UP, 45.f));
					m_dwTime = GetTickCount();
				}

				// 오른쪽 대각선 아래
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
			{
				if (CKeyMgr::Get_Instance()->Key_Pressing('C'))
				{
					m_bAim = true;
					m_bMove = false;
					m_eCurScene = CPlayer::AIM;
					m_pFrameKey = L"Player_Aim";
					m_tFrame.iFrameScene = 0;
				}
				else
				{
					m_eCurScene = RUN_DIA_ATTACK;
					m_pFrameKey = L"Player_Run_Attack";
				}

				if (m_dwTime + 200 < GetTickCount() && m_bAttack)
				{
					CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CBullet>(BULLET::RIGHT_DIA_DOWN, 315.f));
					m_dwTime = GetTickCount();
				}
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 점프
			if (m_bJump)
			{
				m_eCurScene = JUMP;
				m_pFrameKey = L"Player_Jump";
			}
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 공격
			else if (m_bAttack)
			{
				if (!m_bAim)
				{
					m_eCurScene = RUN_ATTACK;
					m_pFrameKey = L"Player_Run_Attack";
				}

				if (m_dwTime + 200 < GetTickCount())
				{
					CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CBullet>(BULLET::RIGHT, 0.f));
					m_dwTime = GetTickCount();
				}
			}
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 오른쪽으로 걷기
			else
			{
				if (!m_bAim)
				{
					m_eCurScene = WALK_RIGHT;
					m_pFrameKey = L"Player_Run";
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 위쪽 공격
		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		{
			m_ePlayerDir = UP;

			if (m_bAim)
			{
				m_tFrame.iFrameScene = 4;
			}

			if (m_bAttack)
			{
				m_eCurScene = ATTACK;
				m_pFrameKey = L"Player_Attack";

				m_tFrame.iFrameScene = 4;

				if (m_dwTime + 200 < GetTickCount())
				{
					CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CBullet>(BULLET::UP, 90.f));
					m_dwTime = GetTickCount();
				}

			}

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 아래쪽 공격
		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		{
			//m_ePlayerDir = DOWN;

			m_eCurScene = IDLE_DOWN;
			m_pFrameKey = L"Player_Down";

			//if (m_bAim)
			//{
			//	m_tFrame.iFrameScene = 2;
			//}

			//if (m_bAttack)
			//{
			//	m_eCurScene = ATTACK_DOWN;
			//	m_pFrameKey = L"Player_Down_Shoot";

			//	if (m_dwTime + 200 < GetTickCount())
			//	{
			//		CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CBullet>(BULLET::DOWN, 270.f));
			//		m_dwTime = GetTickCount();
			//	}

			//}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// Normal_Bulelt 스킬
		//else if (CKeyMgr::Get_Instance()->Key_Pressing('V'))
		//{
		//	m_eCurScene = SKILL;
		//	m_pFrameKey = L"Player_Skill";

		//	if (m_dwTime + 500 < GetTickCount())
		//	{
		//		if (m_ePlayerDir == RIGHT)
		//			CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_SpecialBullet<CSpecialBullet>(BULLET::RIGHT));
		//		else if (m_ePlayerDir == LEFT)
		//			CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_SpecialBullet<CSpecialBullet>(BULLET::LEFT));

		//		m_dwTime = GetTickCount();
		//	}
		//}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 가만히
		else
		{
			m_bMove = false;

			if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
			{
				m_bJump = true;
			}
			if (m_bJump)
			{
				m_eCurScene = JUMP;
				m_pFrameKey = L"Player_Jump";
			}

			else if (m_bAttack)
			{
				m_eCurScene = ATTACK;
				m_pFrameKey = L"Player_Attack";

				if (m_dwTime + 200 < GetTickCount())
				{
					if (LEFT == m_ePlayerDir)
						CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CBullet>(BULLET::LEFT, 180.f));
					if (RIGHT == m_ePlayerDir)
						CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CBullet>(BULLET::RIGHT, 0.f));
					m_dwTime = GetTickCount();
				}
			}


			// Idle로
			if (CKeyMgr::Get_Instance()->Key_Up(VK_LEFT) || CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT)
				|| CKeyMgr::Get_Instance()->Key_Up(VK_DOWN))
			{
				m_eCurScene = CPlayer::IDLE;
				m_pFrameKey = L"Player_Idle";
			}

			//else
			//{
			//	m_pFrameKey = L"Player_Idle";
			//	m_eCurScene = IDLE;
			//}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
		{
			m_bJump = true;
		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('X'))
		{

			m_bAttack = true;
		}
		else if (!CKeyMgr::Get_Instance()->Key_Pressing('X'))
		{
			m_bAttack = false;
		}
	}

}

void CPlayer::Jumping()
{
	if (m_bHit)
		m_bJump = false;

	if (m_bJump)
	{
		m_fJumpSpeed = m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f;
		if (-15.f > m_fJumpSpeed)
			m_fJumpSpeed = -15.f;

		m_tInfo.fY -= m_fJumpSpeed;
		m_fJumpTime += 0.2f;

		if (m_tInfo.fY > 550.f + (m_tInfo.iCY >> 1))
		{
			m_bJump = false;
			m_tInfo.fY = 550.f;
			m_fJumpTime = 0.f;

			if (!m_bJumpEffect)
			{
				CObj* pObj = CAbstractFactory<CPlayerJumpEffect>::Create(m_tInfo.fX, m_tInfo.fY + 75.f);
				CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pObj);
				m_bJumpEffect = true;
			}
		}
	}

	else
	{
		m_tInfo.fY += 10.f;
		if (m_tInfo.fY > 550.f)
			m_tInfo.fY = 550.f;
	}
}

void CPlayer::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case CPlayer::INTRO:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;		// 27
			m_tFrame.iFrameScene = 0;	// 0 ~ 3
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 150;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::WALK_LEFT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::WALK_RIGHT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::IDLE_DOWN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Player_Down";
			break;
		case CPlayer::JUMP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 30;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::DASH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::ATTACK_DOWN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::SKILL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::RUN_ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::RUN_DIA_ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::AIM:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_pFrameKey = L"Player_Hit";
			break;
		case CPlayer::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::GHOST:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 23;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tInfo.fY = 200.f;
			break;
		}
		m_ePreScene = m_eCurScene;
	}
}

void CPlayer::Set_Hit()
{
	if (!m_bHitTime)
		m_bHit = true;

	m_bHitTime = true;
}