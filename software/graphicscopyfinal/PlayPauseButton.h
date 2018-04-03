/*
 * PlayPauseButton.h
 *
 *  Created on: Feb 15, 2018
 *      Author: f3o0b
 */

#ifndef PLAYPAUSEBUTTON_H_
#define PLAYPAUSEBUTTON_H_

#include "View.h"

extern const void* PlayPauseButton;

struct PlayPauseButton {
	const struct View _;

	int colour;
	int focusColour;

	int focused;
	int play;
};

void initPlayPauseButton();

#endif /* PLAYPAUSEBUTTON_H_ */
