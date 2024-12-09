#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<stdint.h>

typedef int (* compare_funcptr)(void * firstElem, void * secondElem);
void swap(char** a, char** b) {
	char* x = *a;
	*a = *b;
	*b = x;
}

int compareLinesFromAToZ( void * firstPtr, void * secondPtr )
{
    //printf("%p, %p\n", firstPtr, secondPtr);
    const char * firstLine  = ((const char *) (firstPtr));
    const char * secondLine = ((const char *) (secondPtr));
	//printf("%s\n%s\n", firstLine, secondLine);

    int currentSymbol = 0;
    for( ; firstLine[currentSymbol] == secondLine[currentSymbol] && firstLine[currentSymbol] != '\0'; currentSymbol++ );

    return firstLine[currentSymbol] - secondLine[currentSymbol];
}

size_t randomizator(size_t min, size_t max)
{
    size_t delta = (max <= min + 1) ? 0 : (2 * max + 3 * min + 0xDEC0DED ^ (max + min)) % (max - min);
    return min + delta;
}


int myqsort(char** array, size_t start, size_t end) {
	size_t pivot = start;
    char * copyOfPivot = array[pivot];
	size_t last = end-1;
	size_t i = pivot+1;
	if (end <= start+1) return 0;
	while (i <= last) {
		if (strcmp(array[i], array[pivot]) < 0 ) 
			swap(&array[i++], &array[pivot++]);
		else 
			swap(&array[i], &array[last--]);
	}
	myqsort(array, start, pivot);
	myqsort(array, pivot+1, end);
	// merge is done automatically
}

void randomQsort(char **arr, size_t start, size_t end) {
    if (start >= end) return; 

    size_t pivotIndex = randomizator(start, end);
    char *pivot = arr[pivotIndex];
    swap(&arr[pivotIndex], &arr[end]);

    size_t partitionIndex = start;
    for (size_t i = start; i < end; i++) {
        if (strcmp(arr[i], pivot) < 0) {
            swap(&arr[i], &arr[partitionIndex]);
            partitionIndex++;
        }
    }

    swap(&arr[partitionIndex], &arr[end]);

    if (partitionIndex > 0) randomQsort(arr, start, partitionIndex - 1);
    randomQsort(arr, partitionIndex + 1, end);
}



void changePointers( size_t numOfFirstElem, size_t numOfSecondElem, void ** pointer )
{
    void * temp = pointer[numOfFirstElem];

    pointer[numOfFirstElem] = pointer[numOfSecondElem];
    pointer[numOfSecondElem] = temp;

}

size_t distribution( void ** data, size_t low, size_t high, size_t pivot, compare_funcptr comparing);

void quickSort( void ** data, size_t low, size_t high, compare_funcptr comparing )
{
    if ( (high - low) < 1 ) {
        return;
    }
    else {

        size_t newLine = distribution( data, low, high, (high+low)/2, comparing);

        quickSort( data, low,     newLine-1, comparing );
        quickSort( data, newLine, high,      comparing );
    }
}

size_t distribution( void ** data, size_t low, size_t high, size_t pivot, compare_funcptr comparing)
{
    int part = -1;
    void * copyOfPivot = data[pivot];
    //printf("pivot %s", copyOfPivot);

    if ( (high - low) == 1) {
        if ( comparing( *(data + high), *(data + low) ) < 0) {
            changePointers( low, high, data);
        }
        return high;
    }

    while ( low < high ) {
        switch ( part ) {
            case -1:
                if ( comparing( copyOfPivot, *(data + low) ) <= 0) {
                    part = 1;
                }
                else {
                    low += 1;
                }
                break;
            case 1:
                if ( comparing( *(data + high), copyOfPivot ) <= 0) {
                    changePointers( low, high, data);
                    low  += 1;
                    part = -1;
                }
                else {
                    high -= 1;
                }
                break;
            default:
                break;
        }
    }
    return low;
}

#define SEC_TO_US(sec) ((sec)*1000000)
#define NS_TO_US(ns)   ((ns)/1000)

uint64_t micros()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    uint64_t us = SEC_TO_US((uint64_t)ts.tv_sec) + NS_TO_US((uint64_t)ts.tv_nsec);
    return us;
}

int main(int argc, char* argv[]) {
	char buf[4096];
	size_t cap = 4096;
	size_t sz = 0;
	char **array = malloc(cap * sizeof(char*));
	while (fgets(buf, sizeof(buf), stdin)) {
		if ( sz >= cap ) {
			array = realloc(array, sizeof(char*)*(cap*=2));
			if (!array) return -fprintf(stderr, "Cannot reallocate memory of %lu\n", cap);
		}
		
		array[sz++] = (strdup(buf));
	}
    uint64_t sortTime = micros();
	quickSort((void **)array, 0, sz - 1, compareLinesFromAToZ);
    printf("%zu %lu\n", sz, micros() - sortTime);
    /*for (int i = 0; i < sz - 1; i++) {
        if (strcmp(array[i], array[i+1]) > 0) printf("ALERTTTTTTTTTTT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        printf("%s delta %d\n", array[i], strcmp(array[i], array[i+1]));
    }*/
	free(array);
}
