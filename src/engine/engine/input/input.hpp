#pragma once
#include "pch.h"

#define KEY_W 0x57
#define KEY_A 0x41
#define KEY_S 0x53
#define KEY_D 0x44

namespace ray
{

class _Input
{
public:
	bool keys[256];
};

// TODO: плохо!
extern _Input Input;

}