
#ifndef MTM_EX3_MTMFLIX_SERIES_H
#define MTM_EX3_MTMFLIX_SERIES_H

#include "map.h"
#include <malloc.h>
#include <string.h>
#include "set.h"

typedef struct series_t* Series;

typedef enum SeriesResult_t {
    SERIES_MEMORY_ALLOCATION_FAILED,
    SERIES_SUCCESS,
    SERIES_NULL_ARGUMENT
} SeriesResult;

MapDataElement copySeriesMapDataElements(MapDataElement element);
Series copySeries(Series series);
MapKeyElement copySeriesMapKeyElements(MapKeyElement element);
char* copySeriesName(char* name);
void freeSeriesMapDataElements(MapDataElement element);
void seriesDestroy(Series series);

void freeSeriesMapKeyElements(MapKeyElement element);

int compareSeriesMapDataElements(MapDataElement element1,
                                 MapDataElement element2);

SetElement copySeriesSetElements(SetElement element);




#endif //MTM_EX3_MTMFLIX_SERIES_H

