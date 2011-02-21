//* -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*- */
/////////////////////////////////////////////////////////////////////////////////
//    Name:       src/platforms/sdl/PlatformAudioOut.cpp
//    Purpose:    Contains platform-specific audio playing routines
//    Author:     Shane T. Mueller, Ph.D.
//    Copyright:  (c) 2003-2011 Shane T. Mueller <smueller@obereed.net>
//    License:    GPL 2
//
//
//
//     This file is part of the PEBL project.
//
//    PEBL is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    PEBL is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with PEBL; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////

#include "PlatformAudioOut.h"
#include "../../devices/PAudioOut.h"

#include "../../utility/PEBLPath.h"
#include "../../utility/PError.h"
#include "../../base/Evaluator.h"

#include "SDL/SDL.h"
#include "SDL/SDL_audio.h"

#include <cmath>

void PlayCallBack(void * dummy, Uint8 * stream, int len);

//initiate static data for callback.
extern AudioInfo *gWaveStream=NULL;


using std::string;
using std::cerr;
using std::cout;
using std::endl;

PlatformAudioOut::PlatformAudioOut():
    PEBLObjectBase(CDT_AUDIOOUT)
{
}



PlatformAudioOut::PlatformAudioOut(const string &  soundfilename):
    PEBLObjectBase(CDT_AUDIOOUT)
{

    //Check to see if we can find the sound file; if not, call everything off.
    string mFilename = Evaluator::gPath.FindFile(soundfilename);

    if(mFilename == "")
        PError::SignalFatalError(string("Unable to find sound file [")  + soundfilename + string("]."));

    
    LoadSoundFile(mFilename.c_str());
    Initialize();    
}


PlatformAudioOut::~PlatformAudioOut()
{
    SDL_FreeWAV(mWave.audio);

}


bool PlatformAudioOut::LoadSoundFile(const string & soundfilename)
{
    //Check to see if we can find the sound file; if not, call everything off.
    mFilename = Evaluator::gPath.FindFile(soundfilename);

    if(mFilename == "")
        PError::SignalFatalError(string("Unable to find sound file [")  + soundfilename + string("]."));

 
	/* Load the wave file into memory */
	if ( SDL_LoadWAV(mFilename.c_str(), &mWave.spec, &mWave.audio, &mWave.audiolen) == NULL )
        {
			std::cerr << "Couldn't load " << mFileName << ": " << SDL_GetError() << std::endl;
            return false;
        }

    //If gWaveStream is loaded, we already have a frequency/format/channels set.
    //we need to convert the current sound to that format.

    if(gWaveStream)
        {
            cout << "***********************gwavestream existeth\n";
            bool samefreq = mWave.spec.freq==gWaveStream->spec.freq;
            bool sameformat = mWave.spec.format==gWaveStream->spec.format;
            bool samechannels = mWave.spec.channels==gWaveStream->spec.channels;
         
            //Only convert if we have a different format
            if(!(samefreq & sameformat&samechannels & samechannels))
                {
                    PError::SignalWarning("Warning: input file will be converted to new playback format.");
                    ConvertAudio(*gWaveStream);
                }

        }
    cerr << "------------------------------------\n";
    cerr << "Loading Sound File.  Specs:\n";
    cerr << "Frequency:   [" << mWave.spec.freq << "]\n";
    Variant form = "";
    switch(mWave.spec.format)
        {
            
        case AUDIO_U8:
            form="AUDIO_U8";
            break;
            
        case AUDIO_U16LSB:
            form = "AUDIO_U16LSB";
            break;


        case AUDIO_S8:
            form="AUDIO_S8";
            break;

        case AUDIO_S16:
            form="AUDIO_S16";
            break;

           
        default:
            form = "UNKNOWN";



        }

    cerr << "Format:      [" << form << "]\n";
    cerr << "Channels:    [" << mWave.spec.channels << "]\n";
    cerr << "Silence:     [" << mWave.spec.silence  << "]\n";
    cerr << "Samples:     [" << mWave.spec.samples  << "]\n";
    cerr << "Size:        [" << mWave.spec.size     << "]\n";
    cerr << "------------------------------------\n";
    mLoaded = true;


    mWave.name = mFilename.c_str();
    mWave.spec.callback = PlayCallBack;
    mWave.spec.userdata = &mWave;

    //Set the global playback wave to the current wave.
    if(!gWaveStream)
        {
            gWaveStream = &mWave;
        }

    return true;

}



bool PlatformAudioOut::ConvertAudio(AudioInfo & info)
{

    // Code heavily borrowed from some SDL tutorial

    SDL_AudioCVT cvt;           /* audio format conversion structure */
    SDL_AudioSpec loaded = mWave.spec;       /* format of the loaded data */
    SDL_AudioSpec spec = info.spec;
    Uint8 *new_buf;


    /* Build a conversion structure for converting the samples.
       This structure contains the data SDL needs to quickly
       convert between sample formats. */
    if (SDL_BuildAudioCVT(&cvt, loaded.format,
                          loaded.channels,
                          loaded.freq,
                          spec.format, spec.channels, spec.freq) < 0) 
        {
            PError::SignalFatalError(Variant("Unable to convert sound: ") + Variant(SDL_GetError()));
        }

    /* Since converting PCM samples can result in more data
       (for instance, converting 8-bit mono to 16-bit stereo),
       we need to allocate a new buffer for the converted data.
       Fortunately SDL_BuildAudioCVT supplied the necessary
       information. */
    cvt.len = mWave.audiolen;
    new_buf = (Uint8 *) malloc(cvt.len * cvt.len_mult);
    if (new_buf == NULL) 
        {
            PError::SignalFatalError("Memory allocation failed in PlatformAudioOut::ConvertAudio");
        }

    /* Copy the sound samples into the new buffer. */
    memcpy(new_buf, mWave.audio, mWave.audiolen);

    /* Perform the conversion on the new buffer. */
    cvt.buf = new_buf;
    if (SDL_ConvertAudio(&cvt) < 0) 
        {
            
            PError::SignalFatalError(Variant("Audio conversion error:")+ Variant(SDL_GetError()));
        }

    /* Swap the converted data for the original. */
    SDL_FreeWAV(mWave.audio);
    mWave.audio =  new_buf;
    mWave.audiolen = mWave.audiolen * cvt.len_mult;

    return true;
 }
               

bool PlatformAudioOut::CreateSineWave(float freq, long unsigned int mslength, long double amplitude)
{
    


    /* Allocate a desired SDL_AudioSpec */
    SDL_AudioSpec *spec = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));       
 

    if(gWaveStream)
        {
            //Use the already-available spec.
            cout <<"using preloaded sound spec\n";

            memcpy(spec,&(gWaveStream->spec),sizeof(SDL_AudioSpec));

        }
    else 
        {
            //Create a new spec

            /* Allocate space for the obtained SDL_AudioSpec */
            //    obtained = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));

            spec->freq =44100;
            spec->format=AUDIO_U8;
            spec->channels=1;
            spec->silence=0x80;
            spec->samples=4096;
            spec->callback= PlayCallBack;
            spec->userdata=&mWave;
        }

    //mslength is time in ms.
    //compute length in samples

    int bits=0;
    if((spec->format == AUDIO_U8) |
       (spec->format == AUDIO_S8) )
        {
            bits=1;

        }else if((spec->format == AUDIO_S16 )|
                 (spec->format == AUDIO_U16))
        {
            bits =2;

        }

    long unsigned int length = mslength/1000.0*spec->freq*spec->channels*bits;
    
    Uint8 *data = new Uint8[length];
    int dat;
    double base;

    for(unsigned int i=0; i<length;i+=spec->channels)
        {

            if(spec->format==AUDIO_U8)
                {
                    //base needs to be different for different formats
                    base = (sin(i*6.28/(spec->freq/freq))*amplitude+1)/2;
                }

            if(base<0)base=0;
            if(base>1)base=1;
            //base is bounded between 0 and 1
            dat = int(base*256);
            //cout << base << "," << amplitude  << ","<< dat << endl;

            //Copy dat to each channel
            for(int j = 0; j < spec->channels;j+=bits)
                data[i+j] = dat;
        }



    LoadSoundFromData(data,length,spec);

    return true;
}



//;unsigned int freq,int size)
bool PlatformAudioOut::LoadSoundFromData( Uint8 *buffer, long unsigned int size, SDL_AudioSpec *spec)
{


    /* setup audio */
    //SDL_AudioSpec spec;
    SDL_AudioSpec obtained;
    
    
    /* Allocate a desired SDL_AudioSpec */
    //spec = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));
    
    /* Allocate space for the obtained SDL_AudioSpec */
    //    obtained = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));
    

    /*
      spec.freq =freq;
      spec.format=AUDIO_U8;
      spec.channels=1;
      spec.silence=0x80;
      spec.samples=4096;
      spec.callback= PlayCallBack;
      spec.userdata=&mWave;
    */


    mWave.spec = *spec;
    mWave.audio=buffer;
    mWave.audiopos=0;
    mWave.audiolen=size;
    mWave.volume=100;
    mWave.name="Generated data";
        

	/* Load the wave file into memory */
    //	if ( SDL_LoadWAV_RW(rw,1, &mWave.spec, &mWave.audio, &mWave.audiolen) == NULL )
    //        {
    //			std::cerr << "Couldn't load created audio data: " << SDL_GetError() << std::endl;
    //            return false;
    //        }

    cout << "opening\n";
    if(SDL_OpenAudio(spec,&obtained)<0)
        {
            cout << "can't open audio--already opened.\n";
            mWave.spec = *spec;
        }else
        {
     
            mWave.spec=obtained;
            delete(spec);
        }
    //I can only delete spec if this is original.  


    cerr << "------------------------------------\n";
    cerr << "Loading Sound Data.  Specs:\n";
    cerr << "Frequency:   [" << mWave.spec.freq << "]\n";

    Variant form = "";
    switch(mWave.spec.format)
        {
            
        case AUDIO_U8:
            form="AUDIO_U8";
            break;
            
        case AUDIO_U16LSB:
            form = "AUDIO_U16LSB";
            break;


        case AUDIO_S8:
            form="AUDIO_S8";
            break;

        case AUDIO_S16:
            form="AUDIO_S16";
            break;

           
        default:
            form = "UNKNOWN";



        }

    cerr << "Format:      [" << form << "]\n";
    cerr << "Channels:    [" << mWave.spec.channels << "]\n";
    cerr << "Silence:     [" << mWave.spec.silence  << "]\n";
    cerr << "Samples:     [" << mWave.spec.samples  << "]\n";
    cerr << "Size:        [" << mWave.spec.size     << "]\n";
    cerr << "------------------------------------\n";
    mLoaded = true;




    //Check to see if we can find the sound file; if not, call everything off.
    mFilename = "<INTERNALLY GENERATED>";


    mWave.name = mFilename.c_str();
    mWave.spec.callback = PlayCallBack;
    mWave.spec.userdata = &mWave;

 
    //Set the global playback wave to the current wave.
    if(!gWaveStream)
        {
            gWaveStream = &mWave;
        }

    return true;

}





//This must be called after the audio is initialized but before it can
//be played.  It actually opens the audio device for playing.
bool PlatformAudioOut::Initialize()
{	
    
    if (mLoaded &&  SDL_OpenAudio(&mWave.spec, NULL) < 0 ) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        return false;
	}
    
    //Reset the position to the beginning.
    mWave.audiopos = 0;

    return true;
}

//  This plays the sound using the callback mixer function (in the background)
//
bool PlatformAudioOut::Play()
{

    SDL_LockAudio();
    mWave.audiopos = 0;
    gWaveStream = &mWave;
    SDL_UnlockAudio();

    //    Initialize();

    SDL_PauseAudio(0);
    
    return true;
}


//This will play the file, not returning until it is complete.
//Do not count on immediate return, as the function goes to sleep
bool PlatformAudioOut::PlayForeground()
{

    

    SDL_LockAudio();

    mWave.audiopos = 0;
    gWaveStream = &mWave;

    SDL_UnlockAudio();



    SDL_PauseAudio(0);
    while(SDL_GetAudioStatus() == SDL_AUDIO_PLAYING)
        {
            //Wait at least 10 ms before checking again.
            SDL_Delay(10);
            //cout << "---------- playing    ["<<SDL_GetTicks() << endl;
        }

    SDL_PauseAudio(1);
    return true;

}

bool PlatformAudioOut::Stop()
{
    SDL_PauseAudio(1);
    //Set the audio stream back to the beginning.
    //SDL_CloseAudio();
    mWave.audiopos=0;

    return true;
}



void PlayCallBack(void * udata, Uint8 * stream, int len)
{


    //    cerr << "Callback Called\n" <<endl;
    Uint8 * waveptr;
    int waveleft;

    //Cast udata to a proper form--this is dangerous and nasty.
    AudioInfo * wave = gWaveStream;//(AudioInfo*)(udata);


    //Put pointer at the proper place in the buffer.
    waveptr = wave->audio + wave->audiopos;
    waveleft = wave->audiolen - wave->audiopos;

    //   cerr << "waveleft: " <<  waveleft << "  len:" << len << endl;
    if(waveleft >= len)
        {

            SDL_MixAudio(stream, waveptr, len, SDL_MIX_MAXVOLUME);
            //This may appear in future formats.
            //SDL_MixAudioFormat(stream,waveptr,wave->spec,len,SDL_MIX_MAXVOLUME);
            wave->audiopos += len;
            
        }
    else
        {
            //This plays the rest of the file and stops playing.
            SDL_MixAudio(stream, waveptr, waveleft, SDL_MIX_MAXVOLUME-20);
            wave->audiopos += waveleft;
            SDL_PauseAudio(1);
            wave->audiopos=0;  //Reset it back to the beginning.
            
        }
    
}
