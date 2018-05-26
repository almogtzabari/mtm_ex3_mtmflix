
#ifndef MTM_EX3_MTMFLIX_SERIES_H
#define MTM_EX3_MTMFLIX_SERIES_H

#include "map.h"
#include <malloc.h>
#include <string.h>
#include "set.h"
#include "mtm_ex3.h"
#include <assert.h>

#define NUMBER_OF_GENRES 8
#define SERIES_NO_AGE_RESTRICTIONS -1

typedef struct series_t* Series;

typedef enum SeriesResult_t {
    SERIES_MEMORY_ALLOCATION_FAILED,
    SERIES_SUCCESS,
    SERIES_NULL_ARGUMENT,
    SERIES_ALREADY_EXISTS
} SeriesResult;


Series seriesCopy(Series series);

char* seriesCopyName(char *name);

void seriesDestroy(Series series);

int seriesCompare(Series series1, Series series2);

Series seriesCreate(char* series_name, int number_of_episodes,
                    Genre genre, int* ages, int episode_duration);

void seriesDestroyName(char* name);

Genre seriesGetGenre (Series series);

SeriesResult printSeriesDetailsToFile(Series series,
                                      FILE* outputStream);
char* getGenreNameByEnum(Genre genre);
bool seriesHasAgeRestrictions(Series series);
int seriesGetMaxAge(Series series);
int seriesGetMinAge(Series series);
char* seriesGetName (Series series);
Genre seriesGetGenreByName(char* series_name, Set series_set,
                           SeriesResult* status);
int seriesGetDurationByName(char *series_name, Set series_set,
                            SeriesResult *status);
int seriesGetEpisodeDuration (Series series);

#endif //MTM_EX3_MTMFLIX_SERIES_H

