////////////////////////////////////////////////////////////
// sampleplayer.c                                         //
// --------------                                         //
// A simple example of a sample player that uses          //
// libmikmod and OSS.  And OSC.                           //
//                                                        //
// Contains a tiny amount of code from the mikmod         //
// player application.                                    //
//                                                        //
// Daniel Holth                                           //
// 18 Oct. 2001                                           //
////////////////////////////////////////////////////////////

#include "sampleplayer.h"

//static BOOL player_on=0;
//static BOOL library_on=0;

SAMPLE **samples;
int      numSamples;

static void Player_InitLib(void)
{
  /* Register the loaders we want to use:  */
  MikMod_RegisterAllLoaders();

  /* Register the drivers we want to use: */
  MikMod_RegisterAllDrivers();
}


int sampleplayer_init(int argc, char *argv[])
{
  int i;
  // int direction;
  
  if(argc < 2)
  {
    printf("Please give at least one argument, the name of\n"
           "uncompressed mono .wav files. (other kinds of\n"
           ".wav files will crash the mikmod engine)\n");
    exit(-1);
  }

  Player_InitLib();
  MikMod_Init("");

  MikMod_SetNumVoices(-1, 64);

  numSamples = argc - 1;
  samples = (SAMPLE**)malloc(sizeof(SAMPLE) * numSamples);
 
  printf("Loading %d samples...\n", numSamples);
  for(i=0; i<numSamples; i++)
  {
      printf("loading %s\n", argv[i+1]);
    *(samples+i) = Sample_Load(argv[i+1]);
  }
  
  return 0;
}

void* sampleplayer_start(void *ignored)
{
  int i;
  
  MikMod_EnableOutput();
  
  i = 0;
  do {
    MikMod_Update();
    usleep(100);
  } while (1);
  
  MikMod_DisableOutput();
  for(i=0; i<numSamples; i++)
  {
    Sample_Free(*(samples+i));
  }
  
  MikMod_Exit();

  return NULL;
}


int sampleplayer_play(int sampleNumber)
{  
  SBYTE  voice;

  if(sampleNumber >= 0 && sampleNumber < numSamples)
  {
    printf("Playing sample %d\n", sampleNumber);
    voice = Sample_Play(*(samples+sampleNumber), 0, 0);
    Voice_SetPanning(voice, 127);
  }
  else
  {
    printf("You tried to play sample number %d\n"
           "but you only loaded %d samples.\n", sampleNumber, numSamples);
  }

  return 0;
}


int sampleplayer_play_speed(int sampleNumber, int speed)
{  
  SBYTE  voice;

  if(sampleNumber >= 0 && sampleNumber < numSamples)
  {
    printf("Playing sample %d\n", sampleNumber);
    voice = Sample_Play(*(samples+sampleNumber), 0, 0);
    Voice_SetPanning(voice, 127);
    Voice_SetFrequency(voice, speed);
  }
  else
  {
    printf("You tried to play sample number %d\n"
           "but you only loaded %d samples.\n", sampleNumber, numSamples);
  }

  return 0;
}


int sampleplayer_play_speed_pan_volume(int sampleNumber, 
                                       int rate, 
                                       int panning, 
                                       int volume)
{
  SBYTE  voice;

  panning = clipInteger(panning, 0, 255);
  volume  = clipInteger(volume,  0, 256);
  rate    = clipInteger(rate,    0, INT_MAX);

  printf("pan: %d volume: %d rate: %d\n", panning, volume, rate);

  if(sampleNumber >= 0 && sampleNumber < numSamples)
  {
    printf("Playing sample %d\n", sampleNumber);
    voice = Sample_Play(*(samples+sampleNumber), 0, 0);
    
    Voice_SetPanning  (voice, panning);
    Voice_SetFrequency(voice, rate);
    Voice_SetVolume   (voice, volume);
  }
  else
  {
    printf("You tried to play sample number %d\n"
           "but you only loaded %d samples.\n", sampleNumber, numSamples);
  }

  return 0;

}


// Returns value clipped to [min, max] inclusive.
int clipInteger(int value, int min, int max)
{
  int result;
  
  result = (value < min)  ? min : value;
  result = (result > max) ? max : value;

  return result;
}
