#pragma once
#include <type_traits>

enum class InputID
{
	Up,
	Down,
	Left,
	Right,
	Max
};

static InputID begin(InputID)				{return InputID::Up;}
static InputID end(InputID)				{return InputID::Max;}
static InputID operator*(InputID key)		{return key;}
static InputID operator++(InputID& key)	{return key= InputID(std::underlying_type<InputID>::type(key) + 1);}