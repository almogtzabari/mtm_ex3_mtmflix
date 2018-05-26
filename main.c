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

int mtmFlixCreateDestroyTest(int* tests_passed){
    _print_mode_name("Testing Create&Destroy functions");
    int test_number = 0;
    MtmFlix m = mtmFlixCreate();
    test( !m , __LINE__, &test_number, "Couldn't create the Mtmflix series", tests_passed);
    _print_test_number(test_number, __LINE__+1);
    mtmFlixDestroy(NULL); // Silient test will crash if Destroy doesn't react properly on NULL input.
    _print_test_success(test_number);
    test_number += 1;
    mtmFlixDestroy(m); //Check for Memory leaks can't test that remotely
    *tests_passed += 1;
    return test_number;
}


int addUserTest(MtmFlix m, int* tests_passed){
    _print_mode_name("Testing addUser functions");
    int test_number = 0;
    test(mtmFlixAddUser(NULL,"user",10) != MTMFLIX_NULL_ARGUMENT,__LINE__, &test_number, "mtmFlixAddUser doesn't return MTMFLIX_NULL_ARGUMENT on NULL mtmflix input.", tests_passed);
    test(mtmFlixAddUser(m, NULL,10) != MTMFLIX_NULL_ARGUMENT,__LINE__, &test_number, "mtmFlixAddUser doesn't return MTMFLIX_NULL_ARGUMENT on NULL username input.", tests_passed);
    test(mtmFlixAddUser(m, "user",MTM_MAX_AGE+1) != MTMFLIX_ILLEGAL_AGE,__LINE__, &test_number, "mtmFlixAddUser doesn't return MTMFLIX_ILLEGAL_AGE on age bigger than MAX_AGE input.", tests_passed);
    test(mtmFlixAddUser(m, "UserOne",MTM_MAX_AGE) == MTMFLIX_ILLEGAL_AGE,__LINE__, &test_number, "mtmFlixAddUser doesnt' allow MTM_MAX_AGE.", tests_passed);
    test(mtmFlixAddUser(m, "UserTwo",MTM_MIN_AGE-1) != MTMFLIX_ILLEGAL_AGE,__LINE__, &test_number, "mtmFlixAddUser doesn't return MTMFLIX_ILLEGAL_AGE on age smaller than MIN_AGE input.", tests_passed);
    test(mtmFlixAddUser(m, "UserThree",MTM_MIN_AGE) == MTMFLIX_ILLEGAL_AGE,__LINE__, &test_number, "mtmFlixAddUser doesnt' allow MTM_MAX_AGE", tests_passed);
    test(mtmFlixAddUser(m,"!user",10) != MTMFLIX_ILLEGAL_USERNAME,__LINE__, &test_number, "mtmFlixAddUser allows invalid username (symbol)", tests_passed);
    test(mtmFlixAddUser(m,"u ser",10) != MTMFLIX_ILLEGAL_USERNAME,__LINE__, &test_number, "mtmFlixAddUser allows invalid username (space)", tests_passed);
    test(mtmFlixAddUser(m,"",10) != MTMFLIX_ILLEGAL_USERNAME,__LINE__, &test_number, "mtmFlixAddUser allows invalid username (empty)", tests_passed);
    test(mtmFlixAddUser(m, "user",MTM_MIN_AGE) != MTMFLIX_SUCCESS,__LINE__, &test_number, "mtmFlixAddUser doesn't return MTMFLIX_SUCCESS on correct input (lowercase name).", tests_passed);
    test(mtmFlixAddUser(m, "USER",MTM_MIN_AGE) != MTMFLIX_SUCCESS,__LINE__, &test_number, "mtmFlixAddUser doesn't return MTMFLIX_SUCCESS on correct input (uppercase name).", tests_passed);
    test(mtmFlixAddUser(m, "user",MTM_MIN_AGE+1) != MTMFLIX_USERNAME_ALREADY_USED,__LINE__, &test_number, "mtmFlixAddUser doesn't return MTMFLIX_USERNAME_ALREADY_USED.", tests_passed);
    //Next block of  tests check for right order of error return status
    test(mtmFlixAddUser(NULL, NULL,MTM_MIN_AGE+1) != MTMFLIX_NULL_ARGUMENT,__LINE__, &test_number, "mtmFlixAddUser doesn't return MTMFLIX_NULL_ARGUMENT on two NULL inputs and invalid age", tests_passed);
    test(mtmFlixAddUser(m, NULL,MTM_MIN_AGE+1) != MTMFLIX_NULL_ARGUMENT,__LINE__, &test_number, "mtmFlixAddUser doesn't return MTMFLIX_NULL_ARGUMENT on NULL username and invalid age", tests_passed);
    test(mtmFlixAddUser(m, "",MTM_MIN_AGE+1) != MTMFLIX_ILLEGAL_USERNAME,__LINE__, &test_number, "mtmFlixAddUser doesn't return MTMFLIX_NULL_ARGUMENT on wrong username and invalid age", tests_passed);
    test(mtmFlixAddUser(m, "user",MTM_MIN_AGE-1) != MTMFLIX_USERNAME_ALREADY_USED,__LINE__, &test_number, "mtmFlixAddUser doesn't return MTMFLIX_USERNAME_ALREADY_USED + incorrect age.", tests_passed);
    return test_number;
}

int removeUserTest(MtmFlix m,int* tests_passed){
    _print_mode_name("Testing removeUser functions");
    int test_number = 0;
    test(mtmFlixRemoveUser(NULL,"user") != MTMFLIX_NULL_ARGUMENT,__LINE__, &test_number, "mtmFlixRemoveUser doesn't return MTMFLIX_NULL_ARGUMENT on NULL input", tests_passed);
    test(mtmFlixRemoveUser(m, NULL) != MTMFLIX_NULL_ARGUMENT,__LINE__, &test_number, "mtmFlixRemoveUser doesn't return MTMFLIX_NULL_ARGUMENT on NULL input", tests_passed);
    test(mtmFlixRemoveUser(m, "") != MTMFLIX_USER_DOES_NOT_EXIST,__LINE__, &test_number, "mtmFlixRemoveUser doesn't return MTMFLIX_USER_DOES_NOT_EXIST on wrong username", tests_passed);
    test(mtmFlixRemoveUser(m, "") != MTMFLIX_USER_DOES_NOT_EXIST,__LINE__, &test_number, "mtmFlixRemoveUser doesn't return MTMFLIX_USER_DOES_NOT_EXIST on" , tests_passed);     //this test checks if the function is case-sensitive
    test(mtmFlixRemoveUser(m, "User") != MTMFLIX_USER_DOES_NOT_EXIST,__LINE__, &test_number, "mtmFlixRemoveUser doesn't return USER_DOES_NOT_EXIST on uppercase of existing user", tests_passed);
    test(mtmFlixRemoveUser(m, "User") != MTMFLIX_USER_DOES_NOT_EXIST,__LINE__, &test_number, "mtmFlixRemoveUser doesn't return USER_DOES_NOT_EXIST on uppercase of existing user", tests_passed);
    test(mtmFlixRemoveUser(m, "user") != MTMFLIX_SUCCESS,__LINE__, &test_number, "mtmFlixRemoveUser doesn't return MTMFLIX_SUCCESS on successful remove", tests_passed);
    test(mtmFlixRemoveUser(m, "user") != MTMFLIX_USER_DOES_NOT_EXIST,__LINE__, &test_number, "mtmFlixRemoveUser doesn't return MTMFLIX_USER_DOES_NOT_EXIST when trying to remove already removed user.", tests_passed);
    return test_number;
}

int addSeriesTest(MtmFlix m,int* tests_passed){
    _print_mode_name("Testing addSeries functions");
    int test_number = 0;
    int ages[2] = {MTM_MIN_AGE-1, MTM_MAX_AGE+1};
    int limits[2] = {MTM_MIN_AGE, MTM_MAX_AGE};
    test(mtmFlixAddSeries(NULL, "Series1",1,COMEDY,ages, 40) !=MTMFLIX_NULL_ARGUMENT,__LINE__, &test_number, "mtmFlixAddSeries doesn't return MTMFLIX_NULL_ARGUMENT on NULL input.", tests_passed);
    test(mtmFlixAddSeries(m, NULL,1,COMEDY,ages, 40) !=MTMFLIX_NULL_ARGUMENT,__LINE__, &test_number, "mtmFlixAddSeries doesn't return MTMFLIX_NULL_ARGUMENT on NULL series name input.", tests_passed);
    test(mtmFlixAddSeries(m, "Series#", 1, COMEDY, limits, 40) !=MTMFLIX_ILLEGAL_SERIES_NAME, __LINE__, &test_number, "mtmFlixAddSeries doesn't retujrn MTMFLIX_ILLEGAL_SERIES_NAME on wrong imput(symbol)", tests_passed);
    test(mtmFlixAddSeries(m, "Series 1", 1, COMEDY, limits, 40) !=MTMFLIX_ILLEGAL_SERIES_NAME, __LINE__, &test_number, "mtmFlixAddSeries doesn't retujrn MTMFLIX_ILLEGAL_SERIES_NAME on wrong imput(spaces)", tests_passed);
    test(mtmFlixAddSeries(m, "Series1",0,COMEDY,ages, 40) !=MTMFLIX_ILLEGAL_EPISODES_NUM,__LINE__, &test_number, "mtmFlixAddSeries doesn't return MTMFLIX_ILLEGAL_EPISODES_NUM on 0 input.", tests_passed);
    test(mtmFlixAddSeries(m, "Series1",-1,COMEDY,ages, 40) !=MTMFLIX_ILLEGAL_EPISODES_NUM,__LINE__, &test_number, "mtmFlixAddSeries doesn't return MTMFLIX_ILLEGAL_EPISODES_NUM on negative input.", tests_passed);
    test(mtmFlixAddSeries(m, "Series1",1,COMEDY,ages, 0) !=MTMFLIX_ILLEGAL_EPISODES_DURATION,__LINE__, &test_number, "mtmFlixAddSeries doesn't return MTMFLIX_ILLEGAL_EPISODES_DURATION on 0 input.", tests_passed);
    test(mtmFlixAddSeries(m, "Series1",1,COMEDY,ages, -1) !=MTMFLIX_ILLEGAL_EPISODES_DURATION,__LINE__, &test_number, "mtmFlixAddSeries doesn't return MTMFLIX_ILLEGAL_EPISODES_DURATION on negative input.", tests_passed);
    test(mtmFlixAddSeries(m, "Series1",1,COMEDY,NULL, 40) !=MTMFLIX_SUCCESS,__LINE__, &test_number, "mtmFlixAddSeries doesn't allow NULL as age input.", tests_passed);
    test(mtmFlixAddSeries(m, "Series1",1,COMEDY,NULL, 40) !=MTMFLIX_SERIES_ALREADY_EXISTS,__LINE__, &test_number, "mtmFlixAddSeries doesn't return MTMFLIX_SERIES_ALREADY_EXISTS.", tests_passed);
    test(mtmFlixAddSeries(m, "Series2", 1, COMEDY, limits, 40) !=MTMFLIX_SUCCESS, __LINE__, &test_number, "mtmFlixAddSeries doesn't add series with ages set as MAX/MIN age.", tests_passed);
    //Next block of  tests check for right order of error return status
    test(mtmFlixAddSeries(NULL, NULL,0,COMEDY,ages, 0) !=MTMFLIX_NULL_ARGUMENT,__LINE__, &test_number, "mtmFlixAddSeries doesn't return MTMFLIX_NULL_ARGUMENT on all wrong input", tests_passed);
    test(mtmFlixAddSeries(m, NULL,0,COMEDY,ages, 0) !=MTMFLIX_NULL_ARGUMENT,__LINE__, &test_number, "mtmFlixAddSeries doesn't return MTMFLIX_NULL_ARGUMENT on all wrong input", tests_passed);
    test(mtmFlixAddSeries(m, "Series1",0,COMEDY,ages, 0) !=MTMFLIX_SERIES_ALREADY_EXISTS,__LINE__, &test_number, "mtmFlixAddSeries doesn't return MTMFLIX_SERIES_ALREADY_EXISTS on serie which exists and wrong episode number/duration", tests_passed);
    test(mtmFlixAddSeries(m, "Series3",0,COMEDY,ages, 0) !=MTMFLIX_ILLEGAL_EPISODES_NUM,__LINE__, &test_number, "mtmFlixAddSeries doesn't return MTMFLIX_ILLEGAL_EPISODES_NUM  on wrong episode number/duration", tests_passed);
    return test_number;
}


int removeSeriesTest(MtmFlix m,int* tests_passed){
    _print_mode_name("Testing removeSeries functions");
    int test_number = 0;
    test(mtmFlixRemoveSeries(NULL, "Series1") != MTMFLIX_NULL_ARGUMENT,__LINE__, &test_number, "mtmFlixRemoveSeries doesn't return MTMFLIX_NULL_ARGUMENT on NULL mtmflix input.", tests_passed);
    test(mtmFlixRemoveSeries(m, NULL) != MTMFLIX_NULL_ARGUMENT,__LINE__, &test_number, "mtmFlixRemoveSeries doesn't return MTMFLIX_NULL_ARGUMENT on NULL name input.", tests_passed);
    test(mtmFlixRemoveSeries(m, "1seireS") != MTMFLIX_SERIES_DOES_NOT_EXIST,__LINE__, &test_number, "mtmFlixRemoveSeries doesn't return MTMFLIX_SERIES_DOES_NOT_EXIST.", tests_passed);
    test(mtmFlixRemoveSeries(m, "Series1") != MTMFLIX_SUCCESS,__LINE__, &test_number, "mtmFlixRemoveSeries doesn't return MTMFLIX_SUCCESS on successful remove.", tests_passed);
    test(mtmFlixRemoveSeries(m, "Series1") == MTMFLIX_SUCCESS,__LINE__, &test_number, "mtmFlixRemoveSeries return MTMFLIX_SUCCESS on already removed series.", tests_passed);
    return test_number;
}

int reportSeriesTest(MtmFlix m,int* tests_passed){
    _print_mode_name("Testing reportSeries functions");
    int test_number = 0;
    //Those test aren't testing the output, output tests will be in separate test_input.exe file
    test(mtmFlixReportSeries(NULL, 1, stdout) != MTMFLIX_NULL_ARGUMENT ,__LINE__, &test_number, "mtmFlixReportSeries doesn't return MTMFLIX_NULL_ARGUMENT on null mtmflix input.", tests_passed);
    test(mtmFlixReportSeries(m, 1, NULL) != MTMFLIX_NULL_ARGUMENT ,__LINE__, &test_number, "mtmFlixReportSeries doesn't return MTMFLIX_NULL_ARGUMENT on null FPTR input.", tests_passed);
    MtmFlix m_temp = mtmFlixCreate();
    test(mtmFlixReportSeries(m_temp, 1, stdout) != MTMFLIX_NO_SERIES, __LINE__, &test_number, "mtmFlixReportSeries doesn't return MTMFLIX_NO_SERIES on empty mtmflix input.", tests_passed);
    mtmFlixDestroy(m_temp);
    // in the next test i chose stderr so it won't appear in the console
    FILE* fptr = fopen("garbage.txt", "w");
    test(mtmFlixReportSeries(m, 1, fptr) != MTMFLIX_SUCCESS, __LINE__, &test_number, "mtmFlixReportSeries doesn't return MTMFLIX_SUCCESS on valid input.", tests_passed);
    fclose(fptr);
    return test_number;
}

int reportUsersTest(MtmFlix m,int* tests_passed){
    _print_mode_name("Testing reportUsers functions");
    int test_number = 0;
    //Those test aren't testing the output, output tests will be in separate test_input.exe file
    test(mtmFlixReportUsers(NULL,  stdout) != MTMFLIX_NULL_ARGUMENT ,__LINE__, &test_number, "mtmFlixReportUsers doesn't return MTMFLIX_NULL_ARGUMENT on null mtmflix input.", tests_passed);
    test(mtmFlixReportUsers(m,  NULL) != MTMFLIX_NULL_ARGUMENT ,__LINE__, &test_number, "mtmFlixReportUsers doesn't return MTMFLIX_NULL_ARGUMENT on null FPTR input.", tests_passed);
    MtmFlix m_temp = mtmFlixCreate();
    test(mtmFlixReportUsers(m_temp,  stdout) != MTMFLIX_NO_USERS, __LINE__, &test_number, "mtmFlixReportUsers doesn't return MTMFLIX_NO_USERS on empty mtmflix input.", tests_passed);
    mtmFlixDestroy(m_temp);
    // in the next test i chose stderr so it won't appear in the console
    FILE* fptr = fopen("garbage.txt", "w");
    test(mtmFlixReportUsers(m,  fptr) != MTMFLIX_SUCCESS, __LINE__, &test_number, "mtmFlixReportUsers doesn't return MTMFLIX_SUCCESS on valid input.", tests_passed);
    fclose(fptr);
    return test_number;
}

int seriesJoinTest(MtmFlix m,int* tests_passed){
    _print_mode_name("Testing seriesJoin functions");
    int test_number = 0;
    mtmFlixAddUser(m, "UserTwo", MTM_MIN_AGE+1); //Assuming u've passed previous tests this should work
    test(mtmFlixSeriesJoin(NULL,"UserTwo", "Series2") != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixSeriesJoin doesn't return MTMFLIX_NULL_ARGUMENT on NULL mtmflix input.", tests_passed);
    test(mtmFlixSeriesJoin(m, NULL, "Series2") != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixSeriesJoin doesn't return MTMFLIX_NULL_ARGUMENT on NULL user name input.", tests_passed);
    test(mtmFlixSeriesJoin(m,"UserTwo", NULL) != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixSeriesJoin doesn't return MTMFLIX_NULL_ARGUMENT on NULL series name input.", tests_passed);
    test(mtmFlixSeriesJoin(m,"UserT", "Series2") != MTMFLIX_USER_DOES_NOT_EXIST, __LINE__, &test_number, "mtmFlixSeriesJoin doesn't return MTMFLIX_USER_DOES_NOT_EXIST on not existing series name input.", tests_passed);
    test(mtmFlixSeriesJoin(m,"UserTwo", "2Series") != MTMFLIX_SERIES_DOES_NOT_EXIST, __LINE__, &test_number, "mtmFlixSeriesJoin doesn't return MTMFLIX_SERIES_DOES_NOT_EXIST on not existing user input.", tests_passed);
    int ages[2] = {MTM_MAX_AGE-1, MTM_MAX_AGE};
    mtmFlixAddSeries(m, "MaxAge", 1, COMEDY, ages,40);
    int ages1[2] = {MTM_MIN_AGE+1, MTM_MIN_AGE+2};
    mtmFlixAddSeries(m, "MinAge", 1, COMEDY, ages1,40);
    test(mtmFlixSeriesJoin(m, "UserTwo", "MaxAge") != MTMFLIX_USER_NOT_IN_THE_RIGHT_AGE, __LINE__, &test_number, "mtmFlixSeriesJoin doesn't return MTMFLIX_USER_NOT_IN_THE_RIGHT_AGE on user who doesn't fit into series age limit.", tests_passed);
    test(mtmFlixSeriesJoin(m, "UserTwo", "MinAge") != MTMFLIX_SUCCESS, __LINE__, &test_number, "mtmFlixSeriesJoin doesn't return MTMFLIX_SUCCESS on valid input.", tests_passed);
    //Next block of  tests check for right order of error return status
    test(mtmFlixSeriesJoin(NULL, "User15", "Series15") != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixSeriesJoin doesn't return MTMFLIX_NULL_ARGUMENT when everything is wrong", tests_passed);
    test(mtmFlixSeriesJoin(m, "User15", "Series15") != MTMFLIX_USER_DOES_NOT_EXIST, __LINE__, &test_number, "mtmFlixSeriesJoin doesn't return MTMFLIX_SERIES_DOES_NOT_EXIST on series/user doesn't exist ", tests_passed);
    return test_number;
}

int seriesLeaveTest(MtmFlix m,int* tests_passed){
    _print_mode_name("Testing seriesLeave functions");
    int test_number = 0;
    test( mtmFlixSeriesLeave(NULL, "UserTwo", "MinAge") != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixSeriesLeave doesn't return MTMFLIX_NULL_ARGUMENT on NULL mtmflix input.", tests_passed);
    test( mtmFlixSeriesLeave(m, NULL, "MinAge") != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixSeriesLeave doesn't return MTMFLIX_NULL_ARGUMENT on NULL user name input.", tests_passed);
    test( mtmFlixSeriesLeave(m, "UserTwo", NULL) != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixSeriesLeave doesn't return MTMFLIX_NULL_ARGUMENT on NULL series name input.", tests_passed);
    test( mtmFlixSeriesLeave(m, "UserTo", "MinAge") != MTMFLIX_USER_DOES_NOT_EXIST, __LINE__, &test_number, "mtmFlixSeriesLeave doesn't return MTMFLIX_USER_DOES_NOT_EXIST on non existing user input.", tests_passed);
    test( mtmFlixSeriesLeave(m, "UserTwo", "MiAge") != MTMFLIX_SERIES_DOES_NOT_EXIST, __LINE__, &test_number, "mtmFlixSeriesLeave doesn't return MTMFLIX_SERIES_DOES_NOT_EXIST on on non existing series input.", tests_passed);
    test( mtmFlixSeriesLeave(m, "UserTwo", "MinAge") != MTMFLIX_SUCCESS, __LINE__, &test_number, "mtmFlixSeriesLeave doesn't return MTMFLIX_SUCCESS on valid input.", tests_passed);
    test( mtmFlixSeriesLeave(m, "UserTwo", "MinAge") != MTMFLIX_SUCCESS, __LINE__, &test_number, "mtmFlixSeriesLeave doesn't return MTMFLIX_SUCCESS on leaving series.", tests_passed);
    test( mtmFlixSeriesLeave(m, "UserTwo", "MinAge") != MTMFLIX_SUCCESS, __LINE__, &test_number, "mtmFlixSeriesLeave doesn't return MTMFLIX_SUCCESS on leaving series which is not in his list.", tests_passed);
    return test_number;
}

int addFriendTest(MtmFlix m,int* tests_passed){
    _print_mode_name("Testing addFriend functions");
    int test_number = 0;
    test(mtmFlixAddFriend(NULL,"UserTwo", "UserThree") != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixAddFriend doesn't return MTMFLIX_NULL_ARGUMENT on NULL mtmflix input.", tests_passed);
    test(mtmFlixAddFriend(m, NULL, "UserThree") != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixAddFriend doesn't return MTMFLIX_NULL_ARGUMENT on NULL username1 input.", tests_passed);
    test(mtmFlixAddFriend(m,"UserTwo", NULL) != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixAddFriend doesn't return MTMFLIX_NULL_ARGUMENT on NULL username2 input.", tests_passed);
    test(mtmFlixAddFriend(m,"UserTo", "UserTwo") != MTMFLIX_USER_DOES_NOT_EXIST, __LINE__, &test_number, "mtmFlixAddFriend doesn't return MTMFLIX_USER_DOES_NOT_EXIST on non existing username1 input.", tests_passed);
    test(mtmFlixAddFriend(m,"UserTwo", "UserTo") != MTMFLIX_USER_DOES_NOT_EXIST, __LINE__, &test_number, "mtmFlixAddFriend doesn't return MTMFLIX_USER_DOES_NOT_EXIST on non existing username1 input.", tests_passed);
    mtmFlixAddUser(m, "NewUser", 25);
    test(mtmFlixAddFriend(m, "NewUser", "NewUser") != MTMFLIX_SUCCESS, __LINE__, &test_number, "mtmFlixAddFriends doesn't return MTMFLIX_SUCCESS when the user adds himself as a friend(nothing should happend but the status should be MTMFLIX_SUCCESS)", tests_passed);
    test(mtmFlixAddFriend(m, "UserTwo", "NewUser") != MTMFLIX_SUCCESS, __LINE__, &test_number, "mtmFlixAddFriend doesn't return MTMFLIX_SUCCESS on valid input.", tests_passed);
    test(mtmFlixAddFriend(m, "UserTwo", "NewUser") != MTMFLIX_SUCCESS, __LINE__, &test_number, "mtmFlixAddFriend doesn't return MTMFLIX_SUCCESS on when adding a friends which already exists.", tests_passed);
    return test_number;
}

int removeFriendTest(MtmFlix m,int* tests_passed){
    _print_mode_name("Testing removeFriend functions");
    int test_number = 0;
    test(mtmFlixRemoveFriend(NULL, "UserTwo", "NewUser") != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixRemoveFriend doesn't return MTMFLIX_NULL_ARGUMENT on NULL mtmflix input.", tests_passed);
    test(mtmFlixRemoveFriend(m, NULL, "NewUser") != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixRemoveFriend doesn't return MTMFLIX_NULL_ARGUMENT on NULL username1 input.", tests_passed);
    test(mtmFlixRemoveFriend(m, "UserTwo", NULL) != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixRemoveFriend doesn't return MTMFLIX_NULL_ARGUMENT on NULL username2 input.", tests_passed);
    test(mtmFlixRemoveFriend(m, "UserTwo", "NewUser") != MTMFLIX_SUCCESS, __LINE__, &test_number, "mtmFlixRemoveFriend doesn't return MTMFLIX_SUCCESS on valid input.", tests_passed);
    test(mtmFlixRemoveFriend(m, "UserTwo", "NewUser") != MTMFLIX_SUCCESS, __LINE__, &test_number, "mtmFlixRemoveFriend doesn't return MTMFLIX_SUCCESS on when removing a friend which already had been removed.", tests_passed);
    return test_number;
}

int getRecommendationsTest(MtmFlix m,int* tests_passed){
    _print_mode_name("Testing getRecommendations functions");
    int test_number = 0;
    test(mtmFlixGetRecommendations(NULL, "UserTwo", 1, stdout) != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixGetRecommendations doesn't return MTMFLIX_NULL_ARGUMENT on NULL mtmflix input.", tests_passed);
    test(mtmFlixGetRecommendations(m, NULL, 1, stdout) != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixGetRecommendations doesn't return MTMFLIX_NULL_ARGUMENT on NULL username input.", tests_passed);
    test(mtmFlixGetRecommendations(m, "UserTwo", 1, NULL) != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixGetRecommendations doesn't return MTMFLIX_NULL_ARGUMENT on NULL FPTR input.", tests_passed);
    test(mtmFlixGetRecommendations(m, "UserTo", 1, stdout) != MTMFLIX_USER_DOES_NOT_EXIST, __LINE__, &test_number, "mtmFlixGetRecommendations doesn't return MTMFLIX_USER_DOES_NOT_EXIST on non existing user.", tests_passed);
    test(mtmFlixGetRecommendations(m, "UserTwo", -1, stdout) != MTMFLIX_ILLEGAL_NUMBER, __LINE__, &test_number, "mtmFlixGetRecommendations doesn't return MTMFLIX_ILLEGAL_NUMBER on negative count input.", tests_passed);
    //Next block of  tests check for right order of error return status
    test(mtmFlixGetRecommendations(NULL, "UserTo", -1, stdout) != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixGetRecommendations doesn't return MTMFLIX_NULL_ARGUMENT on NULL mtmflix ,not existing user and illegal count", tests_passed);
    test(mtmFlixGetRecommendations(m, "UserTo", -1, NULL) != MTMFLIX_NULL_ARGUMENT, __LINE__, &test_number, "mtmFlixGetRecommendations doesn't return MTMFLIX_NULL_ARGUMENT on NULL FPTR ,not existing user and illegal count", tests_passed);
    test(mtmFlixGetRecommendations(m, "UserTo", -1, stdout) != MTMFLIX_USER_DOES_NOT_EXIST, __LINE__, &test_number, "mtmFlixGetRecommendations doesn't return MTMFLIX_USER_DOES_NOT_EXIST on none existing user and illegal count", tests_passed);
    FILE* fptr = fopen("garbage.txt", "w");
    test(mtmFlixGetRecommendations(m, "UserTwo", 0, fptr) != MTMFLIX_SUCCESS, __LINE__, &test_number, "mtmFlixGetRecommendations doesn't return MTMFLIX_SUCCESS on valid input.", tests_passed);
    fclose(fptr);
    return test_number;
}

int main() {
    printf("\nWelcome to the homework 3 mtmflix tests, written by Vova Parakhin.\n\n---Passing those tests won't "
           "guarantee you a good grade---\nBut they might get you close to one "
          ",make some tests yourself to be sure.\n\n");
    printf("You can change w/e you want in the file itself but make sure \nto contact me if you want to upload");
    printf(" \'upgraded version\' of the file\n");
    printf("\nPress any button to start the tests:. \n\n");
    getchar();
    int tests_passed = 0;
    int tests_number = 0;
    MtmFlix m = mtmFlixCreate();
    tests_number += mtmFlixCreateDestroyTest( &tests_passed);
    tests_number += addUserTest(m, &tests_passed);
    tests_number += removeUserTest(m, &tests_passed);
    tests_number += addSeriesTest(m, &tests_passed);
    tests_number += removeSeriesTest(m, &tests_passed);
    tests_number += reportSeriesTest(m, &tests_passed);
    tests_number += reportUsersTest(m, &tests_passed);
    tests_number += seriesJoinTest(m, &tests_passed);
    tests_number += seriesLeaveTest(m, &tests_passed);
    tests_number += addFriendTest(m, &tests_passed);
    tests_number += removeFriendTest(m, &tests_passed);
    tests_number += getRecommendationsTest(m, &tests_passed);
    mtmFlixDestroy(m);
    print_grade(tests_number, tests_passed);
    return 0;
}