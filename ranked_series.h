#ifndef MTM_EX3_MTMFLIX_RANKED_SERIES_H
#define MTM_EX3_MTMFLIX_RANKED_SERIES_H

#include <assert.h>
#include "set.h"
#include "mtmflix.h"

typedef struct ranked_series_t* RankedSeries;

RankedSeries rankedSeriesCreate (int rank,const char* series_name,
                                 const char* series_genre);

RankedSeries rankedSeriesCopy (RankedSeries ranked_series);

void rankedSeriesDestroy (RankedSeries ranked_series);

int rankedSeriesCompare (RankedSeries ranked_series1,
                         RankedSeries ranked_series2);

void rankedSeriesPrintToFile(int number_of_series_to_print,
                             Set ranked_series_set,
                             FILE* outputStream,MtmFlixResult* result);
#endif //MTM_EX3_MTMFLIX_RANKED_SERIES_H
