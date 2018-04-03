
#include "Equalizer.h"

// Takes in a value and sets Treble_Out to be used by the equalizer
// Value should be between 0 and 5 inclusive (0 = no treble, 5 = large amount)
void update_treble(signed char value) {
	signed char treble = value;
	if (treble > 5) {
		treble = 5;
	}
	if (treble < 0) {
		treble = 0;
	}
	Treble_Out = treble;
}

// Takes in a value and sets Bass_Out to be used by the equalizer
// Value should be between 0 and 5 inclusive (0 = no bass, 5 = large amount)
void update_bass(signed char value) {
	signed char bass = value;
	if (bass > 5) {
		bass = 5;
	}
	if (bass < 0) {
		bass = 0;
	}
	Bass_Out = bass;
}

// Returns the current value of Treble_Out
signed char get_treble(void) {
	signed char treble = Treble_Out;
	return treble;
}

// Returns the current value of Bass_Out
signed char get_bass(void) {
	signed char bass = Bass_Out;
	return bass;
}
