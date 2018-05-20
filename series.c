#include "series.h"
#include "mtm_ex3.h"
#include <malloc.h>
#include <string.h>


//-----------------------------------------------------------------------//
//                            STRUCT SERIES                              //
//-----------------------------------------------------------------------//

struct series_t{
    const char* series_name;
    int number_of_episodes;
    Genre genre;
    int* ages;
    double episode_duration;
};

//-----------------------------------------------------------------------//
//                       STRUCT SERIES FUNCTIONS                         //
//-----------------------------------------------------------------------//

Series seriesCreate(const char* series_name,int number_of_episodes,
                    Genre genre, int* ages, double episode_duration){

    Series series = malloc(sizeof(*series));
    if(!series){
        /* Series memory allocation failed. */
        return NULL;
    }
    char* name = malloc(strlen(series_name)+1);
    if(!name){
        /* Name memory allocation failed. */
        free(series);
        return NULL;
    }
}
SeriesDataElement copySeriesDataElements(SeriesDataElement series_data){

}

SeriesKeyElement copySeriesKeyElements(SeriesKeyElement series_key){

}

void freeSeriesDataElements(SeriesDataElement series_data){

}

void freeSeriesKeyElements(SeriesKeyElement series_key){

}

int compareSeriesDataElements(SeriesDataElement series_data1,
                              SeriesDataElement series_data2){

}


