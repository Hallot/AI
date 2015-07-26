#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char** read_into_array(const char* file_name);
void array_to_file(char** data, int size, char* file_name);
void normalised_array_to_file(char** data, int size, char* file_name);