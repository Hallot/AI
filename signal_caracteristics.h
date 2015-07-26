#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

int get_sampling_rate(int size, int time_range);
char** ideal_delay(char** data, int size, int delay, int sampling_rate);
char** moving_average(char** data, int size, int k1, int k2);
char** convolution(char** data, int size, int length);
char** energy(char** data, int size, int length);
char** magnitude(char** data, int size, int length);
char** zcr(char** data, int size, int length);
