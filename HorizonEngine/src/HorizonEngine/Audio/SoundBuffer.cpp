#include "SoundBuffer.h"
#include <sndfile.h>
#include <inttypes.h>
#include <AL/alext.h>
#include <Logging/Logging.h>

namespace Hzn {

	SoundBuffer* SoundBuffer::get()
	{
		static SoundBuffer* sndbuf = new SoundBuffer();
		return sndbuf;
	}

	ALuint SoundBuffer::addSound(const char* filename)
	{

		ALenum err, format;
		ALuint buffer;
		SNDFILE* sndfile;
		SF_INFO sfinfo;
		short* membuf;
		sf_count_t num_frames;
		ALsizei num_bytes;

		// Open the audio file
		sndfile = sf_open(filename, SFM_READ, &sfinfo);
		if (!sndfile)
		{
			HZN_CORE_ERROR("Failed to open this audio file");
			return 0;
		}
		if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
		{
			sf_close(sndfile);
			return 0;
		}

		// Get the sound format, and figure out the OpenAL format
		format = AL_NONE;
		if (sfinfo.channels == 1)
		{
			format = AL_FORMAT_MONO16;
		}
			
		else if (sfinfo.channels == 2)
		{
			format = AL_FORMAT_STEREO16;
		}
			
		else if (sfinfo.channels == 3)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (sfinfo.channels == 4)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT3D_16;
		}
		if (!format)
		{
			sf_close(sndfile);
			return 0;
		}

		// Decode the whole audio file to a buffer.
		membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

		num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
		if (num_frames < 1)
		{
			free(membuf);
			sf_close(sndfile);
			return 0;
		}
		num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

		/* Buffer the audio data into a new buffer object, then free the data and
		 * close the file.
		 */
		buffer = 0;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

		free(membuf);
		sf_close(sndfile);

		/* Check if an error occured, and clean up if so. */
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
			if (buffer && alIsBuffer(buffer))
				alDeleteBuffers(1, &buffer);
			return 0;
		}

		// add to the list of known buffers
		p_SoundBuffers.push_back(buffer);

		return buffer;
	}

	bool SoundBuffer::removeSound(const ALuint& buffer)
	{
		auto it = p_SoundBuffers.begin();
		while (it != p_SoundBuffers.end())
		{
			if (*it == buffer)
			{
				alDeleteBuffers(1, &*it);

				it = p_SoundBuffers.erase(it);

				return true;
			}
			else {
				++it;
			}
		}
		return false;
	}


	SoundBuffer::SoundBuffer()
	{
		p_SoundBuffers.clear();

	}

	SoundBuffer::~SoundBuffer()
	{
		alDeleteBuffers(p_SoundBuffers.size(), p_SoundBuffers.data());

		p_SoundBuffers.clear();
	}

}
