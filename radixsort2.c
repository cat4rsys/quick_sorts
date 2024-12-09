#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _list {
	char* str;
	struct _list* next;
} list;

typedef struct _slice {
	list* head;
	list** tail;
} slice;

void slice_init(slice* s) {
	s->head = NULL;
	s->tail = &s->head;
}

void slice_append(slice* s, list *l) {
	*s->tail = l;
	s->tail = &l->next;
}

void slice_concat(slice* s, slice* a) {
	*s->tail = a->head;
	s->tail = a->tail;
}

void slice_cut(slice* s) {
	*s->tail = NULL;
}

int radixsort(slice* s, int radix, int maxradix) {
	if ( radix >= maxradix ) return 0;
	if ( !s->head ) return 0;
	slice map[256];
	slice nulls;

	// initialization of tails
	for(int i = 0; i < 256; i++ ) slice_init(&map[i]);
	slice_init(&nulls);

	for (list* i = s->head; i; i = i->next) {
		if ( strlen(i->str) > radix)
			slice_append(&map[(unsigned char)i->str[radix]], i);
		else slice_append(&nulls, i);	
	}
	// tail-cut
	for(int i=0; i<256; i++) if (map[i].head) slice_cut(&map[i]);
	slice_cut(&nulls);
	
	slice_init(s);

	if ( nulls.head ) slice_concat(s, &nulls);
	for(int i=0; i<256; i++) {
		if (!map[i].head) continue;
		radixsort(&map[i], radix+1, maxradix);
		slice_concat(s, &map[i]);
	}
	return 0;
}


void free_list(list* lst) {
	if ( !lst ) return;
	list* next = lst->next;
	free(lst->str);
	free(lst);
	free_list(next);
}

int main(int argc, char* argv[]) {
	char buf[4096];
	slice s;
	slice_init(&s);
	while (fgets(buf, sizeof(buf), stdin)) {
		list* n = malloc(sizeof(list));
		*n = (list){strdup(buf), NULL};
		slice_append(&s, n);
	}
	fprintf(stderr, "Sorting started...\n");
	radixsort(&s, 0, 6);
	for( list* i = s.head; i; i=i->next )
		fputs(i->str, stdout);
	
	free_list(s.head);
}

