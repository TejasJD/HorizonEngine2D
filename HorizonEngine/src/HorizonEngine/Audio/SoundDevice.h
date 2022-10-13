#pragma once

#ifndef HZN_SOUNDDEVICE_H
#define HZN_SOUNDDEVICE_H

#include "HorizonEngine/Core/Core.h"
#include <AL/alc.h>
#include <AL/al.h>

#include <Logging/Logging.h>

namespace Hzn
{
	class HZN_API SoundDevice
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


