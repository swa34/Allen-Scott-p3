#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFSIZE 4096
#define N_LINES 10
#define TMPFILE "/tmp/stdin_tmpf.bin"

void oops(char *s1, char *s2) {
  fprintf(stderr, "Error: %s\n(errno) ", s1);
  perror(s2);
  exit(1);
}

unsigned count_chars(const char *str, char byte, int n_chars) {
  int cnt = 0;
  for (int i = 0; i < n_chars; ++i)
	if (str[i] == byte)
	  ++cnt;

  return cnt;
}

unsigned find_cutoff(int fd) {
  int linecnt, n_chars;
  int subpos, block;
  char cbuf[BUFSIZE];
  subpos = block = linecnt = n_chars = 0;

  // count lines to allocate linelocs array
  while ((n_chars = read(fd, cbuf, BUFSIZE)) > 0)
	linecnt += count_chars(cbuf, '\n', n_chars);
  if (linecnt <= N_LINES)
	return 0;

  // array of positions of newlines
  int linelocs[linecnt];
  linelocs[0] = 0;
  int loc_index = 0;

  if (lseek(fd, 0, SEEK_SET) == -1)
	oops("couldn't seek start", "");

  while ((n_chars = read(fd, cbuf, BUFSIZE)) > 0) {
	for (int i = 0; i < n_chars; ++i)
	  if (cbuf[i] == '\n') {
		loc_index++;
		subpos = i+1;
		linelocs[loc_index] = (BUFSIZE*block) + subpos;
	  }

	block++;
  }

  return linelocs[linecnt - N_LINES];
}

// create temporary file holding stdin contents
int stdin_tmpf() {
  int out_fd, in_fd;
  int n_chars;
  char buf[BUFSIZE];

  if ((in_fd = fileno(stdin)) == -1)
	oops("couldn't open stdin", "");
  if ((out_fd = open(TMPFILE, O_RDWR | O_CREAT)) == -1)
	oops("failed to create tmpf", "");
  while ((n_chars = read(in_fd, buf, BUFSIZE)) > 0)
	if (write(out_fd, buf, n_chars) != n_chars)
	  oops("read/write error", "stdin_tmpf");

  if (lseek(out_fd, 0, SEEK_SET) == -1)
	oops("seek failure", "stdin_tmpf");
  return out_fd;
}

int main(int argc, char **argv) {
  int in_fd, out_fd;
  bool cleanup = false;

  if (argc == 1) {
	in_fd = stdin_tmpf();
	cleanup = true;
  } else if ((in_fd = open(argv[1], O_RDONLY)) == -1)
	oops("Couldn't open file", argv[1]);
  if ((out_fd = fileno(stdout)) == -1)
	oops("Couldn't open stdout", "");

  unsigned cutoff = find_cutoff(in_fd);
  int n_chars;
  char buf[BUFSIZE];

  if (lseek(in_fd, cutoff, SEEK_SET) == -1)
	oops("couldn't seek cutoff", ""); // TODO int to str
  while ((n_chars = read(in_fd, buf, BUFSIZE)) > 0)
	if (write(out_fd, buf, n_chars) != n_chars)
	  oops("couldn't write stdout", "");

  if (close(in_fd) == -1 || close(out_fd) == -1)
	oops("couldn't close files", "");
  if (cleanup && unlink(TMPFILE) == -1)
	oops("failed to cleanup", TMPFILE);

  return 0;
}