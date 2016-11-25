#ifndef PCSOUND_H
#define PCSOUND_H

#include <sys/io.h>


uint32_t play_note(char note, int octave, int time);
//Play sound using built in speaker
void play_sound(uint32_t nFrequence);

//make it shutup
void nosound();

void beep_it(int freq,int interval);
#endif
