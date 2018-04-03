#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Serial.h"
#include "Bound.h"
#include "Canvas.h"
#include "List.h"


#define XRES 800
#define YRES 480

class Application;
class Context;
class Activity;
class View;
class Renderer;
class Canvas;

class Application {
  friend class Context;
  Activity* currentActivity;

  Serial* serial;
  Renderer* renderer;

public:
  Application();
  void startApplication(Activity* activity);
  void startActivity(Activity* activity);
private:
  void stopCurrentActivity();
  void superLoop();
};

class Context {
  Application* application;
public:
  Context();
  ~Context();
  void attachApplication(Application* app);
  void detachApplication();
protected:
  Serial* getSerial();
  void startActivity(Activity* activity);
private:
};

class Renderer {
	View* rootView;
public:
	static Renderer* getInstance();
	void setRootView(View* rootView);
	void render();
};

class View : public Context {
  friend class Renderer;

  int x, y, width, height;
  List childrenViews;
  View* parent;
public:
  View();
  void setX(int x);
  void setY(int y);
  void setWidth(int width);
  void setHeight(int height);

  virtual void onAttach();
  virtual void onDetach();
  virtual void onDraw(Canvas canvas);
  virtual void onMeasure();
  virtual int onDownPress();
  virtual int onMovePress();
  virtual int onUpPress();
  virtual int onClick();

  void draw(Bound parent);
  void measure();

  void attachView(View* view);
  void detachView(View* view);

  void invalidate();
protected:
  Bound* bounds;
  Bound* damaged;
  void invalidateInParent(Bound damaged);
};

class Activity : public Context {
  friend class Application;
  View rootView;
public:
  virtual void onCreate();
  virtual void onDestroy();
  virtual string getName();
protected:
  void attachView(View* view);
  void detachView();
private:
};

#endif
