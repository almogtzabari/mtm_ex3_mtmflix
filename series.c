#include "series.h"
#include "mtm_ex3.h"




//-----------------------------------------------------------------------//
//                   STATIC FUNCTIONS DECALARATIONS                      //
//-----------------------------------------------------------------------//

static int* seriesInsertAgeLimit(int *ages, SeriesResult *status);
static char* getGenreNameByEnum(Genre genre);
static int getGenrePosition(Genre genre);

//-----------------------------------------------------------------------//
//                            STRUCT SERIES                              //
//-----------------------------------------------------------------------//

struct series_t{
    const char* series_name;
    int number_of_episodes;
    Genre genre;
    int* ages;
    int episode_duration;
};

//-----------------------------------------------------------------------//
//                       STRUCT SERIES FUNCTIONS                         //
//-----------------------------------------------------------------------//

/**
 ***** Function: seriesCreate *****
 * Description: Creates a new series.
 *
 * @param series_name - Name of the series.
 * @param number_of_episodes - Number of episodes of the series.
 * @param genre - Genre of the series.
 * @param ages - Array of age limitations. The size of the array is 2. The
 * First cell is the minimum age and the second cell is maximum age.
 * If ages is NULL there is no age limitaions.
 * @param episode_duration - Average duration of episode in series.
 * @return - New series or NULL in case of fail.
 */
Series seriesCreate(const char* series_name, int number_of_episodes,
                    Genre genre, int* ages, int episode_duration){

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
    strcpy(name,series_name);
    SeriesResult status;
    int* series_age_limit = seriesInsertAgeLimit(ages, &status);
    if(status != SERIES_SUCCESS){
        /* Couldn't allocate memory for ages array of series. */
        free(name);
        free(series);
        return NULL;
    }
    series->ages = series_age_limit;
    series->series_name = (const char*)name;
    series->genre = genre;
    series->episode_duration = episode_duration;
    series->number_of_episodes = number_of_episodes;
    return series;
}

/**
 ***** Function: seriesCopy *****
 * Description: Creates a copy of the given series.
 *
 * @param series - A series to create a copy of.
 * @return - A copy of the given series or NULL in case of failure.
 */
Series seriesCopy(Series series){
    if(!series){
        /* NULL argument.*/
        return NULL;
    }
    Series series_copy = seriesCreate(series->series_name,
                                      series->number_of_episodes,series->genre,series->ages,
                                      series->episode_duration);

    if(!series_copy){
        /* Series create failed. */
        return NULL;
    }
    return series_copy;
}



char* seriesCopyName( char *name){
    if(!name){
        /* NULL argument. */
        return NULL;
    }
    char* name_copy = malloc(sizeof(strlen(name)+1));
    if(!name_copy){
        /* Name copy memory allocation failed. */
        return NULL;
    }
    strcpy(name_copy,name);
    return name_copy;
}

/**
 ***** Function: seriesDestroy *****
 * Description: Free all allocated memory of a given series.
 *
 * @param series - Series we want to destroy.
 */
void seriesDestroy(Series series){
    free((char*)series->series_name);
    free(series->ages);
    free(series);
}

int seriesCompare(Series series1, Series series2){
    if(!strcmp(series1->series_name,series2->series_name)){
        /* Series has the same name. This is in order to check if a series
         * exist in a set using its name only. */
        return 0;
    }
    assert(series1 && series2);
    int genre1_position = getGenrePosition(series1->genre);
    int genre2_position = getGenrePosition(series2->genre);
    int genre_diff = genre2_position-genre1_position;
    if(genre_diff){
        /* The series has different genres. */
        return genre_diff;
    }
    /* Two series has the same genre. */
    return strcmp(series1->series_name,series2->series_name);
}

void seriesDestroyName(char* name){
    free(name);
}


//-----------------------------------------------------------------------//
//                    STRUCT SERIES STATIC FUNCTIONS                     //
//-----------------------------------------------------------------------//

/** //todo: what about bad min/max input?
 ***** Function: seriesSetAgeLimit *****
 * Description: This function will be used at seriesCreate.
 * The function gets an array of ages or NULL. If NULL, there are no age
 * limitations. Else, the first cell will contain minimum age limit and the
 * second cell will contain maximum age limit. In case not NULL, the
 * function will duplicate the array with a slight change: If the maximum
 * age in the array is higher than MtmFlix maximum age then the new array
 * will contain MtmFlix maximum age. If the minimum age in the array is
 * lower than MtmFlix minimum age then the new array will contain MtmFlix
 * minimum age.
 *
 * @param ages - Array of age limitations. If NULL there are no limitaions.
 * @return - A
 */
static int* seriesInsertAgeLimit(int* ages, SeriesResult *status) {
    if (ages) {
        /* There is an age limitations */
        int *series_ages = malloc(sizeof(ages));
        if (!series_ages) {
            /* Ages array memory allocation failed. */
            *status = SERIES_MEMORY_ALLOCATION_FAILED;
            return NULL;
        }
        if (ages[0] < MTM_MIN_AGE) {
            /* Minimum age of series is lower then MtmFlix minimum age. */
            series_ages[0] = MTM_MIN_AGE;
        } else {
            series_ages[0] = ages[0];
        }
        if (ages[1] > MTM_MAX_AGE) {
            /* Maximum age of series is higher then MtmFlix maximum age. */
            series_ages[1] = MTM_MAX_AGE;
        } else {
            series_ages[1] = ages[1];
        }
        *status = SERIES_SUCCESS;
        return series_ages;
    }
    /* There is no age limitation. */
    *status = SERIES_SUCCESS;
    return NULL;
}

Genre seriesGetGenre (Series series){
    return series->genre;
}

/**
 ***** Function: printSeriesDetailsToFile *****
 * Description:Prints a series name and genre to a file.
 * @param current_series - The series we want to print to the given file.
 * @param outputStream - The file we want to print into.
 * @return
 * SERIES_MEMORY_ALLOCATION_FAILED - In case of memory allocation error.
 * SERIES_SUCCESS - Printing to file succeeded.
 */
SeriesResult printSeriesDetailsToFile(Series current_series,
                                      FILE* outputStream){
    char* series_genre_string = getGenreNameByEnum
            (current_series->genre);
    if(!series_genre_string){
        return SERIES_MEMORY_ALLOCATION_FAILED;
    }
    const char* series_details = mtmPrintSeries(current_series->series_name,
                                                series_genre_string);
    free(series_genre_string);
    if(!series_details){
        return SERIES_MEMORY_ALLOCATION_FAILED;
    }
    fprintf(outputStream,"%s\n",series_details);
    return SERIES_SUCCESS;
}

/**
 ***** Function: getGenreNameByEnum *****
 * Description: Converts genre to the string that represents the genre.
 *
 * @param genre - A number that represents a genre.
 * @return
 * NULL in case of memory allocation error, else the string represents the
 * number of genre.
 */
static char* getGenreNameByEnum(Genre genre){
    const char* genres[] = { "SCIENCE_FICTION", "DRAMA", "COMEDY", "CRIME",
                             "MYSTERY","DOCUMENTARY", "ROMANCE", "HORROR"};
    char* genre_string = malloc(strlen(genres[genre])+1);
    if(!genre_string){
        return NULL;
    }
    strcpy(genre_string,genres[genre]);
    return genre_string;
}


/**
 ***** Function: getGenrePosition *****
 * Description: Gets a genre and returns its lexicographical position.
 * ascii value.
 *
 * //------ Genre ------ Enum ------ Lex oreder
 * //  SCIENCE_FICTION     0            7
 * //  DRAMA               1            3
 * //  COMEDY              2            0
 * //  CRIME               3            1
 * //  MYSTERY             0            5
 * //  DOCUMENTARY         1            2
 * //  ROMANCE             2            6
 * //  HORROR              3            4
 *
 * @param genre - The genre we want to get its position.
 * @return
 * Integer indicates the position of the genre.
 */
static int getGenrePosition(Genre genre){

    int genres_position[NUMBER_OF_GENRES] = {7,3,0,1,5,2,6,4};
    return genres_position[genre];

}