#include "io.h"
#include "signal_caracteristics.h"


// Entry point
// Arg is data file name
int main (int argc, char** argv)
{
  if (argc != 2)
  {
    printf("Usage : prog <file>\n");
    return -1;
  }
  
  // Read file into array
  char** data = read_into_array(argv[1]);
  
  // Allocate memory for results
  char** output_data = malloc(100 * sizeof(char*));
  
  // Get the number of elements in the array
  int size = 0;
  while (data[size] != NULL)
  {
    size++;
  }
  size--;
  
  // Calculate energy and print it
  output_data = energy(data, 100, 240);
  printf("%f\n", log_average(output_data, 100));
  
  // Calculate magnitude and print it
  output_data = magnitude(data, 100, 240);
  printf("%f\n", log_average(output_data, 100));
  
  // Calculate zcr and print it
  output_data = zcr(data, 100, 240);
  printf("%f\n", average(output_data, 100));
  
  free(output_data);
  
  return 0;
}