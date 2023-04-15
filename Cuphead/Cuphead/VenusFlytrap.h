#pragma once

#ifndef __VENUSFLYTRAP_H__
#define __VENUSFLYTRAP_H__

#include "Obj.h"
class CVenusFlytrap : public CObj
{
public:
	CVenusFlytrap();
	virtual ~CVenusFlytrap();

public:
	enum STATE { IDLE, DEAD, END };
	
	enum DIR { LEFT, RIGHT, DIR_END };

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	virtual void Frame_Move() override;

private:
	void Scene_Change();


private:
	STATE		m_ePreScene;
	STATE		m_eCurScene;

	DIR			m_eDir;

	int			m_iSizeX;
	int			m_iSizeY;
};

#endif