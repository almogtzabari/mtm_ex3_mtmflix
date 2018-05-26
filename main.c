#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "test_utilities.h"
#include "user.h"
#include "mtmflix.h"
#include "series.h"
#include "set.h"
#include "mtm_ex3.h"
#include "list.h"


int main() {
    MtmFlix m = mtmFlixCreate();

    mtmFlixAddUser(m, "Mathew", 20);
    mtmFlixAddUser(m, "Bill", 21);
    mtmFlixAddUser(m, "Alice", 21);
    mtmFlixAddUser(m, "Patrick", 20);
    mtmFlixAddUser(m, "Lisa", 15);

    mtmFlixAddFriend(m, "Mathew", "Lisa");
    mtmFlixAddFriend(m, "Mathew", "Mathew");
    mtmFlixAddFriend(m, "Mathew", "Bill");
    mtmFlixAddFriend(m, "Mathew", "Alice");
    
    int ages21[2] = {21, 50};
    int teens[2] = {15, 19};

    mtmFlixAddSeries(m, "Shameless", 1,COMEDY,NULL,40);
    mtmFlixAddSeries(m, "TwoAndaHalfMan", 1,COMEDY,NULL,40);
    mtmFlixAddSeries(m, "Futurama", 1,COMEDY,teens,40);
    mtmFlixAddSeries(m, "TheoryOfBigBang", 1,COMEDY,NULL,40);
    mtmFlixAddSeries(m, "RickAndMorty", 1,COMEDY,NULL,20);
    mtmFlixAddSeries(m, "Taxi1", 1,COMEDY,NULL,90);
    mtmFlixAddSeries(m, "Taxi2", 1,COMEDY,NULL,90);
    mtmFlixAddSeries(m, "Taxi3", 1,COMEDY,NULL,90);
    mtmFlixAddSeries(m, "WestWorld",1,SCIENCE_FICTION,NULL,40);
    mtmFlixAddSeries(m, "StarTREK",1,SCIENCE_FICTION,NULL,40);
    mtmFlixAddSeries(m, "XFiles",1,SCIENCE_FICTION,NULL,40);
    mtmFlixAddSeries(m, "DoctorWho",1,SCIENCE_FICTION,NULL,40);
    mtmFlixAddSeries(m, "BlackMirror",1,SCIENCE_FICTION,NULL,40);
    mtmFlixAddSeries(m, "StarWars1",1,SCIENCE_FICTION,NULL,100);
    mtmFlixAddSeries(m, "StarWars2",1,SCIENCE_FICTION,NULL,100);
    mtmFlixAddSeries(m, "StarWars3",1,SCIENCE_FICTION,NULL,100);
    mtmFlixAddSeries(m, "House",1,DRAMA,NULL,40);
    mtmFlixAddSeries(m, "13ReasonsWhy",1,DRAMA,NULL,40);
    mtmFlixAddSeries(m, "PrisonBreak",1,DRAMA,NULL,40);
    mtmFlixAddSeries(m, "GamesOfThrones",1,DRAMA,NULL,40);
    mtmFlixAddSeries(m, "BreakingBad",1,DRAMA,NULL,40);
    mtmFlixAddSeries(m, "TheGodFather1",1,DRAMA,NULL,180);
    mtmFlixAddSeries(m, "TheGodFather2",1,DRAMA,ages21,40);
    mtmFlixAddSeries(m, "TheGodFather3",1,DRAMA,NULL,180);
    mtmFlixAddSeries(m, "Snatched",1, CRIME,NULL,40);
    mtmFlixAddSeries(m, "AmericanPie",1,DRAMA,NULL,40);

    mtmFlixSeriesJoin(m, "Patrick", "AmericanPie");

    mtmFlixSeriesJoin(m, "Lisa", "Futurama");
    mtmFlixSeriesJoin(m, "Lisa", "TwoAndaHalfMan");

    mtmFlixSeriesJoin(m,"Mathew","Shameless");
    mtmFlixSeriesJoin(m,"Mathew","House");
    mtmFlixSeriesJoin(m,"Mathew","BreakingBad");
    mtmFlixSeriesJoin(m,"Mathew","WestWorld");
    mtmFlixSeriesJoin(m,"Mathew","DoctorWho");
    mtmFlixSeriesJoin(m,"Mathew","13ReasonsWhy");
    mtmFlixSeriesJoin(m,"Mathew","TheoryOfBigBang");

    mtmFlixSeriesJoin(m, "Bill", "TwoAndaHalfMan");
    mtmFlixSeriesJoin(m, "Bill", "Taxi1");
    mtmFlixSeriesJoin(m, "Bill", "Taxi2");
    mtmFlixSeriesJoin(m, "Bill", "XFiles");
    mtmFlixSeriesJoin(m, "Bill","DoctorWho");
    mtmFlixSeriesJoin(m, "Bill", "BlackMirror");
    mtmFlixSeriesJoin(m, "Bill", "StarWars1");
    mtmFlixSeriesJoin(m, "Bill", "RickAndMorty");
    mtmFlixSeriesJoin(m, "Bill", "Snatched");
    mtmFlixSeriesJoin(m, "Bill", "TheGodFather2");

    mtmFlixSeriesJoin(m, "Alice", "TheGodFather2");
    mtmFlixSeriesJoin(m, "Alice", "DoctorWho");
    mtmFlixSeriesJoin(m, "Alice", "Lost");
    mtmFlixSeriesJoin(m, "Alice", "StarWars1");
    mtmFlixSeriesJoin(m, "Alice", "RickAndMorty");
    mtmFlixSeriesJoin(m, "Alice", "Snatched");
    mtmFlixSeriesJoin(m, "Alice", "GamesOfThrones");
    mtmFlixSeriesJoin(m, "Alice", "XFiles");
    mtmFlixSeriesJoin(m, "Alice", "BlackMirror");
    mtmFlixSeriesJoin(m, "Alice", "TwoAndaHalfMan");

    FILE* fptr = fopen("users.txt", "w");
    mtmFlixReportUsers(m, fptr);
    fclose(fptr);

    FILE* fptr1 = fopen("recommendations.txt", "w");
    mtmFlixGetRecommendations(m, "Mathew",0, fptr1);
    fclose(fptr1);

    mtmFlixDestroy(m);
    return 0;
}