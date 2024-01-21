#include <stdio.h>

#include "clisp.h"

int main() {
    struct lisp_object *obj;

    obj = lisp_parse("(2 3 4)");
    if (obj == NULL) {
        printf("parse error\n");
        return 1;
    }

    printf("type l: %d\n", obj->type);
    printf("value (car l): %s\n", car(obj->value.list)->value.atom->value);
    printf("type (car (cdr l)): %d\n", car(cdr(obj->value.list))->type);
    printf("value (car (cdr l)): %s\n",
           car(cdr(obj->value.list))->value.atom->value);
    return 0;
}
