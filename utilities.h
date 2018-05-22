
#ifndef MTM_EX3_MTMFLIX_UTILITIES_H
#define MTM_EX3_MTMFLIX_UTILITIES_H

#include "map.h"
#include "set.h"
#include "series.h"
#include "user.h"
#include "list.h"
#include "mtmflix.h"
#include "mtm_ex3.h"


//-----------------------------------------------------------------------//
//                       SERIES SET FUNCTIONS                            //
//-----------------------------------------------------------------------//

SetElement seriesCopySetElement(SetElement element);
void seriesDestroySetElement(SetElement element);
int seriesCompareSetElements(SetElement element1, SetElement element2);

//-----------------------------------------------------------------------//
//                       SERIES LIST FUNCTIONS                           //
//-----------------------------------------------------------------------//

ListElement copyFavoriteSeriesName(ListElement element);
void destroyFavoriteSeriesName(ListElement element);

//-----------------------------------------------------------------------//
//                       USERS SET FUNCTIONS                             //
//-----------------------------------------------------------------------//

SetElement userCopySetElement(SetElement element1);
void userDestroySetElememnt(SetElement element1);
int userCompareSetElements(SetElement element1, SetElement element2);


//-----------------------------------------------------------------------//
//                       USERS LIST FUNCTIONS                            //
//-----------------------------------------------------------------------//

ListElement copyFriendUsername (ListElement element);
void destroyFriendUsername(ListElement element);


#endif //MTM_EX3_MTMFLIX_UTILITIES_H
