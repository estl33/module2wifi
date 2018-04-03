/*
 * Equalizer.h
 *
 *  Created on: Feb 14, 2018
 *      Author: f3o0b
 */

#ifndef EQUALIZER_H_
#define EQUALIZER_H_

#define Treble_Out (*(volatile signed char*) (0x0000030))
#define Bass_Out (*(volatile signed char*) (0x0000040))

// Returns the current value of Treble_Out
signed char get_treble(void);

// Returns the current value of Bass_Out
signed char get_bass(void);

// Takes in a value and sets Treble_Out to be used by the equalizer
// Value should be between 0 and 5 inclusive (0 = no treble, 5 = large amount)
void update_treble(signed char value);

// Takes in a value and sets Bass_Out to be used by the equalizer
// Value should be between 0 and 5 inclusive (0 = no bass, 5 = large amount)
void update_bass(signed char value);

#endif /* EQUALIZER_H_ */
