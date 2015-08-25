// Beginning Game Programming, Second Edition
// dxaudio.h - DirectSound framework header file
#pragma once

#ifndef _DXAUDIO_H
#define _DXAUDIO_H

#include "DirectSound.h"

#include <string>

using namespace std;


//primary DirectSound object
extern CSoundManager *dsound;

//function prototypes
bool DirectSound_Init(HWND hwnd);
void DirectSound_Shutdown();
CSound *LoadSound(string filename);
void PlaySound(CSound *sound);
void LoopSound(CSound *sound);
void StopSound(CSound *sound);


#endif

