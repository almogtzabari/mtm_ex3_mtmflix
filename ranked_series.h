#ifndef MTM_EX3_MTMFLIX_RANKED_SERIES_H
#define MTM_EX3_MTMFLIX_RANKED_SERIES_H

#include <assert.h>

typedef struct ranked_series_t* RankedSeries;

RankedSeries rankedSeriesCreate (int rank,char* series_name,
                                 char* series_genre);

RankedSeries rankedSeriesCopy (RankedSeries ranked_series);

void rankedSeriesDestroy (RankedSeries ranked_series);

int rankedSeriesCompare (RankedSeries ranked_series1,
                         RankedSeries ranked_series2);
#endif //MTM_EX3_MTMFLIX_RANKED_SERIES_H
