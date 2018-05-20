#include "utilities.h"

//-----------------------------------------------------------------------//
//                       SERIES MAP FUNCTIONS                            //
//-----------------------------------------------------------------------//
//    DESCRIPTION: THESE FUNCTIONS ARE AUXILIARY FUNCTIONS FOR SERIES    //
//                 MAPS. THESE FUNCTIONS ARE DOING THE NEEDED CASTING    //
//                 BEFORE CALLING THE REAL SERIES FUNCTIONS.             //
//-----------------------------------------------------------------------//
/**
 ***** Function: copySeriesMapDataElements *****
 * Description: Gets a series map data element and returns a copy.
 *
 * @param element
 * @return
 */
MapDataElement copySeriesMapDataElements(MapDataElement element) {
    return (MapDataElement)copySeries((Series) element);
}

/**
 ***** Function: copySeriesMapKeyElements *****
 * Description: Gets a key of a series (name) and returns a copy of it.
 *
 * @param element - The key we want to copy.
 * @return - A copy of the given key. If fails returns NULL;
 */
MapKeyElement copySeriesMapKeyElements(MapKeyElement element){
    return (MapKeyElement)copySeriesName((char*)element);
}


/**
 ***** Function: freeSeriesMapDataElements *****
 * Description: Free all allocated memory of the given series.
 *
 * @param element - The data (series) we want to destroy.
 */
void freeSeriesMapDataElements(MapDataElement element){
    seriesDestroy((Series)element);
}

/**
 ***** Function: freeSeriesMapKeyElements *****
 * Description: Destroy the given key (series name).
 *
 * @param element - The key we want to destroy.
 */
void freeSeriesMapKeyElements(MapKeyElement element){
    free((char*)element);
}

/**
 ***** Function: compareSeriesMapDataElements *****
 * Description: Gets 2 data elements (series) and returns an integer
 * indicates which element is bigger.
 *
 * @param element1 - Element we want to compare.
 * @param element2 - Element we want to compare.
 * @return
 * Positive integer - If the first element is bigger.
 * Negative interger - If the second element is bigger.
 * Zero - If the two elements are equal.
 */
int compareSeriesMapDataElements(MapDataElement element1,
                                 MapDataElement element2){
    return compareSeries((Series)element1,(Series)element2);
}

/**
 ***** Function: seriesCreateMapDataElement *****
 * Description: Creates a series map data element (series).
 *
 * @param series_name - Name of data's element.
 * @param number_of_episodes - Number of episode of data's element.
 * @param genre - Genre of data's element.
 * @param ages - Age limitations of data's element.
 * @param episode_duration - Episode duration of data's element.
 * @return - New series map data element (series).
 */
MapDataElement seriesCreateMapDataElement(char* series_name,
                            int number_of_episodes, Genre genre,
                            int* ages, double episode_duration){
    return seriesCreate(series_name,number_of_episodes,genre,ages,
                        episode_duration);

}







//-----------------------------------------------------------------------//
//                       SERIES SET FUNCTIONS                            //
//-----------------------------------------------------------------------//
//    DESCRIPTION: THESE FUNCTIONS ARE AUXILIARY FUNCTIONS FOR SERIES    //
//                 SETS. THESE FUNCTIONS ARE DOING THE NEEDED CASTING    //
//                 BEFORE CALLING THE REAL SERIES FUNCTIONS.             //
//-----------------------------------------------------------------------//

/**
 ***** Function: freeSeriesSetElements *****
 * Description: Free all allocate memory of series set element (series).
 *
 * @param element - The element we want to destroy.
 */
void freeSeriesSetElements(SetElement element){
    seriesDestroy((Series)element);
}

/**
 ***** Function: compareSeriesSetElements *****
 * Description: Gets two series-set elements and compare them. The function
 * returns an integer that indicates which is bigger.
 *
 * @param element1 - Element we want to compare.
 * @param element2 - Element we want to compare.
 * @param element - The element we want to destroy.
 * @return
 * Positive integer - If the first element is bigger.
 * Negative interger - If the second element is bigger.
 * Zero - If the two elements are equal.
 */
int compareSeriesSetElements(SetElement element1, SetElement element2){
    return compareSeries((Series)element1,(Series)element2);
}

/**
 ****** Function: copySeriesSetElements *****
 * Description: Gets a series-set element (series) and returns a copy of
 * it.
 *
 * @param element - Element we want to copy.
 * @return - A copy of the given element.
 */
SetElement copySeriesSetElements(SetElement element){
    return (SetElement)copySeries((Series)element);
}





//-----------------------------------------------------------------------//
//                       USERS MAP FUNCTIONS                             //
//-----------------------------------------------------------------------//
//    DESCRIPTION: THESE FUNCTIONS ARE AUXILIARY FUNCTIONS FOR USERS     //
//                 MAPS. THESE FUNCTIONS ARE DOING THE NEEDED CASTING    //
//                 BEFORE CALLING THE REAL USERS FUNCTIONS.              //
//-----------------------------------------------------------------------//

MapDataElement userCreateMapDataElement(char* username, int age){
    return userCreate(username,age);
}

MapDataElement copyUserMapDataElements(MapDataElement element) {
    return userCopy((User) element);
}

MapKeyElement copyUserMapKeyElements(MapKeyElement element){
    return usernameCopy((char*)element);
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
//    DESCRIPTION: THESE FUNCTIONS ARE AUXILIARY FUNCTIONS FOR USERS     //
//                 SETS. THESE FUNCTIONS ARE DOING THE NEEDED CASTING    //
//                 BEFORE CALLING THE REAL USERS FUNCTIONS.              //
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