#include<_PluginCreator.h>

using namespace std;
using namespace kg;

extern "C"
{
	class Testsystem : public System
	{
	public:
		virtual void init( Engine& engine, SystemManager& systemManager )
		{
			cout << "init" << endl;
		}

		virtual void sfmlEvent( const sf::Event& sfEvent )
		{
			static bool call = true;

			if( call )
			{
				cout << "event" << endl;
				call = false;
			}
		}

		virtual void update( Engine& engine, World& world )
		{
			static bool call = true;

			if( call )
			{
				cout << "update" << endl;
				call = false;
			}
		}

		virtual double getUpdateImportance() const
		{
			return 0.0;
		}

		virtual const std::string& getPluginName() const
		{
			return "TestSystem";
		}

		virtual const int getPluginId() const
		{
			return 333;
		}

	};

	DLL_EXPORT void kgConnect( PluginManager& pluginManager )
	{
		pluginManager.addSystemPlugin( std::make_shared<PluginFactory<System, Testsystem>>( 333, "TestSystem" ) );
	}
}
