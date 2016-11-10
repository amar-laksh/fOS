#include <drivers/pcsound.h>

// TODO - Make the notes work for any octave, maybe try a formula?
//Play sound using built in speaker

uint32_t return_freq(char note, int octave){
	uint8_t magic = (uint8_t)note;
	int32_t notes[14] = {
					440, 493, 523, 587, 659, 698, 783, 466, 11, 554, 622, 11, 739, 830
				};
	if(magic >= 65 && magic < 72)
		return notes[magic-65];
	else if(magic >= 97 && magic < 104)
		return notes[(magic-97)+7];
	return 11;
}

void play_note(char note, int octave, int time){
	uint32_t freq = return_freq(note, octave);
	if(freq != 11){
		play_sound(freq);
		delay(time*1000);
		nosound();
		sprintf("\n%d\n",freq);
	}
}

void play_sound(uint32_t nFrequency) {
 	uint32_t Div;
 	uint8_t tmp;
 	Div = 1193180 / nFrequency;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }

 //make it shutup
void nosound() {
 	uint8_t tmp = inb(0x61) & 0xFC;
 	outb(0x61, tmp);
 }

void beep_it(int freq, int interval){
	play_sound(freq);
	delay(interval);
	nosound();
}