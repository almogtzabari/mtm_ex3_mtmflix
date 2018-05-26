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

//    MtmFlix m = mtmFlixCreate();
//    mtmFlixAddUser(m,"alex",20);
//    mtmFlixAddUser(m,"alex1",20);
//    mtmFlixRemoveUser(m,"alex");
//    mtmFlixRemoveUser(m,"alex1");
//    mtmFlixDestroy(m);
    User new_user = userCreate("moshe",20);
    Set users = setCreate(userCopySetElement,userDestroySetElememnt,
                          userCompareSetElements);
    setAdd(users,new_user);
    setRemove(users,new_user);
    userDestroy(new_user);
//    setDestroy(users);
    return 0;
}