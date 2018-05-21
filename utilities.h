
#ifndef MTM_EX3_MTMFLIX_UTILITIES_H
#define MTM_EX3_MTMFLIX_UTILITIES_H

#include "map.h"
#include "set.h"
#include "series.h"
#include "user.h"
#include "mtmflix.h"
#include "mtm_ex3.h"


//-----------------------------------------------------------------------//
//                       SERIES SET FUNCTIONS                            //
//-----------------------------------------------------------------------//

int compareSeriesSetElements(SetElement element1, SetElement element2);

SetElement copySeriesSetElements(SetElement element);

void freeSeriesSetElements(SetElement element);



//-----------------------------------------------------------------------//
//                       USERS SET FUNCTIONS                             //
//-----------------------------------------------------------------------//
void userDestroySetElement(SetElement element);
int userCompareSetElements(SetElement element1, SetElement element2);
SetElement userCopySetElement(SetElement element);



#endif //MTM_EX3_MTMFLIX_UTILITIES_H
