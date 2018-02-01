#include <unistd.h>
#include "Button.h"

int main()
{
	int i = 0, state = 1;
	void* n;
	void* o;
	void* p;
	void* q;

	clear();

	printf("Hello from Nios II!\n");

	initButton();

	n = new(Button, 200, 100, 100, 100);
	o = new(Button, 500, 100, 100, 100);
	p = new(Button, 200, 300, 100, 100);
	q = new(Button, 500, 300, 100, 100);

	while(1){
		clear();
		printf("Test\n");
		move(n,state*10,0);
		move(q,-state*10,0);
		move(o,0,state*10);
		move(p,0,-state*10);
		draw(n, 0, 0);
		draw(p, 0, 0);
		draw(o, 0, 0);
		draw(q, 0, 0);
		i++;
		if(i == 30)
		{
			i = 0;
			state = -state;
		}
		usleep(1000000);
	}

	delete(n);

	return 0;
}
