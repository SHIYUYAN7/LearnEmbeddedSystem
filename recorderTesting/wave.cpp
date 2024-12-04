#include "wave.h"

WAV_HEADER CreateWaveHeader(int numChannels,                    
                            unsigned int sampleRate,            
                            unsigned short bitsPerSample)       
{
  WAV_HEADER header;
  header.riffSize = 0;
  header.numChannels = numChannels;
  header.sampleRate = sampleRate;
  header.bitsPerSample = bitsPerSample;
  header.bytesPerSecond = sampleRate * numChannels /8 * bitsPerSample;
  header.blockAlign = numChannels * bitsPerSample/8;
  header.dataSize = 0;
  return header;
}