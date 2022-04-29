/**
 * JSON grammar: From https://json.org/
 *
 * json    : value
 * 
 * value   : string | number | object | array | true | false | null
 * 
 * object  : {}
 *         | { members } 
 * 
 * members : pair
 *         | pair , members
 * 
 * pair    : string : value
 * 
 * array   : []
 *         | 
 * [ elements ]
 * elements: value
 *         | value , elements
 * 
 */

/**
 * The following two lines are what is called a #include guard.  It insures that
 * this header file can only be included once, even if included by multiple
 * header files.
 */
#ifndef _JSON_H
#define _JSON_H 1

// JSON value types
typedef enum {
  DT_STRING, DT_NUMBER, DT_OBJECT, DT_ARRAY, DT_TRUE, DT_FALSE, DT_NULL
} value_dt;

/**
 * These structures are merely a suggestion.  You may modify/delete/add to
 * them as you see fit.  You may not use any internet solution.  Any internet
 * sourced code will result in failing this assignment.  You will obviously
 * then need to change the print functions accordingly.
 */

typedef struct array array_t;
typedef struct pair pair_t;
typedef struct value value_t;

struct value {
  value_dt type;
  union {
    void *any;		// An "any" pointer
    char *s;		// A string
    double n;		// A number
    pair_t *o;		// An object
    array_t *a;		// An array
  };
};

struct array {
  long length;
  value_t *a;		// a may be NULL if length == 0.
};

// An object is a (linked) list of pairs:
struct pair {
  char *name;
  value_t v;
  pair_t *next;
};

#endif
