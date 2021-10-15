//
// Created by DedSheeran on 10/9/21.
//

#ifndef HISTOGRAM_AAHMAD2_C_HISTOGRAM_AAHMAD2_H
#define HISTOGRAM_AAHMAD2_C_HISTOGRAM_AAHMAD2_H

#define N 1000000
#define NUM_THREADS 4
#define MAXVAL 256

typedef struct {
    int startIndex; // the first index in my region
    int endIndex; // the last index in my region
    int *histogram; // the global histogram pthread_mutex_t *mutex; // a mutex, for synchronization
} ThreadInfo;



#endif //HISTOGRAM_AAHMAD2_C_HISTOGRAM_AAHMAD2_H
