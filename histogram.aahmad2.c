#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define N 1000000
#define NUM_THREADS 4
#define MAXVAL 256

//Make a histogram, set all the vals to 0.
int values[MAXVAL];
int histogram[MAXVAL];

typedef struct {
    int startIndex; // the first index in my region
    int endIndex; // the last index in my region
    int *histogram; // the global histogram pthread_mutex_t *mutex; // a mutex, for synchronization
} ThreadInfo;

int arr [N];







void tallyGood(void *param){
    //int hist[N];

    ThreadInfo *T = (ThreadInfo *) param;
    for (int i=0; i<N; ++i)
        T->histogram[arr[i]] += 1;
}
int main() {

    pthread_t tids[N];


    //TODO: FIX THIS
    for (int i = 1; i<N ;i++){
        double r = drand48();
        arr[i] = (r * MAXVAL/i);
    }

    pthread_mutex_t mutex;
    pthread_t tid1, tid2;

    pthread_mutex_init(&mutex, NULL);

    struct timeval t1;

    ThreadInfo thread[NUM_THREADS];

    thread[0].startIndex = 0;
    thread[0].endIndex = N/4;
    thread[0].histogram = histogram;

    thread[1].startIndex = N/4 + 1;
    thread[1].endIndex =  N/2;
    thread[1].histogram = histogram;

    thread[2].startIndex = N/2 + 1;
    thread[2].endIndex = (N/4 + N/2);
    thread[2].histogram = histogram;

    thread[3].startIndex = (N/4 + N/2) +  1;
    thread[3].endIndex = N-1;
    thread[3].histogram = histogram;

    for (int j=0; j<MAXVAL; ++j)
        histogram[j] = 0;

    gettimeofday(&t1, NULL);
    for (int k=0; k<NUM_THREADS; ++k)
        pthread_create(&tids[k], NULL, tallyGood, &thread[k]);

    for (int k=0; k<NUM_THREADS; ++k)
        pthread_join(tids[k], NULL);

    struct timeval t2;
    gettimeofday(&t2, NULL);


    int x;
    return 0;


}
