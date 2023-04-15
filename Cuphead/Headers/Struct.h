#pragma once

#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct tagInfo
{
	float	fX;
	float	fY;
	int		iCX;
	int		iCY;

	int		iAtt;
	int		iHp;
	int		iMaxHp;
}INFO;

typedef struct tagLinePos
{
	tagLinePos() {}
	tagLinePos(float _x, float _y) : fX(_x), fY(_y) {}

	float fX;
	float fY;
}LINEPOS;

typedef struct tagLineInfo
{
	tagLineInfo() {}
	tagLineInfo(LINEPOS _tLeft, LINEPOS _tRight)
		: tLeftPos(_tLeft), tRightPos(_tRight)
	{

	}

	LINEPOS		tLeftPos;
	LINEPOS		tRightPos;

}LINEINFO;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iFrameScene;
	DWORD	dwFrameSpeed;
	DWORD	dwFrameTime;
	float	fSizeX;
	float	fSizeY;
	int		iIndexY;

}FRAME;


#endif