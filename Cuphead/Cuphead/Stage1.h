#pragma once

#ifndef __STAGE1_H__
#define __STAGE1_H__

//// ≥Û¿€π∞ ∫∏Ω∫ ////

#include "Scene.h"
class CStage1 : public CScene
{
public:
	CStage1();
	virtual ~CStage1();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	bool	m_bWave1;	// Potato
	bool	m_bWave2;	// Carrot

	bool	m_bCreate;

	CObj*	m_pPlayer;
	CObj*	m_pPotato;
	CObj*	m_pCarrot;

	bool	m_bCollision;

};


#endif