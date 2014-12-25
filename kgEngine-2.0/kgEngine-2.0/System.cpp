#include "System.h"
using namespace std;
using namespace sf;

namespace kg
{
	SystemSaveInformation::SystemSaveInformation( const std::string& constructFromString )
	{
		m_fromString( constructFromString );
	}

	SystemSaveInformation::SystemSaveInformation( int systemId )
		: m_systemId( systemId )
	{

	}

	const std::vector<std::string> SystemSaveInformation::getInformation() const
	{
		return m_information;
	}

	void SystemSaveInformation::setInformation( const std::vector<std::string>& information )
	{
		m_information = information;
	}

	int SystemSaveInformation::getSystemId() const
	{
		return m_systemId;
	}

	void SystemSaveInformation::m_fromString( const std::string& str )
	{
		m_information.clear();

		std::string sID;
		std::string sValues;
		bool isInValues = false;
		for( const auto& el : str )
		{
			if( isInValues )
			{
				sValues.push_back( el );
			}
			else
			{
				if( el == '[' )
					isInValues = true;
				else
					sID.push_back( el );
			}
		}
		sValues.pop_back();//because last char is ']'

		m_systemId = atoi( sID.c_str() );
		boost::split( m_information, sValues, boost::is_any_of( ";" ) );
	}

	std::string SystemSaveInformation::toString() const
	{
		if( m_information.size() == 0 )
			return "";

		std::string retVal;
		retVal = std::to_string( m_systemId );
		retVal += "[";
		for( const auto & el : m_information )
		{
			retVal += el;
			retVal += ";";
		}
		retVal.pop_back();
		retVal += "]";

		return retVal;
	}

	bool SystemSaveInformation::hasInformation() const
	{
		return m_information.size();
	}



}
