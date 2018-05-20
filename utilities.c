#include "utilities.h"

//-----------------------------------------------------------------------//
//                       SERIES MAP FUNCTIONS                            //
//-----------------------------------------------------------------------//

MapDataElement copySeriesMapDataElements(MapDataElement element) {
    return (MapDataElement)copySeries((Series) element);
}

MapKeyElement copySeriesMapKeyElements(MapKeyElement element){
    return (MapKeyElement)copySeriesName((char*)element);
}

void freeSeriesMapDataElements(MapDataElement element){
    seriesDestroy((Series)element);
}

void freeSeriesMapKeyElements(MapKeyElement element){
    free((char*)element);
}

int compareSeriesMapDataElements(MapDataElement element1,
                                 MapDataElement element2){

}

//-----------------------------------------------------------------------//
//                       SERIES SET FUNCTIONS                            //
//-----------------------------------------------------------------------//

void freeSeriesSetElements(SetElement element){
    seriesDestroy((Series)element);
}

int compareSeriesSetElements(SetElement element1, SetElement element2){
    return compareSeries((Series)element1,(Series)element2);
}

SetElement copySeriesSetElements(SetElement element){
    return (SetElement)copySeries((Series)element);
}