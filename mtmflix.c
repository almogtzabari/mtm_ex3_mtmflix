#include "mtmflix.h"
#include "map.h"
#include "series.h"
#include "utilities.h"
#include "user.h"

static bool usernameCheck (const char* username);

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
    flix->users =NULL;
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
    if(mapContains(mtmflix->users,(MapKeyElement)username)){
        return MTMFLIX_USERNAME_ALREADY_USED;
    }
    MapDataElement user_data=userCreateMapDataElement(username,age);
    if(!user_data){
        /*If creating user data failed */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    MapKeyElement user_key=(MapKeyElement)usernameCopy(username); //todo: Check map copying key element to replace casting.
    if(!user_key){
        freeUserMapDataElements(user_data);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(mapPut(mtmflix->users,user_data,user_key)!=MAP_SUCCESS) {
        /* If adding a user failed */
        freeUserMapDataElements(user_data);
        freeUserMapKeyElements(user_key);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    return MTMFLIX_SUCCESS;
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


