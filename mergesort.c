#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _list {
	char* str;
	struct _list* next;
} list;


void free_list(list* lst) {
	if ( !lst ) return;
	list* next = lst->next;
	free(lst->str);
	free(lst);
	free_list(next);
}

int _split(list* lst, list** a, list** b) {
	if( !lst ) {*a = NULL; *b = NULL; return 0;}
	*a = lst;
	return _split(lst->next, b, &lst->next);
}

int _merge(list* a, list* b, list** result) {
	if ( !a )  *result = b;  
	else if ( !b ) *result = a; 
	else if ( strcmp(a->str, b->str) < 0 ) {
		*result = a;
		return _merge(a->next, b, &a->next);
	} else {
		*result = b;
		return _merge(a, b->next, &b->next);
	}
}

int mergesort(list** result) {
	list* a = NULL, *b = NULL;
	if ( !*result || !(*result)->next ) return 0; 
	_split(*result, &a, &b);
	mergesort(&a); mergesort(&b);
	_merge(a, b, result);
	return 0;
}

int main(int argc, char* argv[]) {
	char buf[4096];
	list* lst = NULL;
	while (fgets(buf, sizeof(buf), stdin)) {
		list* n = malloc(sizeof(list));
		*n = (list){strdup(buf), lst};
		lst = n;
	}
	fprintf(stderr, "Sorting started...\n");
	mergesort(&lst);
	for( list* i = lst; i; i=i->next )
		fputs(i->str, stdout);
	
	free_list(lst);
}

