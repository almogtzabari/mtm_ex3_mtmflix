
#ifndef MTM_EX3_MTMFLIX_UTILITIES_H
#define MTM_EX3_MTMFLIX_UTILITIES_H

#include "map.h"
#include "set.h"
#include "series.h"
#include "user.h"
#include "mtmflix.h"
#include "mtm_ex3.h"

//-----------------------------------------------------------------------//
//                       SERIES MAP FUNCTIONS                            //
//-----------------------------------------------------------------------//

int compareSeriesMapDataElements(MapDataElement element1,
                                 MapDataElement element2);

void freeSeriesMapKeyElements(MapKeyElement element);

void freeSeriesMapDataElements(MapDataElement element);

MapKeyElement copySeriesMapKeyElements(MapKeyElement element);

MapDataElement copySeriesMapDataElements(MapDataElement element);

MapDataElement seriesCreateMapDataElement(char* series_name,
                                          int number_of_episodes,
                                          Genre genre,
                                          int* ages,
                                          double episode_duration);

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
MapDataElement userCreateMapDataElement(char* username, int age);


//-----------------------------------------------------------------------//
//                       USERS SET FUNCTIONS                             //
//-----------------------------------------------------------------------//
void userDestroySetElement(SetElement element);
int userCompareSetElements(SetElement element1, SetElement element2);
SetElement userCopySetElement(SetElement element);



#endif //MTM_EX3_MTMFLIX_UTILITIES_H
