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
    printf("Line[0]: %s\n", tok);

    if(num == 0)
      return tok;

    int i = 1;
    for(i = 1; i <=num; i++) {
	    tok = strtok(NULL, ",");        //doesn't account for empty fields (may crash there)
                                      //if field is empty, account for that; could be indexing into wrong thing
                                      //write own strtok or use strsep-strtok but accounts for NULL fields
      printf("Line[%d]: %s\n", i, tok);
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

      //printf("%s\n", tok);

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

  printf("Entering checkName()\n");

  int i = 0;

  // Loop through struct to find name, return 1 if found
  for(i = 0; i < currSize; i++) {

    //printf("%s    %d\n", arr[i].name, i);

    //printf("checkName() for loop entered\n");

    if(arr[i].name == NULL){               
      printf("Error: NULL value in array");
      return -1;
    }

    if(strcmp(arr[i].name, currName) == 0) {      // FIXME: Segfaults here because when tweet 736 is read in, currName is NULL
      printf("Successful find of name\n");
      return i;
    }
  }
  // If name not found, return 0

  printf("Unsuccessful find, return -1\n");
  return -1;

}


void sortTweeterArray(struct nameCount* arr, int currSize){

  //int x, y;
  int tempTweetCount;
  char* tempName;

  for(int i = 0; i < currSize; i++) {
    for(int j = (i + 1); j < currSize; j++) {
      if(arr[i].tweetCount < arr[j].tweetCount) {
        tempName = arr[i].name;
        tempTweetCount = arr[i].tweetCount;

        arr[i].name = arr[j].name;
        arr[i].tweetCount = arr[j].tweetCount;

        arr[j].name = tempName;
        arr[j].tweetCount = tempTweetCount;

      }

    }
  }

}







int main(int argc, char* argv[]) {//NOTE: Needs to be able to handle edge cases

    FILE* stream = fopen(argv[1], "r"); //Code obtained from https://stackoverflow.com/questions/12911299/read-csv-file-in-c

    // Intialize variables
    int i = 0;
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

    if(line != NULL) {
      // Find column with "name" attribute
      nameCol = getnamefield(line);

      //printf("%d\n", nameCol);

      // If no "name" attribute print error message
      if(nameCol < 0) {
        printf("Error: no nameCol in stream\n");
        return 1;
      }
    }
    else {
      printf("Error: File empty");
      return 1;
    }

    //printf("First Line: %s\n", line);

    //while (fgets(line, 377, stream) && i < 20000)
    //while (fgets(line, 377, stream))
    while (!feof(stream))
    {
            //printf("Testing While Loop 1\n");
      fgets(line, 377, stream);

      printf("\nGetting Line %d", i);
      printf("\n\nLine: %s\n", line); // When fgets reads in line 736, it only reads in "


      if(strcmp(line, "") == 0) {
        printf("Line is empty");
      }

        // Getting column of "name" attribute	
	      currName = getfield(line, nameCol);//SEGFAULTS AT PRINT - RETURNING GARBAGE AT LINE 735 in CSV FILE (Logunov)

        printf("currName: %s\n", currName);//SEFFAULTS HERE IF I < 750!!!!!

        // If no entries, add first and continue to parse

        currNameIndex = checkName(allNameCounts, currName, currSize);//ERROR: SEGFAULTS AT THIS POINT ---- Need to figure out how to run if the array has not even been initialized yet
                                                                     // Segfaults here because currName is NULL for tweet 736

        printf("currNameIndex: %d\n", currNameIndex);
        printf("Ugh\n");

        if(currSize == 0) {
          printf("No tweeters have been added\n");
          //strcpy(allNameCounts[0].name, currName);
          allNameCounts[0].name = currName;
	        allNameCounts[0].tweetCount = 1;
          currSize++;
          i++;
          //continue;
          //printf("CurrSize is 0\n");
          printf("First Tweeter added\n");
          //currNameIndex = checkName(allNameCounts, currName, currSize);//ERROR: SEGFAULTS AT THIS POINT ---- Need to figure out how to run if the array has not even been initialized yet
        } // allNameCounts is empty, add currName

        // Get index of current name in allNameCounts
        //currNameIndex = checkName(allNameCounts, currName, currSize);//ERROR: SEGFAULTS AT THIS POINT ---- Need to figure out how to run if the array has not even been initialized yet
        //first entry name is NULL because not initialized yet


        //printf("Testing While Loop\n");
        
        else if(currNameIndex >= 0) {
          printf("Tweeter count about to be increased");
          allNameCounts[currNameIndex].tweetCount++;
          printf("Tweeter count increased");
        }//if the selected name, currName, already exists in the allNameCount array
        else {
          printf("Needs to add tweeter\n");
          // store name and intialize count
          //currSize++;
          allNameCounts[currSize].name = currName;
          allNameCounts[currSize].tweetCount = 1;
          currSize++;
          printf("Tweeter added\n");
        }

        // printf("Field 3 would be %s\n", getfield(tmp, 3);
        //printf("%s \n", line);
        // NOTE strtok clobbers tmp
        i++;
    }//Code obtained from https://stackoverflow.com/questions/12911299/read-csv-file-in-c

  }
  else {
    printf("Invalid file input.\n");
  }

  printf("\n\n\n\n\n");

  sortTweeterArray(allNameCounts, currSize);

  for(int x = 0; x < 10; x++){
    printf("Array Item %d: %s, %d \n", x, allNameCounts[x].name, allNameCounts[x].tweetCount);
  }
  
  return 0;
}

//length of line can't be longer than longest line

//need to verify if we have the right number of columns
