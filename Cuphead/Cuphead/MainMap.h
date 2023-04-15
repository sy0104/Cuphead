#pragma once

#ifndef __MAINMAH_H__
#define __MAINMAH_H__

#include "Scene.h"
class CMainMap : public CScene
{
public:
	CMainMap();
	virtual ~CMainMap();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};


#endif