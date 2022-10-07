#include "SoundDevice.h"
#include <AL/al.h>

#include <stdio.h>
#include <Logging/Logging.h>

namespace Hzn
{
	SoundDevice* SoundDevice::get()
	{
		static SoundDevice* snd_device = new SoundDevice();
		return snd_device;
	}

	SoundDevice::SoundDevice()
	{
		//get default device
		p_ALCDevice = alcOpenDevice(nullptr); 
		if (!p_ALCDevice)
		{
			HZN_CORE_ERROR("Failed to get sound device");
		}
			

		// create context
		p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);  
		if (!p_ALCContext)
		{
			HZN_CORE_ERROR("Failed to set sound context");
		}

		// make context current
		if (!alcMakeContextCurrent(p_ALCContext))
		{
			HZN_CORE_ERROR("Failed to make context current");
		}

		const ALCchar* name = nullptr;
		if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
		if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
			name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
		
	}

	SoundDevice::~SoundDevice()
	{
		if (!alcMakeContextCurrent(nullptr))
		{
			HZN_CORE_ERROR("Failed to set context to nullptr");
		}

		alcDestroyContext(p_ALCContext);
		if (p_ALCContext)
		{
			HZN_CORE_ERROR("Failed to unset during close");
		}
			

		if (!alcCloseDevice(p_ALCDevice))
		{
			HZN_CORE_ERROR("Failed to close sound device");
		}
	}

}
