#pragma once
#include "stdafx.h"

namespace std
{
	template<class T>
	sf::Vector2<T> floor( sf::Vector2<T> vec )
	{
		return sf::Vector2<T>( floor( vec.x ), floor( vec.y ) );
	}

	template<class T>
	sf::Vector2<T> ceil( sf::Vector2<T> vec )
	{
		return sf::Vector2<T>( ceil( vec.x ), ceil( vec.y ) );
	}

}
namespace kg
{
	template<class T>
	sf::Vector2<T> roundCoordinateVector( sf::Vector2<T>& vector )
	{
		return sf::Vector2<T>( std::floor( vector.x + .5f ), std::floor( vector.y + .5f ) );
	};

	template<class T>
	auto length( sf::Vector2<T> vec ) -> decltype(sqrt( (pow( vec.x, 2 ) + pow( vec.y, 2 )) ))
	{
		return sqrt( (pow( vec.x, 2 ) + pow( vec.y, 2 )) );
	}
}
