#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct nameCount {

  char* name;
  int tweetCount;

};//struct to keep track of a tweeter and number of times he/she tweeted

char* getfield(char* line, int num)//Code obtained from https://stackoverflow.com/questions/12911299/read-csv-file-in-c
{
 
    char* tempLine = strdup(line);
    printf("%s\n", tempLine);

    char* tok = strtok(tempLine, ",");

    if(num == 0)
      return tok;

    int i = 1;
    for(i = 1; i <=num; i++) {
	tok = strtok(NULL, ",");
    }
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

      printf("%s\n", tok);

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

            printf("Testing checkName Loop\n");



  // Loop through struct to find name, return 1 if found
  for(i = 0; i < currSize; i++) {

    printf("%s    %d\n", arr[i].name, i);

    if(strcmp(arr[i].name, currName) == 0) {

      printf("Successful find of name\n");
      return i;
    }
  }
  // If name not found, return 0

  printf("Unsuccessful find, return -1\n");
  return -1;

}



void main(int argc, char* argv[]) {//NOTE: Needs to be able to handle edge cases

  // if user enter correct input
  if(argc == 2) {

    FILE* stream = fopen(argv[1], "r"); //Code obtained from https://stackoverflow.com/questions/12911299/read-csv-file-in-c

    // Intialize variables
    int i = 0;
    int nameCol;
    char line[374];
    struct nameCount allNameCounts[20000];
    char* currName;
    int currNameIndex = 0;
    int currSize = 0;

    // Get first line
    fgets(line, 374, stream);
    // Find column with "name" attribute
    nameCol = getnamefield(line);

    printf("%d\n", nameCol);

    // If no "name" attribute print error message
    if(nameCol < 0) {
      printf("Error: no nameCol in stream\n");
      return;
    }

    printf("First Line: %s\n", line);

    while (fgets(line, 374, stream) && i < 10)
    {
            printf("Testing While Loop 1\n");

        // Getting column of "name" attribute	
	currName = getfield(line, nameCol);

            printf("Testing While Loop 2\n");

        printf("%s\n", currName);

        // If no entries, add first and continue to parse
        if(currSize == 0) {
          //strcpy(allNameCounts[0].name, currName);
          allNameCounts[0].name = currName;
	  allNameCounts[0].tweetCount = 1;
          currSize++;
          i++;
          continue;
        }

        // Get index of current name in allNameCounts
        currNameIndex = checkName(allNameCounts, currName, currSize);//ERROR: SEGFAULTS AT THIS POINT ---- Need to figure out how to run if the array has not even been initialized yet
//first entry name is NULL because not initialized yet


        printf("Testing While Loop\n");
        
        if(checkName(allNameCounts, currName, currSize) >= 0) {
          allNameCounts[currNameIndex].tweetCount++;
        }//if the selected name, currName, already exists in the allNameCount array
        else {
          // store name and intialize count
          currSize++;
          allNameCounts[currSize].name = currName;
          allNameCounts[currSize].tweetCount = 1;

        }
        
        // printf("Field 3 would be %s\n", getfield(tmp, 3);
        printf("%s \n", line);
        // NOTE strtok clobbers tmp
        i++;
    }//Code obtained from https://stackoverflow.com/questions/12911299/read-csv-file-in-c

  }
  else {
    printf("Invalid input. Usage: maxTweeter <FILENAME>\n");
  }

}

//length of line can't be longer than longest line

//need to verify if we have the right number of columns
