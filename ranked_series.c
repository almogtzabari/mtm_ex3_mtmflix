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

RankedSeries rankedSeriesCopy (RankedSeries ranked_series){
    RankedSeries ranked_series_copy=rankedSeriesCreate(ranked_series->rank,
            ranked_series->series_name,ranked_series->series_genre);
    if(!ranked_series_copy){
        return NULL;
    }
    return ranked_series_copy;
}

void rankedSeriesDestroy (RankedSeries ranked_series){
    free(ranked_series->series_genre);
    free(ranked_series->series_name);
    free(ranked_series);
}

int rankedSeriesCompare (RankedSeries ranked_series1,
                         RankedSeries ranked_series2){
    if((ranked_series1->rank)!=(ranked_series2->rank)){
        return (ranked_series2->rank)-(ranked_series1->rank);
    }
    return strcmp(ranked_series1->series_name,ranked_series2->series_name);
}

void rankedSeriesPrintToFile(int number_of_series_to_print,
                             Set ranked_series_set,
                             FILE* outputStream,MtmFlixResult* result){
    if(number_of_series_to_print==0){
        number_of_series_to_print=setGetSize(ranked_series_set);
    }
    SET_FOREACH(RankedSeries ,ranked_series,ranked_series_set){
        if(number_of_series_to_print>0){
            const char* ranked_series_details=mtmPrintSeries
                    (ranked_series->series_name,
                     ranked_series->series_genre);
            if(!ranked_series_details){
                *result=MTMFLIX_OUT_OF_MEMORY;
                return;
            }
            if((fprintf(outputStream,"%s",ranked_series_details)<0){
                *result=MTMFLIX_OUT_OF_MEMORY;
                return;
            }
        }
        number_of_series_to_print--;
    }
}