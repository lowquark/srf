#ifndef SCRIPT_PRIVATE_H
#define SCRIPT_PRIVATE_H

/*
#define SCRIPT_STOP 0
#define SCRIPT_CALL 1

#define SCRIPT_CALL_MAX_ARGS 4

#define SCRIPT_ARG_NIL      0
#define SCRIPT_ARG_INT      1
#define SCRIPT_ARG_DOUBLE   2
#define SCRIPT_ARG_BOOL     3
#define SCRIPT_ARG_USERDATA 4

typedef struct script_arg_int {
  unsigned int type;
  int value;
} script_arg_int_t;
typedef struct script_arg_double {
  unsigned int type;
  double value;
} script_arg_double_t;
typedef struct script_arg_bool {
  unsigned int type;
  int value;
} script_arg_bool_t;
typedef struct script_arg_userdata {
  unsigned int type;
  void * value;
  const char * metatype;
} script_arg_userdata_t;

// variant for different argument types
typedef union script_arg {
  unsigned int type;
  script_arg_int_t i;
  script_arg_double_t d;
  script_arg_bool_t b;
  script_arg_userdata_t u;
} script_arg_t;
*/

typedef void (* script_handler_t)(void * event);

// delayed calls for script task
typedef struct script_event {
  script_handler_t handler;
  void * data;
} script_event_t;

extern void script_emit(const script_event_t * event);

#endif
