#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL_events.h>

#define INPUT_KEY_ESCAPE 0x01

#define INPUT_KEY_A   'a'
#define INPUT_KEY_B   'b'
#define INPUT_KEY_C   'c'
#define INPUT_KEY_D   'd'
#define INPUT_KEY_E   'e'
#define INPUT_KEY_F   'f'
#define INPUT_KEY_G   'g'
#define INPUT_KEY_H   'h'
#define INPUT_KEY_I   'i'
#define INPUT_KEY_J   'j'
#define INPUT_KEY_K   'k'
#define INPUT_KEY_L   'l'
#define INPUT_KEY_M   'm'
#define INPUT_KEY_N   'n'
#define INPUT_KEY_O   'o'
#define INPUT_KEY_P   'p'
#define INPUT_KEY_Q   'q'
#define INPUT_KEY_R   'r'
#define INPUT_KEY_S   's'
#define INPUT_KEY_T   't'
#define INPUT_KEY_U   'u'
#define INPUT_KEY_V   'v'
#define INPUT_KEY_W   'w'
#define INPUT_KEY_X   'x'
#define INPUT_KEY_Y   'y'
#define INPUT_KEY_Z   'z'

#define INPUT_KEY_0   '0'
#define INPUT_KEY_1   '1'
#define INPUT_KEY_2   '2'
#define INPUT_KEY_3   '3'
#define INPUT_KEY_4   '4'
#define INPUT_KEY_5   '5'
#define INPUT_KEY_6   '6'
#define INPUT_KEY_7   '7'
#define INPUT_KEY_8   '8'
#define INPUT_KEY_9   '9'

typedef void (* input_QuitHandler)(void * d);
typedef void (* input_KeyHandler)(unsigned int key_id, void * d);

// public, thread-safe messaging api
void input_on_quit(input_QuitHandler h, void * d);
void input_on_keydown(input_KeyHandler h, void * d);
void input_on_keyup(input_KeyHandler h, void * d);

void input_init();
void input_deinit();

int input_handle_sdl_event(const SDL_Event * event);

#endif
