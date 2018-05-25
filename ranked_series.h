#ifndef MTM_EX3_MTMFLIX_RANKED_SERIES_H
#define MTM_EX3_MTMFLIX_RANKED_SERIES_H

typedef struct ranked_series_t* RankedSeries;

RankedSeries rankedSeriesCreate (int rank,char* series_name,
                                 char* series_genre);




#endif //MTM_EX3_MTMFLIX_RANKED_SERIES_H
