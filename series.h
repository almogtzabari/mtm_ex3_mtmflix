
#ifndef MTM_EX3_MTMFLIX_SERIES_H
#define MTM_EX3_MTMFLIX_SERIES_H

#include "map.h"
#include <malloc.h>
#include <string.h>
#include "set.h"
#include "mtm_ex3.h"

typedef struct series_t* Series;

typedef enum SeriesResult_t {
    SERIES_MEMORY_ALLOCATION_FAILED,
    SERIES_SUCCESS,
    SERIES_NULL_ARGUMENT
} SeriesResult;


Series copySeries(Series series);

char* copySeriesName(char* name);

void seriesDestroy(Series series);

int compareSeries(Series series1, Series series2);

Series seriesCreate(const char* series_name, int number_of_episodes,
                    Genre genre, int* ages, double episode_duration);



#endif //MTM_EX3_MTMFLIX_SERIES_H

