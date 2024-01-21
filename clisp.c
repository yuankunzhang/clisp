#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clisp.h"

static struct lisp_object *parse_object(const char **pinput);
static void free_object(struct lisp_object *obj);

/* public functions */

struct lisp_object *lisp_parse(const char *input) {
    return parse_object(&input);
}

void lisp_free(struct lisp_object *obj) { free_object(obj); }

struct lisp_object *car(struct lisp_list *obj) { return obj->car; }

struct lisp_list *cdr(struct lisp_list *obj) { return obj->cdr; }

/* local functions */

static char peek(const char **pinput) { return **pinput; }
static char next(const char **pinput) { return *(*pinput)++; }
static bool is_delimiter(char c) { return c == '(' || c == ')' || isspace(c); }

static void skip_whitespaces(const char **pinput) {
    while (isspace(**pinput)) {
        (*pinput)++;
    }
}

static struct lisp_atom *parse_atom(const char **pinput) {
    struct lisp_atom *atom;
    size_t len = 0;

    while ((*pinput)[len] != '\0' && !is_delimiter((*pinput)[len])) {
        len++;
    }

    atom = (struct lisp_atom *)malloc(sizeof(struct lisp_atom));
    if (atom == NULL) {
        return NULL;
    }

    atom->value = (char *)malloc(len + 1);
    if (atom->value == NULL) {
        free(atom);
        return NULL;
    }

    strncpy(atom->value, *pinput, len);
    atom->value[len] = '\0';
    *pinput += len;

    return atom;
}

static struct lisp_list *parse_list(const char **pinput) {
    struct lisp_list *list;

    list = (struct lisp_list *)malloc(sizeof(struct lisp_list));
    if (list == NULL) {
        return NULL;
    }

    list->car = NULL;
    list->cdr = NULL;

    skip_whitespaces(pinput);
    list->car = parse_object(pinput);
    if (list->car == NULL) {
        free(list);
        return NULL;
    }

    skip_whitespaces(pinput);
    if (peek(pinput) != ')') {
        list->cdr = parse_list(pinput);
        if (list->cdr == NULL) {
            free_object(list->car);
            free(list);
            return NULL;
        }
    }

    return list;
}

static struct lisp_object *parse_object(const char **pinput) {
    struct lisp_object *obj;

    skip_whitespaces(pinput);
    if (peek(pinput) == '\0') {
        return NULL;
    }

    obj = (struct lisp_object *)malloc(sizeof(struct lisp_object));
    if (obj == NULL) {
        return NULL;
    }

    if (peek(pinput) == ')') {
        free(obj);
        return NULL;
    } else if (peek(pinput) == '(') {
        next(pinput); // consume '('
        obj->type = LISP_LIST;
        obj->value.list = parse_list(pinput);
        if (obj->value.list == NULL) {
            free(obj);
            return NULL;
        }
        if (peek(pinput) != ')') {
            free(obj->value.list);
            free(obj);
            return NULL;
        }
        next(pinput); // consume ')'
    } else {
        obj->type = LISP_ATOM;
        obj->value.atom = parse_atom(pinput);
        if (obj->value.atom == NULL) {
            free(obj);
            return NULL;
        }
    }

    return obj;
}

static void free_atom(struct lisp_atom *atom) {
    free(atom->value);
    free(atom);
}

static void free_list(struct lisp_list *list) {
    if (list->cdr != NULL) {
        free_list(list->cdr);
    }
    if (list->car != NULL) {
        free_object(list->car);
    }
    free(list);
}

static void free_object(struct lisp_object *obj) {
    if (obj->type == LISP_ATOM) {
        free_atom(obj->value.atom);
    } else if (obj->type == LISP_LIST) {
        free_list(obj->value.list);
    }
    free(obj);
}
