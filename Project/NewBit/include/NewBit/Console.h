#pragma once
#include "stdafx.h"
#include "Callback.h"

namespace kg
{
	enum DLL_EXPORT LOG_TYPE
	{
		LOG,
		INFO,
		ERR
	};

	struct DLL_EXPORT LogEntry
	{
		const LOG_TYPE type;
		const std::time_t timestamp;
		const std::string message;

		LogEntry( const LOG_TYPE& type, const std::time_t& timestamp, const std::string& message );
	};

	class DLL_EXPORT Console : boost::noncopyable
	{
	public:

		typedef std::vector<LogEntry> Log;

		void log( const std::string& message );
		void info( const std::string& message );
		void error( const std::string& message );

		const Log& getLog();

	signals:
		Signal<const LogEntry&> s_entryAdded;

	private:

		//log type, timestamp, message
		Log m_log;
	};
}
