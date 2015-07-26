// Lab 2

// TO DO
// Calculate E, the logarithm of the average value of the short-term energy signal
// Calculate M, the hogarithm of the average value of the short-term magnitude signal
// Calculate Z, the average value of the Zero Crossing Rate signal
// Produce three scatter plots, showing all the audio signals, using the following three combinations of features:
// 'E' versus 'M', 'E' versus 'Z', 'M' versus 'Z'
// Build a classifier
// Test iy by computing the average classification accuracy of the K-fold validation procedure

// @input : the path to the directory with the audio files
// @output : 3 files containing E, M and Z

#include <sys/types.h>
#include <dirent.h>
#include "io.h"
#include "signal_caracteristics.h"




// Training set : 10 random files but % kept, 5 sound and 5 silent
// Total set : 100 -> testing set : 90 files = 9 * 10
// Caracteristics : E > 24, M > 15, Z < 0.1
// Let's cll them E and ¬E, M and ¬M, Z and ¬Z
// I don't really know how to choose those values so I chose ones that seemed to be good cut of points.
// I don't know either if 10 is enough, or what value I should choose

// Carac           E   |    ¬E    ||   M   |     ¬M    ||    Z   |    ¬Z    | Total
//              ___________________________________________________________________
// Sound       |   5   |     0    ||   5   |     0     ||    1   |     4    |   5  |
// Silence     |   0   |     5    ||   0   |     5     ||    0   |     5    |   5  |
//             ____________________________________________________________________|
// Total       |   5   |     5    ||   5   |     5     ||    1   |     9    |  10  |
//             |___________________________________________________________________|

// Prior probabilities and base rates
// P(Sound) = 0.5
// P(Silence) = 0.5

// Probability of "evidence"
// P(E) = 0.5
// P(M) = 0.5
// P(Z) = 0.1

// Probability of "likelihood"
// P(E/Sound) = 1
// P(M/Sound) = 1
// P(Z/Sound) = 0.2
// P(E/Silence) = 0
// P(M/Silence) = 0
// P(Z/Silence) = 0

#define CUT_E 24.0
#define CUT_M 15.0
#define CUT_Z 0.1


// Update base values and related conditional probabilities
void update_values(int n_total, int so[][2], int si[][2], double P_So[][2], double P_Si[][2])
{
  P_So[0][0] = so[0][0] / n_total;
  P_So[0][1] = so[0][1] / n_total;
  P_So[1][0] = so[1][0] / so[0][0];
  P_So[1][1] = so[1][1] / so[0][0];
  P_So[2][0] = so[2][0] / so[0][0];
  P_So[2][1] = so[2][1] / so[0][0];
  P_So[3][0] = so[3][0] / so[0][0];
  P_So[3][1] = so[3][1] / so[0][0];

  P_Si[0][0] = si[0][0] / n_total;
  P_Si[0][1] = si[0][1] / n_total;
  P_Si[1][0] = si[1][0] / si[0][0];
  P_Si[1][1] = si[1][1] / si[0][0];
  P_Si[2][0] = si[2][0] / si[0][0];
  P_Si[2][1] = si[2][1] / si[0][0];
  P_Si[3][0] = si[3][0] / si[0][0];
  P_Si[3][1] = si[3][1] / si[0][0];
}

// In this function, we don't calculate P(evidence) = P(E).P(M).P(Z) on purpose
// It has no value since both p1 and p2 will be divided by it
// Return 1 if sound and 0 if silence
// Update the base values at the same time
int return_category(double e, double m, double z, int n_total, int so[][2], int si[][2], double P_So[][2], double P_Si[][2])
{
  double p1;
  double p2;
  
  // 0 if above (below for z) the chosen cut off rate and 1 otherwise
  int te = e < CUT_E;
  int tm = m < CUT_M;
  int tz = z > CUT_Z;
  
  // Calculate P(Sound/E,M,Z) and P(Silence/E,M,Z)
  p1 = P_So[0][0] * P_So[1][te] * P_So[2][tm] * P_So[3][tz];
  p2 = P_Si[0][0] * P_Si[1][te] * P_Si[2][tm] * P_Si[3][tz];

  //Update the base values
  n_total++;
  
  // The higher probability is the classification for this file
  if (p1 > p2)
  {
    // This is a sound file
    so[0][0]++;
    si[0][1]++; // Useless as this value is not used, but keep consistency
    so[1][te]++;
    so[2][tm]++;
    so[3][tz]++;
  }
  else
  {
    // This is a silent file
    si[0][0]++;
    so[0][1]++; // Useless as this value is not used, but keep consistency
    si[1][te]++;
    si[2][tm]++;
    si[3][tz]++;
  }
  
  return p1 > p2;
}



// Entry point
// Arg is path to folder with data files
int main (int argc, char** argv)
{
  if (argc != 2)
  {
    printf("Usage : prog <folder>\n");
    return -1;
  }
   
  double e[100];
  double m[100];
  double z[100];
  int classification[100];
  char* result = "sound";
  int i = 0;
  
  // Variables
  int n_total = 10;

  // 2-dim array containing number information
  // schema (same for Si)
  //   _______________________________________________
  //  |   n_So   ||   n_E   |    n_M    |     n_Z     |
  //  |   n_¬So  ||   n_¬E  |    n_¬M   |     n_¬Z    |
  //  |_______________________________________________|

  int so[4][2];
  int si[4][2];

  so[0][0] = 5;
  so[0][1] = 5;
  so[1][0] = 5;
  so[1][1] = 0;
  so[2][0] = 5;
  so[2][1] = 0;
  so[3][0] = 1;
  so[3][1] = 4;

  si[0][0] = 5;
  si[0][1] = 5;
  si[1][0] = 0;
  si[1][1] = 5;
  si[2][0] = 0;
  si[2][1] = 5;
  si[3][0] = 1;
  si[3][1] = 4;


  // 2-dim array containing probability information
  // schema (same for Si)
  //   ________________________________________________
  //  |   P(So)   || P(E/So)  | P(M/So)  |   P(Z/So)   |
  //  |   P(¬So)  || P(¬E/So) | P(¬M/So) |   P(¬Z/So)  |
  //  |________________________________________________|

  double P_So[4][2];
  double P_Si[4][2];


  
  // Create the base probabilities for the classifier
  update_values(n_total, so, si, P_So, P_Si);
  
  // Open file for writing results
  FILE *fp = fopen("results.txt", "w+");
  if (fp == NULL)
  {
    fprintf(stderr, "Error opening results.txt.\n");
    exit(2);
  }
  
  // Allocate memory for results
  char** output_data = malloc(100 * sizeof(char*));
  
  // Open all files in the folder given as argument
  // Order of files can be considered as random
  DIR *mydir = opendir(argv[1]);
  struct dirent *entry = NULL;
  
  // Loop through the files
  while((entry = readdir(mydir)))
  {
    //If . or .., skip them (not data)
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
    {
      continue;
    }
    
    // Create the path for each file
    char path[100];
    strcat(path, argv[1]);
    strcat(path, entry->d_name);
    
    // Read file into array
    char** data = read_into_array(path);
    
    // Calculate energy and write it to file
    output_data = energy(data, 100, 240);
    e[i] = log_average(output_data, 100);
    
    // Calculate magnitude and write it to file
    output_data = magnitude(data, 100, 240);
    m[i] = log_average(output_data, 100);
    
    // Calculate zero-crossing rate and write it to file
    output_data = zcr(data, 100, 240);
    z[i] = average(output_data, 100);
    
    
    // Check if we have finished a batch of 10 files
    if (i % 10 == 0)
    {
      // Update probabilities
      update_values(n_total, so, si, P_So, P_Si);
    }
    
    // Get the classification of the current file from the classifier
    classification[i] = return_category(e[i], m[i], z[i], n_total, so, si, P_So, P_Si);
    
    // Change the int into a string
    if (classification[i])
    {
      result = "silence";
    }
    else
    {
      result = "sound";
    }
    
    // Write the result in a file
    if (fprintf(fp, "File %s has been classified as %s\n", entry->d_name, result) < 0)
    {
      fprintf(stderr, "Error writing to results.txt.\n");
      exit(3);
    }
    
    i++;
    // Empty string
    path[0] = 0;
  }

  closedir(mydir);
  fclose(fp);
  
  free(output_data);
  
  return 0;
}