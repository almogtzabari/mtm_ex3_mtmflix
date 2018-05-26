
#ifndef MTM_EX3_MTMFLIX_UTILITIES_H
#define MTM_EX3_MTMFLIX_UTILITIES_H

#include "map.h"
#include "set.h"
#include "series.h"
#include "user.h"
#include "list.h"
#include "mtmflix.h"
#include "mtm_ex3.h"
#include "ranked_series.h"

//-----------------------------------------------------------------------//
//                            DESCIPTION                                 //
//                                                                       //
//   THE FOLLOWING FUNCTIONS ARE AUXILIARY FUNCTION THAT USES THE COPY,  //
//   CREATE AND DESTROY OF EACH ADT (SERIES,USERS). A FULL DESCRIPTION   //
//   OF EACH FUNCTION CAN BE FOUND IN EACH ADT FILES.                    //
//-----------------------------------------------------------------------//



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
//                       Ranked series SET FUNCTIONS                     //
//-----------------------------------------------------------------------//

SetElement rankedSeriesCopySetElement(SetElement element1);
void rankedSeriesDestroySetElement(SetElement element1);
int rankedSeriesCompareSetElement(SetElement element1, SetElement element2);


//-----------------------------------------------------------------------//
//                       USERS LIST FUNCTIONS                            //
//-----------------------------------------------------------------------//

ListElement copyFriendUsername (ListElement element);
void destroyFriendUsername(ListElement element);
//-----------------------------------------------------------------------//
//                       GENERIC FUNCTIONS                               //
//-----------------------------------------------------------------------//

int genericStrcmp (ListElement element1,ListElement element2);

#endif //MTM_EX3_MTMFLIX_UTILITIES_H
