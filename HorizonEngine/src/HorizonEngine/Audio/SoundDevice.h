#pragma once

#ifndef HZN_SOUNDDEVICE_H
#define HZN_SOUNDDEVICE_H

#include <AL/alc.h>
#include <AL/al.h>

#include "HorizonEngine/Core/Core.h"

namespace Hzn
{
	/// <summary>
	/// 
	/// </summary>
	class SoundDevice
	{
	public:
		static SoundDevice* Init();

	private:
		SoundDevice();
		~SoundDevice();

		ALCdevice* p_ALCDevice;
		ALCcontext* p_ALCContext;

	};
}

#endif // !HZN_SOUNDDEVICE_H


