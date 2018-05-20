#include "mtmflix.h"
#include "map.h"
#include "series.h"
#include "utilities.h"



struct mtmFlix_t{
    Map users;
    Map series;

};

MtmFlix mtmFlixCreate(){
    MtmFlix flix = malloc(sizeof(*flix));
    if(!flix){
        /* Failed to allocate memory. */
        return NULL;
    }
    flix->series = mapCreate(copySeriesMapDataElements,
            copySeriesMapKeyElements,freeSeriesMapDataElements,
                             freeSeriesMapKeyElements,compareSeriesMapDataElements);
    if(!flix->series){
        /* Failed to allocate memory for the series map. */
        free(flix);
        return NULL;
    }
    flix->users = NULL;
    return flix;

}


