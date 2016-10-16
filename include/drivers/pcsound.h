#ifndef PCSOUND_H
#define PCSOUND_H

#include <sys/io.h>


//Play sound using built in speaker
static void play_sound(uint32_t nFrequence);

//make it shutup
static void nosound();

#endif
