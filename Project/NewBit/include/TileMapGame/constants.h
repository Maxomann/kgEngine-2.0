#pragma once
#include <_PluginCreator.h>

namespace kg
{
	namespace Constants
	{
		const std::string PACKAGE_NAME = "TileMapGame";

		const int CHUNK_SIZE = 1280;//px
		const int TILE_SIZE = 64;//px
		const int STANDART_TILE_ZVALUE = 0;

		const std::string ANIMATION_DEFAULT_STATE = "default";
		const sf::Time ANIMATION_MAX_SYNCED_TIME = sf::microseconds( std::numeric_limits<sf::Int64>::max() - 999999999 );

		//BLUEPRINT
		const std::string BLUEPRINT_PACKAGE_NAME = "package";
		const std::string BLUEPRINT_FILE_PATH = "path";
	};
}
