#include "mtmflix.h"
#include "map.h"
#include "series.h"


struct mtmFlix_t{
    Map users;
    Map series;


};

MtmFlix mtmFlixCreate(){
    Map users = mapCreate(copySeriesDataElements,copySeriesKeyElements,
            freeSeriesDataElements,freeSeriesKeyElements,compareSeriesDataElements);
}


