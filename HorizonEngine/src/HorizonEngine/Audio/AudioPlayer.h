#pragma once

#ifndef HZN_AUDIOPLAYER_H
#define HZN_AUDIOPLAYER_H

#include "HorizonEngine/Core/Core.h"

#include<windows.h>
#include <atlstr.h>

#pragma comment(lib,"winmm.lib")

namespace Hzn
{
	class HZN_API AudioPlayer
	{
	public:
		AudioPlayer(std::string filePath);

		bool OpenDevice();
		bool PlayAudio();
		bool PauseAudio();
		bool StopAudio();
		
	private:
		static std::string path;
	};

}


#endif // !HZN_AUDIOPLAYER_H