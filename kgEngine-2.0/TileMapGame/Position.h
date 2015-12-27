#pragma once

namespace kg
{
	struct Position
	{
		int x = 0;
		int y = 0;
		int zValue = 0;
		int worldLayer = 0;

		Position( int x, int y, int zValue, int worldLayer );
		bool operator==( const Position& other )const;
	};

	struct Position2d
	{
		int x;
		int y;
		int worldLayer;

		Position2d( int x, int y, int worldLayer );
		bool operator==( const Position2d& other )const;
	};
}