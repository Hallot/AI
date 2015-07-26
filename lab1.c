// Lab 1

// TO DO
// Determine the Sampling Rate (the samples in the file account for 300 ms of speech)
// Apply the ideal delay operator with delay 5, 10 and 15 ms
// Apply the moving average with k1=k2=5,10 and 15 ms
// Convolve the signal with a window of length 10 ms
// Extract the short-term energy signal from the signal in "laboratory.dat" (window length 30 ms)
// Extract the short-term magnitude signal from the signal in "laboratory.dat" (window length 30 ms)
// Extract the short-term Zero Crossing Rate (ZCR) signal from the signal in "laboratory.dat" (window length 30 ms)
// Plot the original signal, the energy signal, the magnitude signal and the ZCR signal as a function of time

// @input : laboratory.dat -> contains data point from audio file
// @output : print the results to txt files to be plotted by gnuplot


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
  
  // Get the number of elements in the array
  int size = 0;
  while (data[size] != NULL)
  {
    size++;
  }
  size--;
  
  // Print the sampling rate
  // Time range is 300ms
  int sampling_rate = 0;
  sampling_rate = get_sampling_rate(size, 300);
  printf("The sampling rate is %d.\n", sampling_rate);
  
  // Array for the data to be printed
  char** output_data = malloc(size * sizeof(char*));
  
  // Output the normalised data
  normalised_array_to_file(data, size, "data.txt");
  
  //Output the ideal delay
  // 5ms delay
  output_data = ideal_delay(data, size, 5, sampling_rate);
  normalised_array_to_file(output_data, size, "ideal_delay_5.txt");
  // 10ms delay
  output_data = ideal_delay(data, size, 10, sampling_rate);
  normalised_array_to_file(output_data, size, "ideal_delay_10.txt");
  // 15ms delay
  output_data = ideal_delay(data, size, 15, sampling_rate);
  normalised_array_to_file(output_data, size, "ideal_delay_15.txt");
  
  // Output the moving average
  // k1=k2=5ms or 40 data points
  output_data = moving_average(data, size, 40, 40);
 normalised_array_to_file(output_data, size, "moving_average_5.txt");
  // k1=k2=10ms or 80 data points
  output_data = moving_average(data, size, 80, 80);
  normalised_array_to_file(output_data, size, "moving_average_10.txt");
  // k1=k2=15ms or 120 data points
  output_data = moving_average(data, size, 120, 120);
  normalised_array_to_file(output_data, size, "moving_average_15.txt");
  
  // Output the convolution
  // Window length of 10ms, or 80 data points
  output_data = convolution(data, size, 80);
  normalised_array_to_file(output_data, size, "convolution_10.txt");
  
  // Output the energy
  // Window length of 30ms or 240 data points
  output_data = energy(data, size, 240);
  normalised_array_to_file(output_data, size, "energy_30.txt");
  
  // Output the magnitude
  // Window length of 30ms or 240 data points
  output_data = magnitude(data, size, 240);
  normalised_array_to_file(output_data, size, "magnitude_30.txt");
  
  // Output the zero crossing rate
  // Window length of 30ms or 240 data points
  output_data = zcr(data, size, 240);
  normalised_array_to_file(output_data, size, "zcr_30.txt");
  
  free(output_data);
  
  return 0;
}