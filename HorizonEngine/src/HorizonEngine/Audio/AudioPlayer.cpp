#include "AudioPlayer.h"

namespace Hzn
{

	std::string AudioPlayer::path;

	AudioPlayer::AudioPlayer(std::string filePath)
	{
		path = filePath;
	}


	bool AudioPlayer::OpenDevice()
	{

		std::wstring open = L"open \"" + std::wstring(path.begin(), path.end()) + L"\" type MPEGVideo alias currentMusic";

		bool result = mciSendStringW(
			open.c_str(),
			NULL,
			0,
			NULL
		);

		return result;
	}

	bool AudioPlayer::PlayAudio()
	{

		bool result = mciSendStringW(
			L"play currentMusic",
			NULL,
			0,
			NULL
		);

		return result;
	}

	bool AudioPlayer::PauseAudio()
	{
		bool result = mciSendStringW(
			L"pause currentMusic",
			NULL,
			0,
			NULL
		);

		return result;
	}

	bool AudioPlayer::StopAudio()
	{
		bool result = mciSendStringW(
			L"stop currentMusic",
			NULL,
			0,
			NULL
		);

		return result;
	}
}