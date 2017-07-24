
#include "input.h"

#include <SDL2/SDL_thread.h>

static int get_key(unsigned int * key, SDL_Scancode code) {
  switch(code) {
    case SDL_SCANCODE_ESCAPE:
      *key = INPUT_KEY_ESCAPE; return 1;

    case SDL_SCANCODE_A:
      *key = INPUT_KEY_A; return 1;
    case SDL_SCANCODE_B:
      *key = INPUT_KEY_B; return 1;
    case SDL_SCANCODE_C:
      *key = INPUT_KEY_C; return 1;
    case SDL_SCANCODE_D:
      *key = INPUT_KEY_D; return 1;
    case SDL_SCANCODE_E:
      *key = INPUT_KEY_E; return 1;
    case SDL_SCANCODE_F:
      *key = INPUT_KEY_F; return 1;
    case SDL_SCANCODE_G:
      *key = INPUT_KEY_G; return 1;
    case SDL_SCANCODE_H:
      *key = INPUT_KEY_H; return 1;
    case SDL_SCANCODE_I:
      *key = INPUT_KEY_I; return 1;
    case SDL_SCANCODE_J:
      *key = INPUT_KEY_J; return 1;
    case SDL_SCANCODE_K:
      *key = INPUT_KEY_K; return 1;
    case SDL_SCANCODE_L:
      *key = INPUT_KEY_L; return 1;
    case SDL_SCANCODE_M:
      *key = INPUT_KEY_M; return 1;
    case SDL_SCANCODE_N:
      *key = INPUT_KEY_N; return 1;
    case SDL_SCANCODE_O:
      *key = INPUT_KEY_O; return 1;
    case SDL_SCANCODE_P:
      *key = INPUT_KEY_P; return 1;
    case SDL_SCANCODE_Q:
      *key = INPUT_KEY_Q; return 1;
    case SDL_SCANCODE_R:
      *key = INPUT_KEY_R; return 1;
    case SDL_SCANCODE_S:
      *key = INPUT_KEY_S; return 1;
    case SDL_SCANCODE_T:
      *key = INPUT_KEY_T; return 1;
    case SDL_SCANCODE_U:
      *key = INPUT_KEY_U; return 1;
    case SDL_SCANCODE_V:
      *key = INPUT_KEY_V; return 1;
    case SDL_SCANCODE_W:
      *key = INPUT_KEY_W; return 1;
    case SDL_SCANCODE_X:
      *key = INPUT_KEY_X; return 1;
    case SDL_SCANCODE_Y:
      *key = INPUT_KEY_Y; return 1;
    case SDL_SCANCODE_Z:
      *key = INPUT_KEY_Z; return 1;

    case SDL_SCANCODE_KP_0:
    case SDL_SCANCODE_0:
      *key = INPUT_KEY_0; return 1;
    case SDL_SCANCODE_KP_1:
    case SDL_SCANCODE_1:
      *key = INPUT_KEY_1; return 1;
    case SDL_SCANCODE_KP_2:
    case SDL_SCANCODE_2:
      *key = INPUT_KEY_2; return 1;
    case SDL_SCANCODE_KP_3:
    case SDL_SCANCODE_3:
      *key = INPUT_KEY_3; return 1;
    case SDL_SCANCODE_KP_4:
    case SDL_SCANCODE_4:
      *key = INPUT_KEY_4; return 1;
    case SDL_SCANCODE_KP_5:
    case SDL_SCANCODE_5:
      *key = INPUT_KEY_5; return 1;
    case SDL_SCANCODE_KP_6:
    case SDL_SCANCODE_6:
      *key = INPUT_KEY_6; return 1;
    case SDL_SCANCODE_KP_7:
    case SDL_SCANCODE_7:
      *key = INPUT_KEY_7; return 1;
    case SDL_SCANCODE_KP_8:
    case SDL_SCANCODE_8:
      *key = INPUT_KEY_8; return 1;
    case SDL_SCANCODE_KP_9:
    case SDL_SCANCODE_9:
      *key = INPUT_KEY_9; return 1;

    default: return 0;
  }
}

typedef struct vector {
  void ** elements;
  unsigned int max_size;
  unsigned int size;
} vector_t;

void vector_push(vector_t * v, void * p) {
  if(v->max_size == 0) {
    v->max_size = 8;
    v->elements = malloc(v->max_size*sizeof(*v->elements));
    v->size = 0;
  } else if(v->size == v->max_size) {
    v->max_size *= 2;
    v->elements = realloc(v->elements, v->max_size*sizeof(*v->elements));
  }

  v->elements[v->size] = p;
  v->size ++;
}
void vector_clear(vector_t * v) {
  free(v->elements);
  v->elements = NULL;
  v->max_size = 0;
  v->size = 0;
}

SDL_mutex * quit_listeners_lock;
vector_t quit_listeners;

SDL_mutex * keydown_listeners_lock;
vector_t keydown_listeners;

SDL_mutex * keyup_listeners_lock;
vector_t keyup_listeners;

typedef struct quit_listener {
  input_QuitHandler handler;
  void * data;
} quit_listener_t;
typedef struct keydown_listener {
  input_KeyHandler handler;
  void * data;
} keydown_listener_t;
typedef struct keyup_listener {
  input_KeyHandler handler;
  void * data;
} keyup_listener_t;

void input_on_quit(input_QuitHandler h, void * d) {
  quit_listener_t * listener = calloc(1, sizeof(*listener));
  listener->handler = h;
  listener->data = d;

  SDL_LockMutex(quit_listeners_lock);
  vector_push(&quit_listeners, listener);
  SDL_UnlockMutex(quit_listeners_lock);
}
void input_on_keydown(input_KeyHandler h, void * d) {
  keydown_listener_t * listener = calloc(1, sizeof(*listener));
  listener->handler = h;
  listener->data = d;

  SDL_LockMutex(keydown_listeners_lock);
  vector_push(&keydown_listeners, listener);
  SDL_UnlockMutex(keydown_listeners_lock);
}
void input_on_keyup(input_KeyHandler h, void * d) {
  keyup_listener_t * listener = calloc(1, sizeof(*listener));
  listener->handler = h;
  listener->data = d;

  SDL_LockMutex(keyup_listeners_lock);
  vector_push(&keyup_listeners, listener);
  SDL_UnlockMutex(keyup_listeners_lock);
}

static void input_emit_quit() {
  for(unsigned int i = 0 ; i < quit_listeners.size ; i ++) {
    quit_listener_t * listener = (quit_listener_t *)quit_listeners.elements[i];
    listener->handler(listener->data);
  }
}
static void input_emit_keydown(unsigned int key_id) {
  for(unsigned int i = 0 ; i < keydown_listeners.size ; i ++) {
    keydown_listener_t * listener = (keydown_listener_t *)keydown_listeners.elements[i];
    listener->handler(key_id, listener->data);
  }
}
static void input_emit_keyup(unsigned int key_id) {
  for(unsigned int i = 0 ; i < keyup_listeners.size ; i ++) {
    keyup_listener_t * listener = (keyup_listener_t *)keyup_listeners.elements[i];
    listener->handler(key_id, listener->data);
  }
}

void input_init() {
  quit_listeners_lock = SDL_CreateMutex();
  keydown_listeners_lock = SDL_CreateMutex();
  keyup_listeners_lock = SDL_CreateMutex();
}
void input_deinit() {
  SDL_DestroyMutex(quit_listeners_lock);
  quit_listeners_lock = NULL;
  SDL_DestroyMutex(keydown_listeners_lock);
  keydown_listeners_lock = NULL;
  SDL_DestroyMutex(keyup_listeners_lock);
  keyup_listeners_lock = NULL;

  for(unsigned int i = 0 ; i < quit_listeners.size ; i ++) {
    free(quit_listeners.elements[i]);
  }
  vector_clear(&quit_listeners);

  for(unsigned int i = 0 ; i < keydown_listeners.size ; i ++) {
    free(keydown_listeners.elements[i]);
  }
  vector_clear(&keydown_listeners);

  for(unsigned int i = 0 ; i < keyup_listeners.size ; i ++) {
    free(keyup_listeners.elements[i]);
  }
  vector_clear(&keyup_listeners);
}

int input_handle_sdl_event(const SDL_Event * event) {
  if(event->type == SDL_QUIT) {
    input_emit_quit();
    return 1;
  } else if(event->type == SDL_KEYDOWN) {
    unsigned int ikey = 0;
    if(get_key(&ikey, event->key.keysym.scancode)) {
      input_emit_keydown(ikey);
    }
    return 1;
  } else if(event->type == SDL_KEYUP) {
    unsigned int ikey = 0;
    if(get_key(&ikey, event->key.keysym.scancode)) {
      input_emit_keyup(ikey);
    }
    return 1;
  }

  return 0;
}

