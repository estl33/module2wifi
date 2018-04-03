#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "View.h"
#include "Bounds.h"

extern const void* Histogram;

struct Histogram {
	const struct View _;

  double amplitude;
};

void setAmplitude(void* _self, double amplitude);

void initHistogram();

#endif
