#include "stdafx.h"
#include "ScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr()
	: m_fScrollX(0.f), m_fScrollY(0.f)
{
}


CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Initialize()
{
	m_fScrollX = 0.f;
	m_fScrollY = 0.f;
}

void CScrollMgr::Scroll_Lock()
{
	if (m_fScrollX > 0)
		m_fScrollX = 0.f;

	if (m_fScrollX < -(TILEX * TILECX - WINCX))
		m_fScrollX = -(TILEX * TILECX - WINCX);

	if (m_fScrollY > 0)
		m_fScrollY = 0.f;

	if (m_fScrollY < -(TILEY * TILECY - WINCY))
		m_fScrollY = -(TILEY * TILECY - WINCY);
}
