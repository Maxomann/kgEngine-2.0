#pragma once
#include "stdafx.h"

namespace kg
{
	struct PositionXY
	{
		int x = 0;
		int y = 0;

		PositionXY( int x, int y );
		bool operator==( const PositionXY& other )const;
		bool operator!=( const PositionXY& other )const;

		sf::Vector2i toVector2i()const;
	};

	struct Position2d
	{
		int x = 0;
		int y = 0;
		int worldLayer = 0;

		Position2d( int x, int y, int worldLayer );
		bool operator==( const Position2d& other )const;
		bool operator!=( const Position2d& other )const;

		PositionXY toPositionXY()const;
	};

	typedef Position2d ChunkPosition;

	struct PositionXYZ
	{
		int x = 0;
		int y = 0;
		int zValue = 0;

		PositionXYZ( int x, int y, int z );
		bool operator==( const PositionXYZ& other )const;
		bool operator!=( const PositionXYZ& other )const;

		PositionXY toPositionXY()const;
	};

	struct Position
	{
		int x = 0;
		int y = 0;
		int zValue = 0;
		int worldLayer = 0;

		Position( int x, int y, int zValue, int worldLayer );
		bool operator==( const Position& other )const;
		bool operator!=( const Position& other )const;

		Position2d toPosition2d()const;
		PositionXY toPositionXY()const;
		PositionXYZ toPositionXYZ()const;
	};
}