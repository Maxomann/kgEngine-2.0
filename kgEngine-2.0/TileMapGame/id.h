#pragma once

namespace kg
{
	namespace id
	{
		namespace SystemPluginId
		{
			enum
			{
				FIRST_ELEMENT,

				ANIMATION_SYSTEM,
				CHUNK_SYSTEM,
				CHUNK_GENERATOR,
				GRAPHICS_SYSTEM,
				GAME_CONTROLLER,

				LAST_ELEMENT
			};
		};

		namespace ComponentPluginId
		{
			enum
			{
				FIRST_ELEMENT = SystemPluginId::LAST_ELEMENT,

				TRANSFORMATION,
				GLOBAL_BOUNDS,
				GRAPHICS,
				ANIMATION,
				CAMERA,

				LAST_ELEMENT
			};
		};

		namespace SystemUpdateImportance
		{
			enum
			{
				FIRST_ELEMENT,

				ANIMATION_SYSTEM,
				CHUNK_SYSTEM,
				CHUNK_GENERATOR,
				GRAPHICS_SYSTEM,
				GAME_CONTROLLER,

				LAST_ELEMENT
			};
		};

		namespace ComponentUpdateImportance
		{
			enum
			{
				FIRST_ELEMENT,

				TRANSFORMATION,
				GLOBAL_BOUNDS,
				GRAPHICS,
				ANIMATION,
				CAMERA,

				LAST_ELEMENT
			};
		};
	};
}
