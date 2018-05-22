
#ifndef MTM_EX3_MTMFLIX_SERIES_H
#define MTM_EX3_MTMFLIX_SERIES_H

#include "map.h"
#include <malloc.h>
#include <string.h>
#include "set.h"
#include "mtm_ex3.h"
#include <assert.h>

#define NUMBER_OF_GENRES 8

typedef struct series_t* Series;

typedef enum SeriesResult_t {
    SERIES_MEMORY_ALLOCATION_FAILED,
    SERIES_SUCCESS,
    SERIES_NULL_ARGUMENT
} SeriesResult;


Series seriesCopy(Series series);

char* seriesCopyName(char *name);

void seriesDestroy(Series series);

int seriesCompare(Series series1, Series series2);

Series seriesCreate(const char* series_name, int number_of_episodes,
                    Genre genre, int* ages, double episode_duration);

void seriesDestroyName(char* name);

Genre seriesGetGenre (Series series);

SeriesResult printSeriesDetailsToFile(Series current_series,
                                      FILE* outputStream);

#endif //MTM_EX3_MTMFLIX_SERIES_H

