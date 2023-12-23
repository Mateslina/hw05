#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/*
    Defining structures:
    - Date: year, month, day for each review
    - Review: date, rating, comment
*/
typedef struct Date
{
    int year;
    int month;
    int day;
} Date;

typedef struct Review
{
    Date date;
    int rating;
    char *comment;
} Review;

//declaring function prototypes

/*
    Checks if a year is a leap year
    Parameter:
    - (int) year
    Returns:
    - (bool) true: if the year is a leap year
    - (bool) false: if the year is not a leap year
*/
bool isLeap(int year);

/*
    Sets the lenght of the month based on if the year is leap or not
    Parameter:
    - (int) year
    - (int) month
    Returns:
    - (int) days_in_month: The corresponding number of days in a month
*/
int setMonthLenght(int year, int month);

/*
    Checks if a given date is valid mostly accroding to the Gregorian calendar
    and the rules of progtest
    Parameters:
    - (Date) date: The structure of a date (year,month,day)
    Returns:
    - (bool) true: if the date is valid
    - (bool) false: if the date is not valid 
*/
bool dateIsValid(Date date);

/*
    Compares two dates based on which comes before the other
    Parameters:
    - (Date) first_date: The structure of the first date (year,month,day)
    - (Date) second_date: The structure of the second date (year,month,day)
    Returns:
    - (bool) true: if second_date comes before first_date
    - (bool) false: if first_date comes before second_date
*/
bool compareDates(Date first_date, Date second_date);

/*
    Calculating the average of review rating in a given interval
    Parameters:
    - (const Review *) reviews: An array of reviews
    - (int) start_index: The start index of the interval we want to calculate
    - (int) end_index: The end index of the interval we want to calculate
    Returns:
    - (double) sum/length: The sum of all the rating in the interval divided by its length (== average) 
*/
double getAverage(const Review *reviews, int start_index, int end_index);

/*
    Auxiliary compare function for quicksort
    Parameters:
    - (const void*) first_rating
    - (const void*) second_rating
    Returns:
    - (int) positive: if first_rating is bigger than second_rating
    - (int) negative: if first_rating is smaller than second_rating
    - (int) 0: if they are equal
*/
int compareRatings(const void *first_rating, const void *second_rating);

/*
    Function to calculate the median of ratings in a given interval
    Parameters:
    - (const Review*) reviews: An array of reviews
    - (int) start_index: The start index of the interval we want to calculate
    - (int) end_index: The end index of the interval we want to calculate
    Returns:
    - (int) median
*/
int getMedian(const Review *reviews, int start_index, int end_index);

/*
    Checks whether two dates are the same
    Parameters:
    - (Date) first_date: The structure of the first date (year,month,day)
    - (Date) second_date: The structure of the second date (year,month,day)
    Returns:
    - (bool) true: if the dates are the same
    - (bool) false: if the dates are not the same
*/
bool datesAreEqual(Date first_date, Date second_date);

/*
    Function to print the desired interval of reviews
    Parameters:
    - (Review *) reviews: An array of reviews
    - (int) start_index: The start index of the interval we want to print
    - (int) end_index: The end index of the interval we want to print
    - (double) average: The average rating of the interval
    - (int) median: The median rating of the interval
    - (char) action: The action the user wants to perform ('?' or '#')
    Returns:
    - (void) prints the desired interval of reviews based on the action ? or #
*/
void printInterval(Review *reviews, int start_index, int end_index, double average, int median, char action);

/*
    Function to get the desired interval of reviews based on the user input
    Parameters:
    - (Review *) reviews: An array of reviews
    - (size_t) reviews_count: The number of reviews
    - (char) action: The action the user wants to perform ('?' or '#')
    - (int) search_length: The length of the interval the user wants to find
    - (int *) start_indexes: An array of indexes of the first review of each date block
    - (int *) end_indexes: An array of indexes of the last review of each date block
    - (size_t) date_blocks: The number of date blocks
    Returns:
    - (void) prints the desired interval of reviews based on the action ? or #
*/
void findInterval(Review *reviews, size_t reviews_count, char action, int search_length, int * start_indexes, int * end_indexes, size_t date_blocks);

/*
    Function to add a new review to the array of reviews
    Parameters:
    - (Review **) reviews: A pointer to the array of reviews
    - (size_t *) reviews_count: A pointer to the number of reviews
    - (int **) end_indexes: A pointer to the array of indexes of the last review of each date block
    - (int **) start_indexes: A pointer to the array of indexes of the first review of each date block
    - (size_t *) date_blocks: A pointer to the number of date blocks
    Returns:
    - (bool) true: if the review was added successfully
    - (bool) false: if the review was not added successfully
*/
bool addReview(Review **reviews, size_t *reviews_count, int **end_indexes, int **start_indexes, size_t *date_blocks, bool *input_valid);

//main function
int main()
{
    //initializing the array of reviews and its size
    Review *reviews = (Review *)malloc(1 * sizeof(Review));
    size_t reviews_count = 0; 

    //initializing arrays to keep track of the indexes of the first and last review of each date block
    int * end_indexes = (int *)malloc(1 * sizeof(int));
    end_indexes[0] = 0;
    int * start_indexes = (int *)malloc(1 * sizeof(int));
    start_indexes[0] = 0;

    size_t date_blocks = 0; //number of blocks

    //initializing the input variables
    char action;
    int search_length = 0;
    bool input_valid = true;

    //reading the input
    printf("Recenze:\n");
    while (true)
    {   
        //reading the action and checking if it is valid
        if (scanf(" %c", &action) != 1)
        {
            break;
        }
        //performing the action "+" - adding a new review
        if (action == '+')
        {
            //if the review was not added successfully, we break the loop
            if(!addReview(&reviews, &reviews_count, &end_indexes, &start_indexes, &date_blocks, &input_valid)){
                break;
            }
        }
        //checking for search actions
        else if ((action == '?' || action == '#') && reviews_count > 0)
        {
            //reading the interval search length and checking if it is valid
            if (scanf(" %d", &search_length) != 1 || search_length < 1)
            {
                input_valid = false;
                break;
            }
            //performing the search of the desired interval
            findInterval(reviews, reviews_count, action, search_length, start_indexes, end_indexes, date_blocks);
        }
        //if the action is anything else than +, ? or #, we break the loop
        else{
            input_valid = false;
            break;
        }
    }

    //if the input was not valid, we print an error message
    if (!input_valid)
    {
        printf("Nespravny vstup.\n");
    }

    //cleaning up the memory 
    for (size_t i = 0; i < reviews_count; i++)
    {
        free(reviews[i].comment);
    }

    free(reviews);
    free(end_indexes);
    free(start_indexes);

    return 0;
}

//function definitions

bool isLeap(int year)
{
    if ((year % 4 == 0 && year % 100 != 0 && year % 4000 != 0) || (year % 400 == 0 && year % 4000 != 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int setMonthLenght(int year, int month)
{
    const int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 2 && isLeap(year))
    {
        return days_in_month[month] + 1; // February in a leap year
    }

    return days_in_month[month];
}

bool dateIsValid(Date date)
{
    if (date.year < 1800 || date.year > 3000 || date.month < 1 || date.month > 12 ||
        date.day < 1 || date.day > setMonthLenght(date.year, date.month))
    {
        return false;
    }
    return true;
}

bool compareDates(Date first_date, Date second_date)
{
    if ((second_date.year < first_date.year) ||
        (second_date.year == first_date.year && second_date.month < first_date.month) ||
        (second_date.year == first_date.year && second_date.month == first_date.month && second_date.day < first_date.day))
    {
        return true;
    }
    else
    {
        return false;
    }
}

double getAverage(const Review *reviews, int start_index, int end_index)
{   
    double sum = 0;
    int length = end_index - start_index + 1;
    
    //adding up all ratings
    for (int i = start_index; i <= end_index; i++)
    {   
        sum += reviews[i].rating;
    }

    return (sum / length);
}

int compareRatings(const void *first_rating, const void *second_rating)
{
    return *(int *)first_rating - *(int *)second_rating;
}

int getMedian(const Review *reviews, int start_index, int end_index)
{
    //initializing a temporary array of ratings to save memory
    size_t count = end_index - start_index + 1;
    int *ratings = (int *)malloc(count * sizeof(int));
    if (ratings == NULL){
        return 1;
    }
    for (size_t i = 0; i < count; i++)
    {
        ratings[i] = reviews[start_index + i].rating;
    }

    //sorting the ratings using built-in quick sort
    qsort(ratings, count, sizeof(int), compareRatings);

    //getting the actual median 
    int median;
    if (count % 2 == 0)
    {
        median = ratings[count / 2 - 1];
    }
    else
    {
        median = ratings[count / 2];
    }
    
    //clean-up
    free(ratings);

    return median;
}

bool datesAreEqual(Date first_date, Date second_date)
{
    if (first_date.year == second_date.year && first_date.month == second_date.month && first_date.day == second_date.day)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void printInterval(Review *reviews, int start_index, int end_index, double average, int median, char action)
{
    if (action == '?')
    {
        printf("%d-%02d-%02d - %d-%02d-%02d: %lf %d\n", reviews[start_index].date.year, reviews[start_index].date.month, reviews[start_index].date.day,
               reviews[end_index].date.year, reviews[end_index].date.month, reviews[end_index].date.day, average, median);
        for (int i = start_index; i <= end_index; i++)
        {
            printf("  %d: %s\n", reviews[i].rating, reviews[i].comment);
        }
    }
    else
    {
        printf("%d-%02d-%02d - %d-%02d-%02d: %lf %d\n", reviews[start_index].date.year, reviews[start_index].date.month, reviews[start_index].date.day,
               reviews[end_index].date.year, reviews[end_index].date.month, reviews[end_index].date.day, average, median);
    }
}

void findInterval(Review *reviews, size_t reviews_count, char action, int search_length, int * start_indexes, int * end_indexes, size_t date_blocks)
{
    //check if there are even enough reviews to find the desired interval
    if (reviews_count < (size_t)search_length)
    {
        printf("Neexistuje.\n");
        return;
    }
    //initializing variables
    int current_interval_end = 0; //to keep track where the current block of reviews ends
    int current_block = 0; //to keep track on which block we are

    //initializing variables for the current interval
    int current_start = 0;
    int current_end = 0;
    double current_average;
    int current_median;
    double current_difference;
    int current_length = 0;
    Date current_end_date = reviews[0].date;

    //initializing variables for the best interval
    int best_start = 0;
    int best_end = 0;
    double best_average = 0;
    int best_median = 0;
    double best_difference = 0;
    int best_length = 0;
    Date best_end_date = reviews[0].date;

    //iterating through all possible intervals
    while(current_block <= (int)date_blocks ){

        //setting where the current interval ends and its length
        current_end = end_indexes[current_interval_end];
        current_length = current_end - current_start + 1;

        //checking if it even makes sense to search for the interval
        //if the current interval is shorter than the desired interval, we can skip it
        if(current_length >= search_length){

            //calculating the average, median and their difference
            current_average = getAverage(reviews, current_start, current_end);
            current_median = getMedian(reviews, current_start, current_end);
            current_difference = abs(current_average - current_median);

            //setting the end date of the current interval
            current_end_date = reviews[current_end].date;

            /* 
                Checking if the current interval is better than the "yet" best one
                - if the difference between the average and the median is bigger
                - if the difference is the same, we check if the end date is bigger
                - if the end date is the same, we check if the length is bigger
            */   
            if (current_difference > best_difference || 
               (current_difference == best_difference && (compareDates(current_end_date, best_end_date) ||
               (datesAreEqual(current_end_date,best_end_date) && (current_length > best_length)))))
            {
                //updating the best interval
                best_difference = current_difference;
                best_start = current_start;
                best_end = current_end;
                best_average = current_average;
                best_median = current_median;
                best_length = current_length;
                best_end_date = current_end_date;
            }
        }

        //iterating to the next interval
        current_interval_end ++;

        //if we reached the end of the current block, we move to the next one
        if(current_end == (int)reviews_count - 1){
            current_block += 1;
            //if we reached the end of the date blocks, we stop
            if(current_block > (int)date_blocks){
                break;
            }
            //setting the start and end of the next block
            current_interval_end = current_block;
            current_start = start_indexes[current_block];
        }
    }
    //printing the desired interval
    printInterval(reviews, best_start, best_end, best_average, best_median, action);
}

bool addReview(Review **reviews, size_t *reviews_count, int **end_indexes, int **start_indexes, size_t *date_blocks, bool *input_valid){
    //new review that will be read and temporary array for its comment
    Review new_review; 
    char temp_comment[4097];
    
    //checking if the read date and rating are valid
    if (scanf(" %d-%d-%d %d", &new_review.date.year, &new_review.date.month, &new_review.date.day, &new_review.rating) != 4)
    {
        *input_valid = false;
        return false;
    }

    //if the input is valid
    else{
        //reading the comment 
        if (scanf(" %4096s", temp_comment) != 1)
        {
            *input_valid = false;
            return false;
        }

        //allocating memory for the actual comment and saving it
        new_review.comment = (char *)malloc((strlen(temp_comment) + 1) * sizeof(char));
        if (new_review.comment == NULL)
        {
            free(new_review.comment);
            return false;
        }
        strcpy(new_review.comment, temp_comment);

        //checking if the date and rating are valid 
        if (!dateIsValid(new_review.date) || new_review.rating < 1)
        {
            *input_valid = false;
            free(new_review.comment);
            return false;
        }

        //adding the review to the array after making it bigger
        *reviews = (Review *)realloc(*reviews, (*reviews_count + 1) * sizeof(Review));
        if (*reviews == NULL)
        {
            free(new_review.comment);
            return false;
        }
        (*reviews)[*reviews_count] = new_review;

        //checking if there are at least two reviews to compare
        if (*reviews_count > 0){

            //if the new review is older than the previous one its not a valid input
            if (compareDates((*reviews)[*reviews_count - 1].date, new_review.date))
            {
                *input_valid = false;
                free(new_review.comment);
                return false;
            }
            //increasing the end index of a block if we are on the same date
            else if (datesAreEqual((*reviews)[*reviews_count - 1].date,new_review.date))
            {
                (*end_indexes)[*date_blocks] += 1;
            }
            //adding a new reviews block if the new review is on a new date
            else{
                //making the index arrays bigger
                *end_indexes = (int *)realloc(*end_indexes, (*date_blocks + 2) * sizeof(int));
                if (*end_indexes == NULL)
                {
                    free(new_review.comment);
                    return false;
                }
                *start_indexes = (int *)realloc(*start_indexes, (*date_blocks + 2) * sizeof(int));
                if (*start_indexes == NULL)
                {
                    free(new_review.comment);
                    return false;
                }
                //setting the indexes of the new block and increasing the number of blocks
                (*end_indexes)[*date_blocks + 1] = 1 + (*end_indexes)[*date_blocks];
                (*start_indexes)[*date_blocks + 1] = (*end_indexes)[*date_blocks] + 1;
                *date_blocks += 1;
            }
        }
        //increase the number of reviews and continue the reading
        *reviews_count += 1;
    }
    return true;
}