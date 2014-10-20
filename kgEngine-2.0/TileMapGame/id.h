namespace kg
{
	namespace id
	{
		enum class SystemPluginId : int
		{
			FIRST_ELEMENT,

			GRAPHICS_SYSTEM,

			LAST_ELEMENT
		};

		enum class ComponentPluginId : int
		{
			FIRST_ELEMENT = ( int )SystemPluginId::LAST_ELEMENT,

			POSITION,
			BOUNDING_BOX,
			GRAPHICS,

			LAST_ELEMENT
		};

		enum class SystemUpdateImportance : int
		{
			FIRST_ELEMENT,

			GRAPHICS_SYSTEM,

			LAST_ELEMENT
		};

		enum class ComponentUpdateImportance : int
		{
			FIRST_ELEMENT,

			POSITION,
			BOUNDING_BOX,
			GRAPHICS,

			LAST_ELEMENT
		};
	}
}
