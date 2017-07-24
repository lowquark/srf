
#include "event.h"

#include <stdlib.h>

void event_clear(event_t * event) {
  listener_t * l = event->listeners;
  while(l) {
    listener_t * next = l->next;
    luaref_clear(&l->ref);
    free(l);
    l = next;
  }
  event->listeners = NULL;
}
void event_add_listener(event_t * event, lua_State * L, int idx) {
  listener_t * newl = calloc(sizeof(*newl), 1);
  luaref_set(&newl->ref, L, idx);

  listener_t ** lslot = &event->listeners;
  while(*lslot) {
    lslot = &(*lslot)->next;
  }
  *lslot = newl;
}
void event_emit(const event_t * event) {
  listener_t * l = event->listeners;
  while(l) {
    luaref_push(&l->ref);

    lua_call(l->ref.L, 0, 0);

    l = l->next;
  }
}
void event_emit_args(const event_t * event, int (* push_fn)(lua_State *, void *), void * ud) {
  listener_t * l = event->listeners;
  while(l) {
    lua_State * L = luaref_push(&l->ref);

    int nargs = push_fn(L, ud);
    lua_call(L, nargs, 0);

    l = l->next;
  }
}
