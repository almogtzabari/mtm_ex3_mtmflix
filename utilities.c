#include "utilities.h"


//-----------------------------------------------------------------------//
//                       SERIES SET FUNCTIONS                            //
//-----------------------------------------------------------------------//


SetElement seriesCopySetElement(SetElement element){
    return seriesCopy((Series)element);
}

int seriesCompareSetElements(SetElement element1, SetElement element2){
    return seriesCompare((Series)element1,(Series)element2);
}

void seriesDestroySetElement(SetElement element){
    seriesDestroy((Series)element);
}

//-----------------------------------------------------------------------//
//                       SERIES LIST FUNCTIONS                           //
//-----------------------------------------------------------------------//


ListElement copyFavoriteSeriesName(ListElement element){
    return seriesCopyName((char*)element);
}

void destroyFavoriteSeriesName(ListElement element){
    seriesDestroyName((char*)element);
}

//-----------------------------------------------------------------------//
//                       USERS SET FUNCTIONS                             //
//-----------------------------------------------------------------------//

SetElement userCopySetElement(SetElement element1){
    return userCopy((User)element1);
}

void userDestroySetElememnt (SetElement element1){
    userDestroy((User)element1);
}

int userCompareSetElements(SetElement element1, SetElement element2){
    return userCompare((User)element1,(User)element2);
}

//-----------------------------------------------------------------------//
//                     Ranked series SET FUNCTIONS                       //
//-----------------------------------------------------------------------//

SetElement rankedSeriesCopySetElement(SetElement element1){
    return rankedSeriesCopy((RankedSeries)element1);
}

void rankedSeriesDestroySetElement (SetElement element1){
     rankedSeriesDestroy((RankedSeries)element1);
}

int rankedSeriesCompareSetElement(SetElement element1,
                                  SetElement element2){
    return rankedSeriesCompare((RankedSeries)element1,
                               (RankedSeries)element2);
}

//-----------------------------------------------------------------------//
//                       USERS LIST FUNCTIONS                            //
//-----------------------------------------------------------------------//

ListElement copyFriendUsername(ListElement element){
    return usernameCopy((char*)element);
}

void destroyFriendUsername(ListElement element){
    destroyUsername((char*)element);
}

//-----------------------------------------------------------------------//
//                       GENERIC FUNCTIONS                               //
//-----------------------------------------------------------------------//

int genericStrcmp (ListElement element1,ListElement element2){
    return strcmp((char*)element1,(char*)element2);
}