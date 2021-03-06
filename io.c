#include "io.h"

// Modified from http://stackoverflow.com/questions/19173442/reading-each-line-of-file-into-array
// Original code from willus
char** read_into_array(const char* file_name)
{
  int lines_allocated = 256;
  int max_line_len = 10;
  
  /* Allocate lines of text */
  char **words = (char **)malloc(sizeof(char*)*lines_allocated);
  if (words == NULL)
  {
    fprintf(stderr, "Out of memory (1).\n");
    exit(1);
  }
  
  FILE *fp = fopen(file_name, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "Error opening file %s.\n", file_name);
    exit(2);
  }
  
  int i;
  for (i = 0; 1; i++)
  {
    int j;
    
    /* Have we gone over our line allocation? */
    if (i >= lines_allocated)
    {
      int new_size;
      
      /* Double our allocation and re-allocate */
      new_size = lines_allocated * 2;
      words = (char **)realloc(words,sizeof(char*)*new_size);
      if (words == NULL)
      {
	fprintf(stderr, "Out of memory.\n");
	exit(3);
      }
      lines_allocated = new_size;
    }
    /* Allocate space for the next line */
    words[i] = malloc(max_line_len);
    if (words[i] == NULL)
    {
      fprintf(stderr, "Out of memory (3).\n");
      exit(4);
    }
    if (fgets(words[i], max_line_len - 1, fp) == NULL)
      break;
    
    /* Get rid of CR or LF at end of line */
    for (j = strlen(words[i]) - 1; j >= 0 && (words[i][j] == '\n' || words[i][j] == '\r'); j--)
      ;
    words[i][j + 1] = '\0';
  }
  
  fclose(fp);
  
  return words;
}


// Print the content of an array into a file
void array_to_file(char** data, int size, char* file_name)
{
  FILE *fp = fopen(file_name, "wb");
  if (fp == NULL)
  {
    fprintf(stderr, "Error opening file.\n");
    exit(1);
  }
  
  int i = 0;
  for (i = 0; i < size; i++)
  {
    fprintf(fp, "%s\n", data[i]); 
  }
  
  fclose(fp);
}


// Print the content of an array into a file
// Data inside the array is normalised
void normalised_array_to_file(char** data, int size, char* file_name)
{
  array_to_file(normalise(data, size), size, file_name);
}