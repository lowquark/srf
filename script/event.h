#ifndef EVENT_H
#define EVENT_H

#include <script/luaref.h>

typedef struct listener {
  luaref_t ref;
  struct listener * next;
} listener_t;

typedef struct event {
  listener_t * listeners;
} event_t;

void event_clear(event_t * event);
void event_add_listener(event_t * event, lua_State * L, int idx);
void event_emit(const event_t * event);
void event_emit_args(const event_t * event, int (* push_fn)(lua_State *, void *), void * ud);

#endif
