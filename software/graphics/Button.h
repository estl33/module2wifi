/*
 * Button.h
 *
 *  Created on: Jan 30, 2018
 *      Author: quent
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "View.h"

extern const void* Button;

struct Button {
	const struct View _;
	void* circle;
	void* rect;
};

void initButton();

#endif /* BUTTON_H_ */
