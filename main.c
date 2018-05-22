#include "mtmflix.h"
int main(){
    MtmFlix flix=mtmFlixCreate();
    mtmFlixAddSeries(flix,"bulbul",4,HORROR,NULL,4);
    mtmFlixAddSeries(flix,"bulbul1",4,DRAMA,NULL,4);
    mtmFlixAddSeries(flix,"bulbul2",4,HORROR,NULL,4);
    mtmFlixAddSeries(flix,"bulbul3",4,DOCUMENTARY,NULL,4);
    printf("gfsdgsd");
    return 0;
}