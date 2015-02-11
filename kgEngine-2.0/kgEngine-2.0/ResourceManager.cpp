#include "ResourceManager.h"
using namespace std;
using namespace sf;

namespace kg
{


	std::vector<std::string> readFileToVector( const std::string& path )
	{
		std::vector<std::string> returnValue;

		std::ifstream file;
		file.open( path );

		if( !file.is_open() )
			throw std::exception();

		std::copy( std::istream_iterator<std::string>( file ),
				   std::istream_iterator<std::string>(),
				   std::back_inserter( returnValue ) );

		return returnValue;
	}

}

