#pragma once
#include "stdafx.h"

namespace std
{
	template<class T>
	sf::Vector2<T> floor( sf::Vector2<T> vec )
	{
		return sf::Vector2<T>( floor( vec.x ), floor( vec.y ) );
	}
}
