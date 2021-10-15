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
int histogramBad[MAXVAL];

typedef struct {
    int startIndex; // the first index in my region
    int endIndex; // the last index in my region
    int *histogram; // the global histogram pthread_mutex_t *mutex; // a mutex, for synchronization
} ThreadInfo;

int arr [N];


// Tallygood function is fine, histogram is working as it should
// problem is with double r = drand48();
//arr[i] = r*maxval/2
// basically not producing a uniform distribution
// not exceeding 255, but also must be spread out over the span of a million elements
//

// Added tallySerial, this is used to compare with tallyGood, will need for tallyBad as well
int tallySerial(int *histogram) {
    int j;
    for (j = 0; j < MAXVAL; ++j) {
        histogram[j] = 0;
    }
    for (int i = 0; i < N; ++i) {
        histogram[arr[i]] += 1;
    }
}





void tallyGood(void *param){
    int hist[MAXVAL];
    pthread_mutex_t *mutex;
    mutex = (pthread_mutex_t *) param;
    ThreadInfo *T = (ThreadInfo *) param;
    int *globHist = T->histogram;
    // have each thread look between each part it is supposed to look @, then set in the hist arr
    // at the end, use mutex to stitch together each count from all *T
    // pthread mutex lock

    for (int i= 0; i < MAXVAL; ++i) {
        hist[i] = 0;
    }
    for (int i=T->startIndex; i<=T->endIndex; ++i)
        hist[arr[i]] += 1;

    // lock mutex


    // iterate through hist
    pthread_mutex_lock(mutex);
    for (int j = 0; j < MAXVAL; ++j)
        globHist[j] += hist[j];
    pthread_mutex_unlock(mutex);

    pthread_exit(0);
}


void *tallyBad(void *param) {
    pthread_mutex_t *mutex;
    mutex = (pthread_mutex_t *) param;
    ThreadInfo *T = (ThreadInfo *) param;
    // this is the code inside each thread
    // each thread knows its own start and end values -- myStart and myEnd for (i=myStart; i<=myEnd; ++i) {
    for (int i=T->startIndex; i<=T->endIndex; ++i) {
        pthread_mutex_lock(mutex);
        histogram[arr[i]] += 1;
        pthread_mutex_unlock(mutex);
    }
}

int main() {

    pthread_t tids[N];


    // JASON SAID THIS WAS FINE, GOT RID OF THE DIVIDED BY I PORTION
    for (int i = 1; i<N ;i++){
        double r = drand48();
        arr[i] = r * MAXVAL;
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


    int newHistogram[MAXVAL];
    tallySerial(newHistogram);



    pthread_create(&tids[0], NULL, tallyGood, &thread[0]);
    pthread_create(&tids[1], NULL, tallyGood, &thread[1]);
    pthread_create(&tids[2], NULL, tallyGood, &thread[2]);
    pthread_create(&tids[3], NULL, tallyGood, &thread[3]);

    for (int k=0; k<NUM_THREADS; ++k)
        pthread_join(tids[k], NULL);



    for (int j=0; j<MAXVAL; ++j)
        histogramBad[j] = 0;

    thread[0].startIndex = 0;
    thread[0].endIndex = N/4;
    thread[0].histogram = histogramBad;

    thread[1].startIndex = N/4 + 1;
    thread[1].endIndex =  N/2;
    thread[1].histogram = histogramBad;

    thread[2].startIndex = N/2 + 1;
    thread[2].endIndex = (N/4 + N/2);
    thread[2].histogram = histogramBad;

    thread[3].startIndex = (N/4 + N/2) +  1;
    thread[3].endIndex = N-1;
    thread[3].histogram = histogramBad;



    pthread_create(&tids[0], NULL, tallyBad, &thread[0]);
    pthread_create(&tids[1], NULL, tallyBad, &thread[1]);
    pthread_create(&tids[2], NULL, tallyBad, &thread[2]);
    pthread_create(&tids[3], NULL, tallyBad, &thread[3]);

    for (int k=0; k<NUM_THREADS; ++k)
        pthread_join(tids[k], NULL);





    struct timeval t2;
    gettimeofday(&t2, NULL);

    // for each
    for (int i = 0; i < MAXVAL; i++) {
        if (newHistogram[i] != histogram[i]) {
            printf("no match");
        }
    }

    //int x;
    return 0;


}
