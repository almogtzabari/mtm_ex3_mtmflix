#include "utilities.h"


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