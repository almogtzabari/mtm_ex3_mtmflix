
#ifndef MTM_EX3_MTMFLIX_UTILITIES_H
#define MTM_EX3_MTMFLIX_UTILITIES_H

#include "map.h"
#include "set.h"
#include "series.h"
#include "user.h"

//-----------------------------------------------------------------------//
//                       SERIES MAP FUNCTIONS                            //
//-----------------------------------------------------------------------//

int compareSeriesMapDataElements(MapDataElement element1,
                                 MapDataElement element2);

void freeSeriesMapKeyElements(MapKeyElement element);

void freeSeriesMapDataElements(MapDataElement element);

MapKeyElement copySeriesMapKeyElements(MapKeyElement element);

MapDataElement copySeriesMapDataElements(MapDataElement element);

//-----------------------------------------------------------------------//
//                       SERIES SET FUNCTIONS                            //
//-----------------------------------------------------------------------//

int compareSeriesSetElements(SetElement element1, SetElement element2);

SetElement copySeriesSetElements(SetElement element);

void freeSeriesSetElements(SetElement element);

//-----------------------------------------------------------------------//
//                       USERS MAP FUNCTIONS                             //
//-----------------------------------------------------------------------//

MapDataElement copyUserMapDataElements(MapDataElement element);
MapKeyElement copyUserMapKeyElements(MapKeyElement element);
void freeUserMapDataElements(MapDataElement element);
void freeUserMapKeyElements(MapKeyElement element);


//-----------------------------------------------------------------------//
//                       USERS SET FUNCTIONS                             //
//-----------------------------------------------------------------------//
void userDestroySetElement(SetElement element);
int userCompareSetElements(SetElement element1, SetElement element2);
SetElement userCopySetElement(SetElement element);



#endif //MTM_EX3_MTMFLIX_UTILITIES_H
