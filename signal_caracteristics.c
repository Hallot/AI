#include "signal_caracteristics.h"

// Calculate the sampling rate
// time_range in ms
int get_sampling_rate(int size, int time_range)
{
  return (size * 1000) / time_range;
}


// Ideal delay function
// Output data is array of value
// delay in ms
char** ideal_delay(char** data, int size, int delay, int sampling_rate)
{
  char** ideal_data = malloc(size * sizeof(char*));
  int n0 = delay * sampling_rate / 1000;
  int i = 0;
  for (i = 0; i < size; i++)
  {
    if (i - n0 < 0)
    {
      ideal_data[i] = "0";
    }
    else
    {
      ideal_data[i] = data[i - n0];
    }
    
  }
  return ideal_data;
}


// Moving average
char** moving_average(char** data, int size, int k1, int k2)
{
  char** movaverage_data = malloc(size * sizeof(char*));
  int i;
  int j;
  double sum = 0;
  for (i = 0; i < size; i++)
  {
    movaverage_data[i] = malloc(sizeof(char*));
    for (j = i - k1; j <= i + k2; j++)
    {
      if (j >= 0 && j < size)
      {
	sum += atof(data[j]);
      }
    }
    sum = sum / (k1 + k2 + 1);
    sprintf(movaverage_data[i], "%f", sum);
    sum = 0;
  }
  
  return movaverage_data;
}


// Convolution
char** convolution(char** data, int size, int length)
{
  char** convolution_data = malloc(size * sizeof(char*));
  int i;
  int j;
  double sum = 0;
  for (i = 0; i < size; i++)
  {
    convolution_data[i] = malloc(sizeof(char*));
    // If within the window
    for (j = i - length + 1; j <= i; j++)
    {
      if (j >= 0)
      {
	sum += atof(data[j]);
      }
    }
    sprintf(convolution_data[i], "%f", sum);
    sum = 0;
  }
  
  return convolution_data;
}


// Short-term energy signal
// Watch out for integer overflow !!!!
char** energy(char** data, int size, int length)
{
  char** energy_data = malloc(size * sizeof(char*));
  int i;
  int j;
  double sum = 0;
  for (i = 0; i < size; i++)
  {
    energy_data[i] = malloc(sizeof(char*));
    // If within the window
    for (j = i - length + 1; j <= i; j++)
    {
      if (j >= 0)
      {
	sum += atof(data[j]) * atof(data[j]);
      }
    }
    sprintf(energy_data[i], "%f", sum);
    sum = 0;
  }
  
  return energy_data;
}


// Short-term magnitude signal
char** magnitude(char** data, int size, int length)
{
  char** magnitude_data = malloc(size * sizeof(char*));
  int i;
  int j;
  double sum = 0;
  for (i = 0; i < size; i++)
  {
    magnitude_data[i] = malloc(sizeof(char*));
    // If within the window
    for (j = i - length + 1; j <= i; j++)
    {
      if (j >= 0)
      {
	sum += fabs(atof(data[j]));
      }
    }
    sprintf(magnitude_data[i], "%f", sum);
    sum = 0;
  }
  
  return magnitude_data;
}


// Zero crossing rate
char** zcr(char** data, int size, int length)
{
  char** zcr_data = malloc(size * sizeof(char*));
  int i;
  int j;
  double sum = 0;
  for (i = 0; i < size; i++)
  {
    zcr_data[i] = malloc(sizeof(char*));
    // If within the window
    for (j = i - length + 1; j <= i; j++)
    {
      if (j > 0)
      {
	// sign returns 1 if argument is > 0, otherwise 0
	sum += fabs(sign(atoi(data[j])) - sign(atoi(data[j - 1])));
      }
    }
    // 300ms x 8 samples per s => N = 2400
    // Hardcoded value
    sum /= 2 * 2400;
    sprintf(zcr_data[i], "%f", sum);
    sum = 0;
  }
  
  return zcr_data;
}