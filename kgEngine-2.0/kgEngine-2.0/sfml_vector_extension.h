#pragma once
#include "stdafx.h"

namespace std
{
	template<class T>
	sf::Vector2<T> floor( sf::Vector2<T> vec )
	{
		return sf::Vector2<T>( std::floor( vec.x ), std::floor( vec.y ) );
	}

	template<class T>
	sf::Vector2<T> ceil( sf::Vector2<T> vec )
	{
		return sf::Vector2<T>( std::ceil( vec.x ), std::ceil( vec.y ) );
	}

}
namespace kg
{
	template<class T>
	sf::Vector2<T> roundCoordinateVector( sf::Vector2<T>& vector )
	{
		return sf::Vector2<T>( std::floor( vector.x + .5f ), std::floor( vector.y + .5f ) );
	};
}
