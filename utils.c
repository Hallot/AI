#include "utils.h"

// Normalise data
// Naive implementation
char** normalise(char** data, int size)
{
  int i;
  double max = atof(data[0]);
  double temp;
  char** normalised_data = malloc(size * sizeof(char*));
  // Find the maximum value
  for (i = 0; i < size; i++)
  {
    temp = atof(data[i]);
    if (fabs(temp) > max)
    {
      max = temp;
    }
  }
  // Normalise
  for (i = 0; i < size; i++)
  {
    normalised_data[i] = malloc(sizeof(char*));
    temp = atof(data[i]);
    temp /= max;
    sprintf(normalised_data[i], "%f", temp);
  }
  
  return normalised_data;
}
// Sign function
// Return 1 if argument is > 0, otherwise 0
int sign(int n)
{
  return n >= 0 ? 1 : 0;
}


// Average
double average(char ** data, int size)
{
  double sum = 0;
  int i;
  for (i = 0; i < size; i++)
  {
    sum += atof(data[i]);
  }
  return sum;
}


// Log of the average
double log_average(char ** data, int size)
{
  double sum = 0;
  int i;
  for (i = 0; i < size; i++)
  {
    sum += atof(data[i]);
  }
  return log(sum);
}