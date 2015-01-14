#ifndef _AUDIO_HPP
#define _AUDIO_HPP
#include <portaudio.h>
#include <iostream>
#include "fm.hpp"

class AudioEngine
{
private:
    PaStream *stream;

    static int paCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
    {
      float *buffer = static_cast<float *>(outputBuffer);
      AudioEngine *audioEngine = static_cast<AudioEngine *>(userData);
      while ( framesPerBuffer -- )
        *buffer++ = audioEngine->volume.get() * audioEngine->fmsynth.process();
    }

    void connect()
    {
        int err;
        err = Pa_OpenDefaultStream(&stream,
                                   0, // no inputs
                                   1, // mono output
                                   paFloat32, // 32-bit floating point
                                   SAMPLE_RATE,
                                   256, // buffer size
                                   paCallback, //your callback function
                                   this ); //data to be passed to callback. In C++, it is frequently (void *)this
        if (err != paNoError )
            throw runtime_error{Pa_GetErrorText(err)};
        err = Pa_StartStream(stream);
        if (err != paNoError )
            throw runtime_error{Pa_GetErrorText(err)};
    }

    void disconnect()
    {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }

public:
    Param volume;
    FMSynth &fmsynth;

    AudioEngine(FMSynth &fm) : fmsynth{fm}, volume{0.5}
    {
    }

    vector<const PaDeviceInfo *> getDevices()
    {
        vector<const PaDeviceInfo *> devices;
        int device_count = Pa_GetDeviceCount();
        if ( device_count < 0 )
            throw runtime_error{"Could not query devices"};
        for (int i = 0; i < device_count; i ++)
        {
            const PaDeviceInfo *device = Pa_GetDeviceInfo(i);
            devices.push_back(device);
        }
        return devices;
    }

    void init()
    {
        auto err = Pa_Initialize();
        if (err != paNoError )
            throw runtime_error{Pa_GetErrorText(err)};
        connect();
    }

    void close()
    {
        disconnect();
        Pa_Terminate();
    }
};

#endif
