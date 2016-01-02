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

	bool Position::operator!=( const Position& other ) const
	{
		return !operator==( other );
	}

	kg::Position2d Position::toPosition2d() const
	{
		return Position2d( x, y, worldLayer );
	}

	kg::PositionXY Position::toPositionXY() const
	{
		return PositionXY( x, y );
	}

	kg::PositionXYZ Position::toPositionXYZ() const
	{
		return PositionXYZ( x, y, zValue );
	}

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

	bool Position2d::operator!=( const Position2d& other ) const
	{
		return !operator==( other );
	}

	kg::PositionXY Position2d::toPositionXY() const
	{
		return PositionXY( x, y );
	}

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

	PositionXYZ::PositionXYZ( int x, int y, int z )
		: x( x ),
		y( y ),
		z( z )
	{ }

	bool PositionXYZ::operator!=( const PositionXYZ& other ) const
	{
		return !operator==( other );
	}

	kg::PositionXY PositionXYZ::toPositionXY() const
	{
		return PositionXY( x, y );
	}

	bool PositionXYZ::operator==( const PositionXYZ& other ) const
	{
		if( this->x == other.x )
			return true;
		else if( this->y == other.y )
			return true;
		else if( this->zValue == other.zValue )
			return true;
		else
			return false;
	}

	PositionXY::PositionXY( int x, int y )
		:x( x ),
		y( y )
	{ }

	bool PositionXY::operator!=( const PositionXY& other ) const
	{
		return !operator==( other );
	}

	sf::Vector2i PositionXY::toVector2i() const
	{
		return sf::Vector2i( x, y );
	}

	bool PositionXY::operator==( const PositionXY& other ) const
	{
		if( this->x == other.x )
			return true;
		else if( this->y == other.y )
			return true;
		else
			return false;
	}
}