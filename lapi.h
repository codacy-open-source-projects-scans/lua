/*
** $Id: lapi.h $
** Auxiliary functions from Lua API
** See Copyright Notice in lua.h
*/

#ifndef lapi_h
#define lapi_h


#include "llimits.h"
#include "lstate.h"


#if defined(LUA_USE_APICHECK)
#include <assert.h>
#define api_check(l,e,msg)	assert(e)
#else	/* for testing */
#define api_check(l,e,msg)	((void)(l), lua_assert((e) && msg))
#endif



/* Increments 'L->top.p', checking for stack overflows */
#define api_incr_top(L)  \
    (L->top.p++, api_check(L, L->top.p <= L->ci->top.p, "stack overflow"))


/*
** macros that are executed whenever program enters the Lua core
** ('lua_lock') and leaves the core ('lua_unlock')
*/
#if !defined(lua_lock)
#define lua_lock(L)	((void) 0)
#define lua_unlock(L)	((void) 0)
#endif



/*
** If a call returns too many multiple returns, the callee may not have
** stack space to accommodate all results. In this case, this macro
** increases its stack space ('L->ci->top.p').
*/
#define adjustresults(L,nres) \
    { if ((nres) <= LUA_MULTRET && L->ci->top.p < L->top.p) \
	L->ci->top.p = L->top.p; }


/* Ensure the stack has at least 'n' elements */
#define api_checknelems(L,n) \
       api_check(L, (n) < (L->top.p - L->ci->func.p), \
                         "not enough elements in the stack")


/* Ensure the stack has at least 'n' elements to be popped. (Some
** functions only update a slot after checking it for popping, but that
** is only an optimization for a pop followed by a push.)
*/
#define api_checkpop(L,n) \
	api_check(L, (n) < L->top.p - L->ci->func.p &&  \
                     L->tbclist.p < L->top.p - (n), \
			  "not enough free elements in the stack")


/*
** To reduce the overhead of returning from C functions, the presence of
** to-be-closed variables in these functions is coded in the CallInfo's
** field 'nresults', in a way that functions with no to-be-closed variables
** with zero, one, or "all" wanted results have no overhead. Functions
** with other number of wanted results, as well as functions with
** variables to be closed, have an extra check.
*/

#define hastocloseCfunc(n)	((n) < LUA_MULTRET)

/* Map [-1, inf) (range of 'nresults') into (-inf, -2] */
#define codeNresults(n)		(-(n) - 3)
#define decodeNresults(n)	(-(n) - 3)

#endif
