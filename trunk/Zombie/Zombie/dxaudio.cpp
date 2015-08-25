// Beginning Game Programming, Second Edition
// dxaudio.cpp - DirectSound framework header file

#include "DirectSound.h"
#include "dxaudio.h"

CSoundManager *dsound = NULL;

bool DirectSound_Init(HWND hwnd)
{
	dsound = new CSoundManager();

	HRESULT result;
	result = dsound->Initialize(hwnd, DSSCL_PRIORITY);
	if (result != DS_OK) return false;

	result = dsound->SetPrimaryBufferFormat(2, 22050, 16);
	if (result != DS_OK)
	{
		return false;
	}

	return true;
}

void DirectSound_Shutdown()
{
	if (dsound) 
	{
		delete dsound;
	}
}


CSound *LoadSound(string filename)
{
    HRESULT result;

    //create local reference to wave data
    CSound *wave = NULL;

    //attempt to load the wave file
	char s[255];
	sprintf(s, "%s", filename.c_str());
    result = dsound->Create(&wave, s);
    if (result != DS_OK)
	{
        wave = NULL;
	}

    //return the wave
    return wave;
}

void PlaySound(CSound *sound)
{
    sound->Play();
}

void LoopSound(CSound *sound)
{
    sound->Play(0, DSBPLAY_LOOPING);
}

void StopSound(CSound *sound)
{
    sound->Stop();
}
