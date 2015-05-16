#pragma once

namespace kg
{
	struct Constants
	{
		static const int CHUNK_SIZE = 640;//px
		static const int TILE_SIZE = 64;//px
		static const int STANDART_TILE_ZVALUE = 0;

		static const std::string ANIMATION_DEFAULT_STATE;
		static const sf::Time ANIMATION_MAX_SYNCED_TIME;

		//BLUEPRINT
		static const std::string BLUEPRINT_PACKAGE_NAME;
		static const std::string BLUEPRINT_FILE_PATH;
	};

	__declspec(selectany) const sf::Time Constants::ANIMATION_MAX_SYNCED_TIME = sf::microseconds( std::numeric_limits<sf::Int64>::max()-999999999 );

	__declspec(selectany) const std::string Constants::ANIMATION_DEFAULT_STATE = "default";

	__declspec(selectany) const std::string Constants::BLUEPRINT_FILE_PATH = "path";

	__declspec(selectany) const std::string Constants::BLUEPRINT_PACKAGE_NAME = "package";
}
