#pragma once
#include "stdafx.h"

namespace kg
{
	using PositionType = int;

	struct PositionXY
	{
		PositionType x = 0;
		PositionType y = 0;

		PositionXY() = default;
		PositionXY( PositionType x, PositionType y );
		bool operator==( const PositionXY& other )const;
		bool operator!=( const PositionXY& other )const;

		sf::Vector2i toVector2i()const;
	};

	struct Position2d
	{
		PositionType x = 0;
		PositionType y = 0;
		PositionType worldLayer = 0;

		Position2d() = default;
		Position2d( PositionType x, PositionType y, PositionType worldLayer );
		bool operator==( const Position2d& other )const;
		bool operator!=( const Position2d& other )const;

		PositionXY toPositionXY()const;
	};

	typedef Position2d ChunkPosition;

	struct PositionXYZ
	{
		PositionType x = 0;
		PositionType y = 0;
		PositionType zValue = 0;

		PositionXYZ() = default;
		PositionXYZ( PositionType x, PositionType y, PositionType zValue );
		bool operator==( const PositionXYZ& other )const;
		bool operator!=( const PositionXYZ& other )const;

		PositionXY toPositionXY()const;
	};

	struct Position
	{
		PositionType x = 0;
		PositionType y = 0;
		PositionType zValue = 0;
		PositionType worldLayer = 0;

		Position() = default;
		Position( PositionType x, PositionType y, PositionType zValue, PositionType worldLayer );
		bool operator==( const Position& other )const;
		bool operator!=( const Position& other )const;

		Position2d toPosition2d()const;
		PositionXY toPositionXY()const;
		PositionXYZ toPositionXYZ()const;
	};
}