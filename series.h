//
// Created by Almog on 19/05/2018.
//

#ifndef MTM_EX3_MTMFLIX_SERIES_H
#define MTM_EX3_MTMFLIX_SERIES_H

typedef struct serie_t* Series;
typedef void* SeriesKeyElement;
typedef void* SeriesDataElement;

SeriesDataElement copySeriesDataElements(SeriesDataElement);
SeriesKeyElement copySeriesKeyElements(SeriesKeyElement);
void freeSeriesDataElements(SeriesDataElement);
void freeSeriesKeyElements(SeriesKeyElement);
int compareSeriesDataElements(SeriesDataElement,SeriesDataElement);

#endif //MTM_EX3_MTMFLIX_SERIES_H

