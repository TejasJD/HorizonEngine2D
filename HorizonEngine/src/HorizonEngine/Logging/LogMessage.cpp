#include "LogMessage.h"


namespace Hzn
{

	MessageType* LogMessage::m_type;
	char* LogMessage::l_message;
	
	void LogMessage::SetMessageType(MessageType* type)
	{
		m_type = type;
	}

	void LogMessage::SetMessage(char* message)
	{
		l_message = message;
	}
	

	MessageType* LogMessage::GetMessageType()
	{
		return m_type;
	}

	char* LogMessage::GetLogMessage()
	{
		return l_message;
	}

	std::vector<Hzn::LogMessage*> LogConsole::messages;

	void LogConsole::HznLog(char* message)
	{
		MessageType mt = Log;

		LogMessage* lm = new LogMessage();
		
		lm->SetMessage(message);
		lm->SetMessageType(&mt);
		messages.insert(messages.end(), lm);
	}

	void LogConsole::LogError(char* message)
	{
		
		MessageType mt = Error;
		LogMessage* lm = new LogMessage();

		lm->SetMessage(message);
		lm->SetMessageType(&mt);
		messages.insert(messages.end(), lm);
	}

	void LogConsole::LogWarning(char* message)
	{
		
		MessageType mt = Warn;
		LogMessage* lm = new LogMessage();

		lm->SetMessage(message);
		lm->SetMessageType(&mt);
		messages.insert(messages.end(), lm);
	}

	std::vector<LogMessage*> LogConsole::GetErrors()
	{
		std::vector<LogMessage*> ErrorMessages;
		
		for (int i = 0; i < messages.size(); i++)
		{
			MessageType* mt = messages[i]->GetMessageType();

			if (*mt == MessageType(Log))
			{
				ErrorMessages.insert(ErrorMessages.end(), messages[i]);
			}
		}

		return ErrorMessages;
	}

	std::vector<Hzn::LogMessage*> LogConsole::GetWarnings()
	{
		std::vector<LogMessage*> WarnMessages;

		for (int i = 0; i < messages.size(); i++)
		{
			MessageType* mt = messages[i]->GetMessageType();

			if (*mt == MessageType(Log))
			{
				WarnMessages.insert(WarnMessages.end(), messages[i]);
			}
		}

		return WarnMessages;
	}

	std::vector<Hzn::LogMessage*> LogConsole::GetLogs()
	{
		
		std::vector<LogMessage*> LogMessages;

		for (int i = 0; i < messages.size(); i++)
		{
			MessageType* mt = messages[i]->GetMessageType();

			if (*mt == MessageType(Log))
			{
				LogMessages.insert(LogMessages.end(), messages[i]);
			}
		}

		return LogMessages;
	}

	std::vector<Hzn::LogMessage*> LogConsole::GetMessages()
	{
		return messages;
	}
}