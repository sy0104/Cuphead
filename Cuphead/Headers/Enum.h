#pragma once

#ifndef __ENUM_H__
#define __ENUM_H__

namespace OBJID
{
	enum ID { PLAYER, WORLD_PLAYER, BOSS, MONSTER, MONSTER_BACK, BULLET, DICE, CARD, FLAG, EFFECT, BACK, PLATFORM, END };
}

namespace BULLET
{
	enum DIR { LEFT, RIGHT, UP, DOWN
			, LEFT_DIA_UP, LEFT_DIA_DOWN, RIGHT_DIA_UP, RIGHT_DIA_DOWN, END };
};

namespace MONSTER
{
	enum MON_DIR { LEFT, RIGHT, END };
}

namespace GROUPID
{
	enum ID { BACKGROUND, OBJECT, EFFECT, UI, END };
}

#endif