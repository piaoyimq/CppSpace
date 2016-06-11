/** hsm.c -- Hierarchical State Machine implementation
 * M. Samek, 01/07/00
 */
#include <assert.h>
#include "hsm.h"
/* state machine insturmentation macros */
#ifndef HSM_INSTR
#define ON_ENTRY(ctx_, state_) ((void)0)
#define ON_EXIT(ctx_, state_)  ((void)0)
#else
#include <stdio.h>
#define ON_ENTRY(ctx_, state_) \
        printf("%s::%s-ENTRY;", (ctx_), (state_))
#define ON_EXIT(ctx_, state_)  \
        printf("%s::%s-EXIT;",  (ctx_), (state_))
#endif

static Msg const startMsg = { START_EVT };
static Msg const entryMsg = { ENTRY_EVT };
static Msg const exitMsg = { EXIT_EVT };
#define MAX_STATE_NESTING 8
/* State ctor */
State::State(char const *n, State *s, EvtHndlr h) :
		name(n), super(s), hndlr(h)
{
}
/* Hsm ctor */
Hsm::Hsm(char const *n, EvtHndlr topHndlr) :
		top("top", 0, topHndlr), name(n)
{
}
/* enter and start the top state */
void Hsm::onStart()
{
	State *entryPath[MAX_STATE_NESTING];
	register State **trace;
	register State *s;
	curr = &top;
	next = 0;
	ON_ENTRY(name, curr->name);
	curr->onEvent(this, &entryMsg);
	while (curr->onEvent(this, &startMsg), next)
	{
		for (s = next, trace = entryPath, *trace = 0; s != curr; s = s->super)
			*(++trace) = s; /* trace path to target */
		while (s = *trace--)
		{ /* retrace entry from source */
			ON_ENTRY(name, s->name);
			s->onEvent(this, &entryMsg);
		}
		curr = next;
		next = 0;
	}
}
/* state machine "engine" */
void Hsm::onEvent(Msg const *msg)
{
	State *entryPath[MAX_STATE_NESTING];
	register State **trace;
	register State *s;
	for (s = curr; s; s = s->super)
	{
		if ((msg = s->onEvent(this, msg)) == 0)
		{ /*processed?*/
			if (next)
			{ /* state transition taken? */
				for (s = next, trace = entryPath, *trace = 0; s != curr; s = s->super)
					*(++trace) = s; /* trace path to target */
				while (s = *trace--)
				{ /* retrace entry from LCA */
					ON_ENTRY(name, s->name);
					s->onEvent(this, &entryMsg);
				}
				curr = next;
				next = 0;
				while (curr->onEvent(this, &startMsg), next)
				{
					for (s = next, trace = entryPath, *trace = 0; s != curr; s = s->super)
						*(++trace) = s; /* record path to target */
					while (s = *trace--)
					{ /* retrace the entry */
						ON_ENTRY(name, s->name);
						s->onEvent(this, &entryMsg);
					}
					curr = next;
					next = 0;
				}
			}
			break; /* event processed */
		}
	}
}
/* exit current states and all superstates up to LCA */
void Hsm::exit_(unsigned char toLca)
{
	register State *s;
	for (s = curr; toLca > 0; --toLca, s = s->super)
	{
		ON_EXIT(name, s->name);
		s->onEvent(this, &exitMsg);
	}
	curr = s;
}
/* find # of levels to Least Common Ancestor */
unsigned char Hsm::toLCA_(State *target)
{
	State *s, *t;
	unsigned char toLca = 1;
	for (s = curr->super; s != 0; ++toLca, s = s->super)
		for (t = target; t != 0; t = t->super)
			if (s == t)
				return toLca;
	return 0;
}
