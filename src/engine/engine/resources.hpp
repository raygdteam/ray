#pragma once
#include "pch.h"

namespace ray::resources
{

enum class_id
{
	unknown,
	raw_data,
	shader,
	texture,
	model,
};



template<typename T>
struct resource_cook_base
{
	virtual bool Cook(T) = 0;
};

}