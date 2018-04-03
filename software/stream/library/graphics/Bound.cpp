#include "Bound.h"

#include "Assert.h"

Bound::Bound(int _x1, int _y1, int _x2, int _y2) : x1(_x1), x2(_x2), y1(_y1), y2(_y2) {
	M_Stack();
	M_Assert(_x1 <= _x2, "x1 needs to be smaller than x2");
	M_Assert(_y1 <= _y2, "y1 needs to be smaller than y2");
	M_Unstack();
}

int Bound::containsPoint(int x, int y) {
	return Bound::containsX(x) && Bound::containsY(y);
}

int Bound::containsX(int x) {
  if(x < x1 || x > x2)
    return 0;

  return 1;
}

int Bound::containsY(int y) {
  if(y < y1 || y > y2)
    return 0;

  return 1;
}

int Bound::intersectWith(Bound a) {
  return (x1 < a.x2 && x2 > a.x1 &&
    y1 < a.y2 && y2 > a.y1);
}

Bound* Bound::merge(Bound a) {
  int y1 = MIN(Bound::y1, a.y1);
  int x1 = MIN(Bound::x1, a.x1);
  int x2 = MAX(Bound::x2, a.x2);
  int y2 = MAX(Bound::y2, a.y2);

  return new Bound(x1, y1, x2, y2);
}

Bound* Bound::intersect(Bound a) {
  int x1 = MAX(Bound::x1, a.x1);
  int y1 = MAX(Bound::y1, a.y1);
  int x2 = MIN(Bound::x2, a.x2);
  int y2 = MIN(Bound::y2, a.y2);

  return new Bound(x1, y1, x2, y2);
}

int Bound::computeOutCode(double x, double y) {
	int xmin = Bound::x1;
	int xmax = Bound::x2;
	int ymin = Bound::y1;
	int ymax = Bound::y2;

	int code = INSIDE;          // initialised as being inside of [[clip window]]

	if (x < xmin)           // to the left of clip window
		code |= LEFT;
	else if (x > xmax)      // to the right of clip window
		code |= RIGHT;
	if (y < ymin)           // below the clip window
		code |= BOTTOM;
	else if (y > ymax)      // above the clip window
		code |= TOP;

	return code;
}

Bound* Bound::cohenSutherland(double x1, double y1, double x2, double y2) {
	int xmin = Bound::x1;
	int xmax = Bound::x2;
	int ymin = Bound::y1;
	int ymax = Bound::y2;

	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	int outcode0 = computeOutCode(x1, y1);
	int outcode1 = computeOutCode(x2, y2);
	int accept = 0;

	while (1) {
		if (!(outcode0 | outcode1)) {
			// bitwise OR is 0: both points inside window; trivially accept and exit loop
			accept = 1;
			break;
		} else if (outcode0 & outcode1) {
			// bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
			// or BOTTOM), so both must be outside window; exit loop (accept is false)
			break;
		} else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			double x, y;

			// At least one endpoint is outside the clip rectangle; pick it.
			int outcodeOut = outcode0 ? outcode0 : outcode1;

			// Now find the intersection point;
			// use formulas:
			//   slope = (y2 - y1) / (x2 - x1)
			//   x = x1 + (1 / slope) * (ym - y1), where ym is ymin or ymax
			//   y = y1 + slope * (xm - x1), where xm is xmin or xmax
			// No need to worry about divide-by-zero because, in each case, the
			// outcode bit being tested guarantees the denominator is non-zero
			if (outcodeOut & TOP) {           // point is above the clip window
				x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
				y = ymax;
			} else if (outcodeOut & BOTTOM) { // point is below the clip window
				x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
				y = ymin;
			} else if (outcodeOut & RIGHT) {  // point is to the right of clip window
				y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
				x = xmax;
			} else if (outcodeOut & LEFT) {   // point is to the left of clip window
				y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
				x = xmin;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				x1 = x;
				y1 = y;
				outcode0 = computeOutCode(x1, y1);
			} else {
				x2 = x;
				y2 = y;
				outcode1 = computeOutCode(x2, y2);
			}
		}
	}

	if(accept) {
		return new Bound(x1, y1, x2, y2);
	} else {
		return 0;
	}
}
