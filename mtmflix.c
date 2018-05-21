#include "mtmflix.h"
#include "map.h"
#include "series.h"
#include "utilities.h"
#include "user.h"

static bool usernameCheck (const char* username);

struct mtmFlix_t{
    Set users;
    Set series;
};


MtmFlix mtmFlixCreate(){
    MtmFlix flix = malloc(sizeof(*flix));
    if(!flix){
        /* Failed to allocate memory. */
        return NULL;
    }
    flix->series = setCreate(copySeriesSetElements,freeSeriesSetElements,
            compareSeriesSetElements);
    if(!flix->series){
        /* Failed to allocate memory for the series map. */
        free(flix);
        return NULL;
    }
    flix->users =setCreate(,,);
    if(!flix->users){
        setDestroy(flix->series);
        free(flix);
    }
    return flix;

}

MtmFlixResult mtmFlixAddUser(MtmFlix mtmflix,
                             const char* username, int age){
    if(!mtmflix || !username){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!usernameCheck(username)){
        return MTMFLIX_ILLEGAL_USERNAME;
    }
    if(age<MTM_MIN_AGE || age>MTM_MAX_AGE){
        return MTMFLIX_ILLEGAL_AGE;
    }

}


static bool usernameCheck (const char* username){
    bool contain_letter=false;
    while(*username){
        if(*username<'0' || (*username>'9' && *username<'A') ||
                (*username>'Z' && *username<'a') || *username>'z'){
            return false;
        }
        if ((*username>'A' && *username<'Z') ||
            (*username>'a' && *username<'z')){
            contain_letter=true;
        }
        username++;
    }
    return contain_letter;
}


