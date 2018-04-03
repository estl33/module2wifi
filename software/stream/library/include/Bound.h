#ifndef BOUND_H_
#define BOUND_H_

#define INSIDE 0
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

class Bound {
public:
  Bound(int x1, int y1, int x2, int y2);
  const int x1, y1, x2, y2;  
  int containsPoint(int x, int y);
  int containsX(int x);
  int containsY(int y);
  int intersectWith(Bound a);

  Bound* merge(Bound a);
  Bound* intersect(Bound a);
  Bound* cohenSutherland(double x1, double y1, double x2, double y2);
private:
  int computeOutCode(double x, double y);
};

#endif
