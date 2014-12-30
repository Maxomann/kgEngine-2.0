#pragma once

namespace kg
{
	namespace id
	{
		enum class SystemPluginId : int
		{
			FIRST_ELEMENT,

			CHUNK_SYSTEM,
			GRAPHICS_SYSTEM,
			SAVE_SYSTEM,
			GAME_CONTROLLER,

			LAST_ELEMENT
		};

		enum class ComponentPluginId : int
		{
			FIRST_ELEMENT = ( int )SystemPluginId::LAST_ELEMENT,

			POSITION,
			SIZE,
			ROTATION,
			GLOBAL_BOUNDS,
			GRAPHICS,
			CAMERA,

			LAST_ELEMENT
		};

		enum class SystemUpdateImportance : int
		{
			FIRST_ELEMENT,

			CHUNK_SYSTEM,
			GRAPHICS_SYSTEM,
			SAVE_SYSTEM,
			GAME_CONTROLLER,

			LAST_ELEMENT
		};

		enum class ComponentUpdateImportance : int
		{
			FIRST_ELEMENT,

			POSITION,
			SIZE,
			ROTATION,
			GLOBAL_BOUNDS,
			GRAPHICS,
			CAMERA,

			LAST_ELEMENT
		};
	}
}
