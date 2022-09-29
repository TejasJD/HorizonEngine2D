#pragma once
#include "HorizonEngine/APIMacros.h"
#include "pch.h"


namespace Hzn 
{

	enum  MessageType
	{
		Log, Warn, Error
	};

	class HZN_API LogMessage
	{
	public:
		static void SetMessageType(MessageType* type);
		static void SetMessage(char* message);
		static MessageType* GetMessageType();
		static char* GetLogMessage();
		

	private:
		static MessageType* m_type;
		static char* l_message;
	};

	

	class HZN_API LogConsole
	{
	public:
		static void HznLog(char* message);
		static void LogError(char* message);
		static void LogWarning(char* message);
		static std::vector<LogMessage*> GetErrors();
		static std::vector<Hzn::LogMessage*> GetWarnings();
		static std::vector<Hzn::LogMessage*> GetLogs();
		static std::vector<Hzn::LogMessage*> GetMessages();

	private:
		static std::vector<Hzn::LogMessage*> messages;
	};
}