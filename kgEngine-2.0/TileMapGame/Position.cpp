#include "Position.h"
using namespace std;

namespace kg
{
	Position::Position( int x, int y, int zValue, int worldLayer )
		:x( x ),
		y( y ),
		zValue( zValue ),
		worldLayer( worldLayer )
	{ }

	bool Position::operator==( const Position& other ) const
	{
		if( this->x == other.x )
			return true;
		else if( this->y == other.y )
			return true;
		else if( this->zValue == other.zValue )
			return true;
		else if( this->worldLayer == other.worldLayer )
			return true;
		else
			return false;
	}

	Position2d::Position2d( int x, int y, int worldLayer )
		:x( x ),
		y( y ),
		worldLayer( worldLayer )
	{ }

	bool Position2d::operator==( const Position2d& other ) const
	{
		if( this->x == other.x )
			return true;
		else if( this->y == other.y )
			return true;
		else if( this->worldLayer == other.worldLayer )
			return true;
		else
			return false;
	}
}