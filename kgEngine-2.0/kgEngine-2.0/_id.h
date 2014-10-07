#pragma once

namespace kg
{
	namespace ComponentId
	{
		enum
		{
			PositionComponent,
			SpriteComponent,
			LAST_ELEMENT
		};
	}

	namespace ComponentUpdateImportance
	{
		enum
		{
			PositionComponent,
			SpriteComponent
		};
	}

	namespace SystemId
	{
		enum
		{
			CameraSystem = ComponentId::LAST_ELEMENT
		};
	}

	namespace SystemUpdateImportance
	{
		enum
		{
			CameraSystem
		};
	}
}