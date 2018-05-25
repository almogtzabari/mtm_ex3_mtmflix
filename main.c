#include "mtmflix.h"
#include "set.h"
#include "utilities.h"

int main(){
    MtmFlix flix=mtmFlixCreate();
    mtmFlixAddUser(flix,"Vered",57);
    mtmFlixAddUser(flix,"Ariel",24);
    mtmFlixAddUser(flix,"Ligad",27);
    mtmFlixAddUser(flix,"Orian",21);
    mtmFlixAddUser(flix,"Efraim",60);

    int ages [2]= {29,100};
    mtmFlixAddSeries(flix,"Stranger",4,DRAMA,ages,4);
    mtmFlixAddSeries(flix,"GameOfThrones",4,MYSTERY,NULL,7);
    mtmFlixAddSeries(flix,"Fauda",4,MYSTERY,NULL,4);
    mtmFlixAddSeries(flix,"Kabab",4,DRAMA,NULL,4);
    mtmFlixAddSeries(flix,"Shawarma",4,DRAMA,NULL,4);
    mtmFlixAddSeries(flix,"Shipud",4,DRAMA,NULL,4);

    mtmFlixSeriesJoin(flix,"Vered","Stranger");
    mtmFlixSeriesJoin(flix,"Vered","GameOfThrones");

    mtmFlixSeriesJoin(flix,"Orian","Kabab");
    mtmFlixSeriesJoin(flix,"Orian","Shawarma");
    mtmFlixSeriesJoin(flix,"Orian","Shipud");

    mtmFlixSeriesJoin(flix,"Efraim","Kabab");
    mtmFlixSeriesJoin(flix,"Efraim","Shawarma");
    mtmFlixSeriesJoin(flix,"Efraim","Shipud");


    mtmFlixAddFriend(flix,"Vered","Ariel");
    mtmFlixAddFriend(flix,"Vered","Ligad");
    mtmFlixAddFriend(flix,"Vered","Orian");
    mtmFlixAddFriend(flix,"Vered","Efraim");
    FILE* temp=fopen("temp1.txt","w");
    mtmFlixGetRecommendations(flix,"Vered",0,temp);
    fclose(temp);
    return 0;
}