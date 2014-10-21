#include<_PluginCreator.h>

#include "Position.h"
#include "BoundingBox.h"
#include "Graphics.h"

#include "GraphicsSystem.h"

using namespace std;
using namespace kg;

extern "C"
{
	DLL_EXPORT void kgConnect( PluginManager& pluginManager )
	{
		//Components
		pluginManager.addComponentPlugin(
			std::make_shared<PluginFactory<Component, Position>>(
			( int )id::ComponentPluginId::POSITION,
			Position::PLUGIN_NAME ) );
		pluginManager.addComponentPlugin(
			std::make_shared<PluginFactory<Component, BoundingBox>>(
			( int )id::ComponentPluginId::BOUNDING_BOX,
			BoundingBox::PLUGIN_NAME ) );
		pluginManager.addComponentPlugin(
			std::make_shared<PluginFactory<Component, Graphics>>(
			( int )id::ComponentPluginId::GRAPHICS,
			Graphics::PLUGIN_NAME ) );
		//Camera not registered as a plugin. A camera entity is created in the GraphicsSystem


		//Systems
		pluginManager.addSystemPlugin(
			std::make_shared<PluginFactory<System, GraphicsSystem>>(
			( int )id::SystemPluginId::GRAPHICS_SYSTEM,
			GraphicsSystem::PLUGIN_NAME ) );
	}
}
