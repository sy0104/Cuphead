#pragma once

#ifndef __MAINGAME_H__
#define __MAINGAME_H__

class CObj;

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

private:
	HDC		m_DC;
	
};


#endif