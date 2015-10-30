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
				ANIMATION,
				CAMERA,

				LAST_ELEMENT
			};
		}

		namespace GameStatePluginId
		{
			enum
			{
				FIRST_ELEMENT = ComponentPluginId::LAST_ELEMENT,

				//Default is 0 (defined in 'id_internal.h')
				SINGLEPLAYER,
				CONSOLE,

				LAST_ELEMENT
			};
		}

		namespace SpecialPluginId
		{
			enum
			{
				FIRST_ELEMENT = GameStatePluginId::LAST_ELEMENT,

				ANIMATION_HANDLER_EASY,

				LAST_ELEMENT
			};
		}



		namespace SystemUpdateImportance
		{
			enum
			{
				FIRST_ELEMENT,

				ANIMATION_SYSTEM,
				CHUNK_GENERATOR,
				CHUNK_SYSTEM,
				GRAPHICS_SYSTEM,

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
				ANIMATION,
				CAMERA,

				LAST_ELEMENT
			};
		}

		namespace Input
		{
			enum
			{
				FIRST_ELEMENT,

				//DefaultGameState
				SHUT_DOWN,

				//SingleplayerGameState
				RELOAD_SAVE,
				SWITCH_CONSOLE,
				MOVE_UP,
				MOVE_DOWN,
				MOVE_LEFT,
				MOVE_RIGHT,
				MOVE_UP_FAST,
				MOVE_DOWN_FAST,
				MOVE_LEFT_FAST,
				MOVE_RIGHT_FAST,
				MOVE_UP_SLOW,
				MOVE_DOWN_SLOW,
				MOVE_LEFT_SLOW,
				MOVE_RIGHT_SLOW,
				ZOOM_IN,
				ZOOM_OUT,
				PAUSE,

				LAST_ELEMENT
			};
		}
	}
}
