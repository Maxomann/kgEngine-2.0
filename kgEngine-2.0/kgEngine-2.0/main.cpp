#include "stdafx.h"
#include "Core.h"
#include "Blueprint.h"
#include "Callback.h"

using namespace std;
using namespace kg;

int main()
{
#ifndef _DEBUG
	try
	{
#endif
		Core core;
		core.init();
		while( !core.shouldTerminate() )
			core.update();
#ifndef _DEBUG
	}
	catch( std::exception& e )
	{
		cout << "exception: " << e.what() << endl;
		system( "pause" );
	}
#endif
}