#include <drivers/pcsound.h>
#include <stdlib/conv.h>

// TODO - Make the sound api available in floating-point integers.
//Play sound using built in speaker
uint32_t return_freq(char note, int octave){
	uint8_t magic = (uint8_t)note;
	float notes[14] = {
					 13.75000000000000
					,15.43385316425387
					,16.35159783128741
					,18.35404799483797
					,20.60172230705437
					,21.82676446456275
					,24.49971474885934

					,14.56761754744031
					,11.00000000000000
					,17.32391443605451
					,19.44543648263006
					,11.00000000000000
					,23.12465141947715
					,25.95654359874658
				};
	uint8_t normFreq = magic - 65;
	uint8_t intFreq = (magic - 97) + 7;
	if(magic >= 65 && magic < 72)
		return (uint32_t)notes[normFreq]*pow(2, (octave-1) );
	else if(magic >= 97 && magic < 104)
		return (uint32_t)notes[intFreq]*pow(2, (octave-1) );
	return 11;	
}

uint32_t play_note(char note, int octave, int time){
	uint32_t freq = return_freq(note, octave);
	if(freq != 11){
		play_sound(freq);
		delay(time);
		nosound();
	}
	return freq;
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

void nosound() {
 	uint8_t tmp = inb(0x61) & 0xFC;
 	outb(0x61, tmp);
 }

void beep_it(int freq, int interval){
	play_sound(freq);
	delay(interval);
	nosound();
}