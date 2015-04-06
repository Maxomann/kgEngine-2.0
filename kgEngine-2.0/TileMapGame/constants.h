#pragma once

namespace kg
{
	struct Constants
	{
		static const int CHUNK_SIZE = 640;//px
		static const int TILE_SIZE = 64;//px
		static const int STANDART_TILE_ZVALUE = 0;

		//BLUEPRINT
		static const std::string BLUEPRINT_PACKAGE_NAME;
		static const std::string BLUEPRINT_FILE_PATH;
	};

	__declspec(selectany) const std::string Constants::BLUEPRINT_FILE_PATH = "path";

	__declspec(selectany) const std::string Constants::BLUEPRINT_PACKAGE_NAME = "package";
}
