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
    return seriesCopyName((char *)element);
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
    return userDestroy((User)element1);
}

int userCompareSetElements(SetElement element1, SetElement element2){
    return userCompare((User)element1,(User)element2);
}


//-----------------------------------------------------------------------//
//                       USERS LIST FUNCTIONS                            //
//-----------------------------------------------------------------------//

ListElement copyFriendUsername(ListElement element){
    return usernameCopy((User)element);
}

void destroyFriendUsername(ListElement element){
    return destroyUsername((char*)element);
}