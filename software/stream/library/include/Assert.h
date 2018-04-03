/**
Taken from https://stackoverflow.com/a/37264642
**/

#ifndef ASSERT_H_
#define ASSERT_H_

#include <string>

using namespace std;

#include "Log.h"
#include "List.h"

#ifndef NDEBUG
#define M_Assert(Expr, Msg) \
    __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg);
#else
#define M_Assert(Expr, Msg) ;
#endif

#ifndef STACK
#define M_Stack() \
    __M_Stack(__PRETTY_FUNCTION__);
#define M_Unstack() \
    __M_Unstack(__PRETTY_FUNCTION__);
#else
#define M_Stack() ;
#define M_Unstack() ;
#endif

void __M_Assert(string expr_str, bool expr, string file, int line, string msg);
void __M_Stack(string function);
void __M_Unstack(string function);

#endif
