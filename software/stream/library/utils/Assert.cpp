#include "Assert.h"

List stack;

void __M_Assert(string expr_str, bool expr, string file, int line, string msg)
{
    if (!expr)
    {
        Log_Error("Assert failed", msg);
        Log_Error("Expected", expr_str);
        Log_Error("Source", file);

        Iterator* iterator = stack.getIteratorEnd();
        string* str = (string*) iterator->prev();
        while(str != 0) {
        	Log_Error("", *str);
        	str = (string*) iterator->prev();
        }
        while(1) {} //Get stuck
    }
}

#define LOG_FUNC

void __M_Stack(string function)
{
	string* str = new string(function);
#ifdef LOG_FUNC
	Log_Low("Func", "in - " + *str);
#endif
    stack.insertEnd((void*) str);
}

void __M_Unstack(string function)
{
#ifdef LOG_FUNC
	Log_Low("Func", "out - " + function);
#endif
    stack.deleteEnd();
}
