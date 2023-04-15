#pragma once

#ifndef __TESTSTAGE_H__
#define __TESTSTAGE_H__

#include "Scene.h"
class CTestStage : public CScene
{
public:
	CTestStage();
	virtual ~CTestStage();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};


#endif