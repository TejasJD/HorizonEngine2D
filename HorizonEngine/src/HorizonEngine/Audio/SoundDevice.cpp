#include "SoundDevice.h"


namespace Hzn
{
	//Initialize the sound play device
	SoundDevice* SoundDevice::Init()
	{
		static SoundDevice* snd_device = new SoundDevice();
		return snd_device;
	}

	SoundDevice::SoundDevice()
	{
		//set nullptr to get system default sound play device
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


	//close the sound play device
	SoundDevice::~SoundDevice()
	{

		alcMakeContextCurrent(nullptr);
		alcDestroyContext(p_ALCContext);
		alcCloseDevice(p_ALCDevice);
		
	}

}
