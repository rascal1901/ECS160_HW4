#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//gcc -lm -g -Wall maxTweeter.c -o maxTweeter

struct nameCount {

  char* name;
  int tweetCount;

};//struct to keep track of a tweeter and number of times he/she tweeted

char* getfield(char* line, int num)//Code obtained from https://stackoverflow.com/questions/12911299/read-csv-file-in-c
{
    char* tempLine = strdup(line);
    //printf("\n\nLine: %s\n", tempLine);

    char* tok = strtok(tempLine, ",");
    //printf("Line[0]: %s\n", tok);

    if(num == 0)
      return tok;

    int i = 1;
    for(i = 1; i <=num; i++) {
	    tok = strtok(NULL, ",");
      //printf("Line[%d]: %s\n", i, tok);
    }
    //printf("%s\n", tok);
    return tok;

}//Code obtained from https://stackoverflow.com/questions/12911299/read-csv-file


int getnamefield(char* line)
{
    // Initialize temp line
    char* tok;
    char* tempLine = strdup(line);

    // Parse line by comma
    tok = strtok(tempLine, ",");
    int col = 0;

    // Find "name" column number
    while(tok != NULL) {
      if(strcmp(tok,"\"name\"") == 0) {//if we find the "name" column in the CSV file
        return col;//return col value to be used in main
      }
     tok = strtok(NULL, ",");
     col++;
    }
    // Invalid file; Does not have "name" column
    return -1;
}//Function to look for the column number that the "names" are in within the CSV file


int checkName(struct nameCount* arr, char* currName, int currSize) {

  int i = 0;

  // Loop through struct array to find name, return index in struct if found
  for(i = 0; i < currSize; i++) {

    if(arr[i].name == NULL){               
      return -1;
    } // If name is null

    if(strcmp(arr[i].name, currName) == 0) {
      //printf("Successful find of name\n");
      return i;
    } // If name is found
  }

  // If name not found, return -1
  return -1;

} // Function to get name of tweeter from line


void sortTweeterArray(struct nameCount* arr, int currSize){

  //int x, y;
  int tempTweetCount;//stores temp value of smaller tweeter's tweet count
  char* tempName;//stores temp value of smaller tweeter's name

  for(int i = 0; i < currSize; i++) {//outer for loop that checks array items from start to finish and indexes the current one with i
    for(int j = (i + 1); j < currSize; j++) {//inner for loop that checks all array items following the one indexed by the first for loop
      if(arr[i].tweetCount < arr[j].tweetCount) {//if the succeeding tweeter has a higher tweet count than the current one (i)
        tempName = arr[i].name;//store current tweeter name in temp name string
        tempTweetCount = arr[i].tweetCount;//store current tweeter TweetCount in a temp int variable

        //switch values between array[i] and array[j]
        arr[i].name = arr[j].name;//store larger tweeter's name at index i further up the top of the array 
        arr[i].tweetCount = arr[j].tweetCount;//store larger tweeter's Tweet count at index i further up the top of the array

        arr[j].name = tempName; //store smaller tweeter's name at index j 
        arr[j].tweetCount = tempTweetCount;//store smaller tweeter's tweet count at index j of array

      }
    }
  }
} // Function to sort tweets in decreasing order; people with largest tweet counts are on top, people with smaller tweet counts are further down array

int main(int argc, char* argv[]) {//NOTE: Needs to be able to handle edge cases

  // Make sure file is .csv file
  char* filename = argv[1];
  int len = strlen(filename);
  const char *last_four = &filename[len-4];
  if(strcmp(last_four, ".csv") != 0) {
    printf("Invalid Input Format.\n");
    return 1;
  } // Source: https://stackoverflow.com/questions/5297248/how-to-compare-last-n-characters-of-a-string-to-another-string-in-c#5297278

  // Create file stream from input filename
  FILE* stream = fopen(argv[1], "r"); //Code obtained from https://stackoverflow.com/questions/12911299/read-csv-file-in-c

  if(stream == NULL) {
    printf("Error: Unable to open file.\n");
    return 1;
  }

  // Intialize variables
  //int i = 0;
  int nameCol;
  char line[377];
  struct nameCount allNameCounts[20000];//20000
  char* currName;
  int currNameIndex = 0;
  int currSize = 0;

  // if user enter correct input
  if(argc == 2) {

    // Get first line
    fgets(line, 377, stream);

    // Find column with "name" attribute
    nameCol = getnamefield(line);

    // If no "name" attribute print error message
    if(nameCol < 0) {
      printf("Error: No \"name\" attribute found.\n");
      return 1;
    }

    // Until end of file
    while (!feof(stream))
    {
      fgets(line, 377, stream);

      if(strcmp(line, "") == 0) {
        printf("Line is empty");
      }

        // Getting column of "name" attribute	
	      currName = getfield(line, nameCol);

        // Get index of name in allNameCounts
        currNameIndex = checkName(allNameCounts, currName, currSize);

        if(currSize == 0) {
          allNameCounts[0].name = currName;
	        allNameCounts[0].tweetCount = 1;
          currSize++;
        } // allNameCounts is empty, add currName
        else if(currNameIndex >= 0) {
          allNameCounts[currNameIndex].tweetCount++;
        }//if the selected name, currName, already exists in the allNameCount array
        else {
          allNameCounts[currSize].name = currName;
          allNameCounts[currSize].tweetCount = 1;
          currSize++;
        }
    }//Code obtained from https://stackoverflow.com/questions/12911299/read-csv-file-in-c

  }
  else {
    printf("Invalid Input Format.\n");
  } // argc != 2

  // Sort array of tweeter and counts
  sortTweeterArray(allNameCounts, currSize);

  // Print top ten
  for(int x = 0; x < 10; x++){
    printf("%s, %d \n", allNameCounts[x].name, allNameCounts[x].tweetCount);
  }
  
  return 0;
}

//length of line can't be longer than longest line

//need to verify if we have the right number of columns
