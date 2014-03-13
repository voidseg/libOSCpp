////////////////////////////////////////////////////////////
// sampleplayer.h                                         //
// --------------                                         //
// A simple example of a sample player that uses          //
// libmikmod and OSS.                                     //
//                                                        //
// Contains code from the mikmod player application.      //
//                                                        //
// Daniel Holth                                           //
// 18 Oct. 2001                                           //
////////////////////////////////////////////////////////////

#ifndef SAMPLEPLAYER_H
#define SAMPLEPLAYER_H

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <mikmod.h>

#ifdef __cplusplus
extern "C" {
#endif

int   sampleplayer_init (int argc, char **argv);
void* sampleplayer_start(void *);
int   sampleplayer_play (int which);
int   sampleplayer_play_speed (int which, int speed);
int   sampleplayer_play_speed_pan_volume(int sample, int rate, int panning, int volume);

// Returns value or max if value > max, min if value < min.
int   clipInteger(int value, int max, int min);

#ifdef __cplusplus
}
#endif

#endif
