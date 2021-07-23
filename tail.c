#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1000

int tail(char *buffer[], int n);
int read_line(char *line, int max);
void print_lines(char* buffer[], int n);

static int lines_read = 0;

int main(int argc, char* argv[]) {
  int number_of_lines = 10;
  char c;

  while(--argc > 0 && (*++argv)[0] == '-') {
    while((c = *++argv[0])) {
      switch(c) {
        case 'n':
          if(argc-1 > 0)
            number_of_lines = atoi(*(argv+1));
          break;
      }
    }
  }  

  char *line_buffer[number_of_lines];

  if(tail(line_buffer, number_of_lines) < 0) {
    printf("ERROR: failed to allocate memory for a line.\n");
    return -1;
  }

  printf("Output:\n"); 
  print_lines(line_buffer, number_of_lines);

  return 0;
}

int tail(char *buffer[], int n) {
  int characters_read = 0;
  char *p, line[MAXLEN];

  while((characters_read = read_line(line, MAXLEN)) > 0) {
    if((p = malloc(sizeof(char)*characters_read)) == NULL) {
      return -1;
    }
    strcpy(p, line);
    buffer[lines_read++ % n] = p;
  }

  return 0;
}

int read_line(char *line, int max) {
  int c, chars_read = 0;

  while((c = getchar()) != EOF && c != '\n' &&  ++chars_read < max-1)
    *line++ = c;

  *line = '\0';

  return chars_read;
}

void print_lines(char *buffer[], int n) {
  for(int i=0; i < (lines_read > n ? n : lines_read); i++)
    printf("%s\n", buffer[lines_read > n ? lines_read++ % n : i]);
}
