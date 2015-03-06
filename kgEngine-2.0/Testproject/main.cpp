#include "Blueprint.h"

using namespace std;
using namespace sf;
using namespace placeholders;
using namespace kg::blueprint;

int main()
{
	BlueprintManager blue;

	blue.parse( "./test2.blueprint" );
	blue.link();

	auto& bps = blue.getBlueprintsByName();
	auto& ents = blue.getEntitiesById();
	auto& exts = blue.getComponentContainerExtensionsByName();

	for( const auto& el : bps )
	{
		cout << "BLUEPRINT " << el.second.getName() << endl;

		auto& comps = el.second.getComponentsByName();

		for( const auto& comp : comps )
		{
			cout << "COMPONENT " << comp.second.getName() << endl;
			auto& vals = comp.second.getComponentValueReferencesByName();

			for( const auto& val : vals )
				cout << "VALUE " << val.first << "::" << val.second->getName() << " = " << val.second->getRawValue() << endl;

			cout << "END COMPONENT" << endl;
		}

		cout << "END BLUEPRINT" << endl << endl;
	}


	for( const auto& el : ents )
	{
		cout << "ENTITY " << el.second.getName() << ":" << el.second.getId() << endl;

		auto& comps = el.second.getComponentsByName();

		for( const auto& comp : comps )
		{
			cout << "COMPONENT " << comp.second.getName() << endl;
			auto& vals = comp.second.getComponentValueReferencesByName();

			for( const auto& val : vals )
				cout << "VALUE " << val.first << "::" << val.second->getName() << " = " << val.second->getRawValue() << endl;

			cout << "END COMPONENT" << endl;
		}

		cout << "END ENTITY" << endl << endl;
	}

	for( const auto& el : exts )
	{
		cout << "EXTENSION " << el.second.getName() << endl;

		auto& comps = el.second.getComponentsByName();

		for( const auto& comp : comps )
		{
			cout << "COMPONENT " << comp.second.getName() << endl;
			auto& vals = comp.second.getComponentValueReferencesByName();

			for( const auto& val : vals )
				cout << "VALUE " << val.first << "::" << val.second->getName() << " = " << val.second->getRawValue() << endl;

			cout << "END COMPONENT" << endl;
		}

		cout << "END EXTENSION" << endl << endl;
	}


	cout << "finished" << endl;

	system( "pause" );
}
