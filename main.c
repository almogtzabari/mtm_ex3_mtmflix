#include "mtmflix.h"
int main(){
    MtmFlix flix=mtmFlixCreate();
    mtmFlixAddUser(flix,"Vered",57);
    mtmFlixAddUser(flix,"Ariel",24);
    mtmFlixAddUser(flix,"Ligad",27);
    mtmFlixAddUser(flix,"Orian",21);
    mtmFlixAddUser(flix,"Efraim",60);

    int ages [2]= {29,100};
    mtmFlixAddSeries(flix,"Stranger",4,DRAMA,ages,4);
    mtmFlixAddSeries(flix,"Suits",4,DRAMA,NULL,4);
    mtmFlixAddSeries(flix,"GameOfThrones",4,MYSTERY,NULL,4);


    mtmFlixSeriesJoin(flix,"Vered","Stranger");
    mtmFlixSeriesJoin(flix,"Vered","Suits");
    mtmFlixSeriesJoin(flix,"Vered","GameOfThrones");

    mtmFlixAddFriend(flix,"Vered","Ariel");
    mtmFlixAddFriend(flix,"Vered","Ligad");
    mtmFlixAddFriend(flix,"Vered","Orian");
    mtmFlixAddFriend(flix,"Vered","Efraim");


    FILE* temp=fopen("temp1.txt","w");
    mtmFlixReportSeries(flix,3,temp);
    fclose(temp);
    return 0;
}