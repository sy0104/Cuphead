#pragma once

#ifndef __STAGE2_H__
#define __STAGE2_H__

// Cagney Carnation
//// ²É º¸½º ////

#include "Scene.h"
class CStage2 : public CScene
{
public:
	CStage2();
	virtual ~CStage2();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	CObj*	m_pCagney;
	CObj*	m_pFinalCagney;
	CObj*	m_pPlayer;

	bool	m_bCreate;
	bool	m_bCollision;
};


#endif