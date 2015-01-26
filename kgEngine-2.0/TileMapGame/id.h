#pragma once
#include "stdafx.h"

namespace kg
{
	namespace id
	{
		namespace SystemPluginId
		{
			enum
			{
				FIRST_ELEMENT,

				CHUNK_SYSTEM,
				CHUNK_GENERATOR,
				GRAPHICS_SYSTEM,
				GAME_CONTROLLER,

				LAST_ELEMENT
			};
		}

		namespace ComponentPluginId
		{
			enum
			{
				FIRST_ELEMENT = SystemPluginId::LAST_ELEMENT,

				TRANSFORMATION,
				GLOBAL_BOUNDS,
				GRAPHICS,
				CAMERA,

				LAST_ELEMENT
			};
		}

		namespace SystemUpdateImportance
		{
			enum
			{
				FIRST_ELEMENT,

				CHUNK_SYSTEM,
				CHUNK_GENERATOR,
				GRAPHICS_SYSTEM,
				GAME_CONTROLLER,

				LAST_ELEMENT
			};
		}

		namespace ComponentUpdateImportance
		{
			enum
			{
				FIRST_ELEMENT,

				TRANSFORMATION,
				GLOBAL_BOUNDS,
				GRAPHICS,
				CAMERA,

				LAST_ELEMENT
			};
		}
	}
}
