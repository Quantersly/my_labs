#ifndef DIAL_H
#define DIAL_H

#include "table.h"


void dial_exit();
Err_table dial_create(Table**);
Err_table dial_insert(Table*);
Err_table dial_delete(Table*);
Err_table dial_search(const Table*, Table**);
Err_table dial_import(Table**);
Err_table dial_search_parent(const Table*, Table**);

#endif
