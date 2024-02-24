#include "pch.h"

#include <AL/al.h>
#include <sndfile.h>
#include <AL/alext.h>

#include "HorizonEngine/Logging/Logging.h"
#include "HorizonEngine/Audio/SoundBuffer.h"

namespace Hzn {

// Initialize the sound buffer
SoundBuffer* SoundBuffer::Init() {
  static SoundBuffer* sndbuf = new SoundBuffer();
  return sndbuf;
}

// Add sound to buffer
ALuint SoundBuffer::LoadSound(const char* filename) {
  ALenum err, format;
  ALuint buffer;
  ALsizei num_bytes;
  SNDFILE* sndfile;
  SF_INFO sfinfo;
  sf_count_t num_frames;
  short* membuf;

  // Open the audio file, it will return nullptr if open failed
  sndfile = sf_open(filename, SFM_READ, &sfinfo);
  if (!sndfile) {
    HZN_CORE_ERROR("Failed to open this audio file");
    return 0;
  }

  if (sfinfo.frames < 1 ||
      sfinfo.frames > (sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels) {
    sf_close(sndfile);
    return 0;
  }

  // set the OpenAL format according to the sound format
  format = AL_NONE;
  switch (sfinfo.channels) {
    case 1:
      format = AL_FORMAT_MONO16;
      break;
    case 2:
      format = AL_FORMAT_STEREO16;
      break;
    case 3:

      if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) ==
          SF_AMBISONIC_B_FORMAT)
        format = AL_FORMAT_BFORMAT2D_16;

      break;
    case 4:

      if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) ==
          SF_AMBISONIC_B_FORMAT)
        format = AL_FORMAT_BFORMAT3D_16;

      break;
    default:
      break;
  }

  if (!format) {
    sf_close(sndfile);
    return 0;
  }

  // Decode the whole audio file to a buffer.
  membuf = static_cast<short*>(
      malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

  num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
  if (num_frames < 1) {
    free(membuf);
    sf_close(sndfile);
    return 0;
  }
  num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

  // Buffer the audio data into a new buffer object, then free the data and
  // close the file.

  buffer = 0;
  alGenBuffers(1, &buffer);
  alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

  free(membuf);
  sf_close(sndfile);

  // If al error occurs, delete buffer
  err = alGetError();
  if (err != AL_NO_ERROR) {
    HZN_CORE_ERROR("OpenAL Error: {0}", alGetString(err));

    if (buffer && alIsBuffer(buffer)) alDeleteBuffers(1, &buffer);
    return 0;
  }

  // add current buffer to the list of known buffers
  p_SoundBuffers.push_back(buffer);

  return buffer;
}

SoundBuffer::SoundBuffer() { p_SoundBuffers.clear(); }

SoundBuffer::~SoundBuffer() {
  alDeleteBuffers(p_SoundBuffers.size(), p_SoundBuffers.data());

  p_SoundBuffers.clear();
}

}  // namespace Hzn
