#pragma once

#ifndef __STAGE3_H__
#define __STAGE3_H__

//// 다이스 보스 ////

#include "Scene.h"
class CStage3 : public CScene
{
public:
	CStage3();
	virtual ~CStage3();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	CObj*	m_pDice;
};


#endif