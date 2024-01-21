#ifndef CLISP_H_
#define CLISP_H_

struct lisp_atom;
struct lisp_list;
struct lisp_object;

struct lisp_atom {
    char *value;
};

struct lisp_list {
    struct lisp_object *car;
    struct lisp_list *cdr;
};

struct lisp_object {
    enum { LISP_ATOM, LISP_LIST } type;
    union {
        struct lisp_atom *atom;
        struct lisp_list *list;
    } value;
};

struct lisp_object *lisp_parse(const char *input);
void lisp_free(struct lisp_object *obj);
struct lisp_object *car(struct lisp_list *obj);
struct lisp_list *cdr(struct lisp_list *obj);

#endif // CLISP_H_
