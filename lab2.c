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
  int i = 0;
  
  // Open files for writing
  FILE *fp_e = fopen("file_e", "w+");
  if (fp_e == NULL)
  {
    fprintf(stderr, "Error opening file_e.\n");
    exit(2);
  }
  FILE *fp_m = fopen("file_m", "w+");
  if (fp_m == NULL)
  {
    fprintf(stderr, "Error opening file_m.\n");
    exit(2);
  }
  FILE *fp_z = fopen("file_z", "w+");
  if (fp_z == NULL)
  {
    fprintf(stderr, "Error opening file_z.\n");
    exit(2);
  }
  
  // Allocate memory for results
  char** output_data = malloc(100 * sizeof(char*));
  
  // Open all files in the folder given as argument
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
    if (fprintf(fp_e, "%f\n", e[i]) < 0)
    {
      fprintf(stderr, "Error writing to file_e.\n");
      exit(3);
    }
    
    // Calculate magnitude and write it to file
    output_data = magnitude(data, 100, 240);
    m[i] = log_average(output_data, 100);
    if (fprintf(fp_m, "%f\n", m[i]) < 0)
    {
      fprintf(stderr, "Error writing to file_m.\n");
      exit(3);
    }
    
    // Calculate zero-crossing rate and write it to file
    output_data = zcr(data, 100, 240);
    z[i] = average(output_data, 100);
    if (fprintf(fp_z, "%f\n", z[i]) < 0)
    {
      fprintf(stderr, "Error writing to file_z.\n");
      exit(3);
    }
    
    i++;
    // Empty string
    path[0] = 0;
  }

  closedir(mydir);
  fclose(fp_e);
  fclose(fp_m);
  fclose(fp_z);
  
  free(output_data);
  
  return 0;
}