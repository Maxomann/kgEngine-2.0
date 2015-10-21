#include "Console.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{

	void Console::log( const std::string& message )
	{
		m_log.push_back( LogEntry( LOG_TYPE::LOG,
								   chrono::system_clock::to_time_t( chrono::system_clock::now() ) ,
								   message ) );
		s_entryAdded( m_log.back() );
	}

	void Console::info( const std::string& message )
	{
		m_log.push_back( LogEntry( LOG_TYPE::INFO,
								   chrono::system_clock::to_time_t( chrono::system_clock::now() ),
								   message ) );
		s_entryAdded( m_log.back() );
	}

	void Console::error( const std::string& message )
	{
		m_log.push_back( LogEntry( LOG_TYPE::ERR,
								   chrono::system_clock::to_time_t( chrono::system_clock::now() ),
								   message ) );
		s_entryAdded( m_log.back() );
	}

	const Console::Log& Console::getLog()
	{
		return m_log;
	}

	LogEntry::LogEntry( const LOG_TYPE& type, const std::time_t& timestamp, const std::string& message )
		:type( type ),
		timestamp( timestamp ),
		message( message )
	{

	}

}
