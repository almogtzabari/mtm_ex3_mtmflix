#include "mtmflix.h"
int main(){
    MtmFlix flix=mtmFlixCreate();
    mtmFlixAddUser(flix,"AVI",41);
    mtmFlixAddUser(flix,"AVI#",42);
    mtmFlixAddUser(flix,"avigdor",43);
    mtmFlixAddUser(flix,"bvigdor",44);
    mtmFlixAddUser(flix,"avigdor",45);
    mtmFlixAddSeries(flix,"Grudge",4,HORROR,NULL,4);
    mtmFlixAddSeries(flix,"Archie",4,HORROR,NULL,4);
    mtmFlixAddSeries(flix,"GameOfThrones",4,DRAMA,NULL,4);
    mtmFlixAddSeries(flix,"Zoe",4,HORROR,NULL,4);
    mtmFlixAddSeries(flix,"Zoz",4,HORROR,NULL,4);
    mtmFlixSeriesJoin(flix,"avigdor","Grudge");
    mtmFlixSeriesJoin(flix,"avigdor","GameOfThrones");
    mtmFlixSeriesJoin(flix,"avigdor","Zoe");
    mtmFlixSeriesJoin(flix,"avigdor","Zoz");
    mtmFlixSeriesJoin(flix,"avigdor","Archie");
    FILE* temp=fopen("temp1.txt","w");
    mtmFlixReportUsers(flix,temp);
    mtmFlixReportSeries(flix,0,temp);
    fclose(temp);
    return 0;
}