#include "utilities.h"

//-----------------------------------------------------------------------//
//                       SERIES MAP FUNCTIONS                            //
//-----------------------------------------------------------------------//

/**
 ***** Function: copySeriesMapDataElements *****
 * Description: Creates a copy
 * @param element
 * @return
 */
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

//-----------------------------------------------------------------------//
//                       USERS MAP FUNCTIONS                             //
//-----------------------------------------------------------------------//

MapDataElement copyUserMapDataElements(MapDataElement element) {
    return (MapDataElement)userCopy((User) element);
}

MapKeyElement copyUserMapKeyElements(MapKeyElement element){
    return (MapKeyElement)usernameCopy((char*)element);
}

void freeUserMapDataElements(MapDataElement element){
    userDestroy(((User)element));
}

void freeUserMapKeyElements(MapKeyElement element){
    free((char*)element);
}

int compareUserMapDataElements(MapDataElement element1,
                                 MapDataElement element2){

}


//-----------------------------------------------------------------------//
//                       USERS SET FUNCTIONS                             //
//-----------------------------------------------------------------------//

void userDestroySetElement(SetElement element){
    return (userDestroy((User)element));
}

SetElement userCopySetElement(SetElement element){
    return userCopy((User)element);
}

/**
 ***** Function: userCompare *****
 * Description: casts void* to User and calls user compare function.
 * The casting is done in order to use the set functions.
 * @param element1 -
 * @param element2
 * @return
 */
int userCompareSetElements(SetElement element1, SetElement element2){
    return (userCompare((User)element1,(User)element2));
}