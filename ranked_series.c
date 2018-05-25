#include <malloc.h>
#include <memory.h>
#include "ranked_series.h"

struct ranked_series_t{
    int rank;
    char* series_name;
    char* series_genre;
};

RankedSeries rankedSeriesCreate (int rank,char* series_name,
                                 char* series_genre){
    RankedSeries new_ranked_series=malloc(sizeof(*new_ranked_series));
    if(!new_ranked_series){
        return NULL;
    }
    char* series_name_copy=malloc(strlen(series_name)+1);
    if(!series_name_copy){
        free(new_ranked_series);
        return NULL;
    }
    strcpy(series_name_copy,series_name);
    new_ranked_series->series_name=series_name_copy;
    char* series_genre_copy=malloc(strlen(series_genre)+1);
    if(!series_genre_copy){
        free(series_name_copy);
        free(new_ranked_series);
        return NULL;
    }
    strcpy(series_genre_copy,series_genre);
    new_ranked_series->series_genre;
    new_ranked_series->rank=rank;
    return new_ranked_series;
}

