#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFF_SIZE 4096
int main(int argc, char const *argv[]) {
  int LINES_TO_READ = 10;
  int ch;
  switch (ch) {
	case 'n':LINES_TO_READ = atoi(optarg);
	  break;
	case '?': perror("unknown option\n");
	  return -1;

  }
  for (ssize_t i = 1; i < argc; i++) {
	const char *filename = argv[i];

	int fd = open(filename, O_RDONLY);

	if (fd < 0) {
	  perror("open");
	  return -1;
	}

	char ch, buffer[BUFF_SIZE];
	size_t index = 0, lines = 1;
	ssize_t rresult, wresult;

	// Read the file byte by byte
	while ((rresult = read(fd, &ch, 1)) != 0) {
	  if (rresult < 0) {
		perror("read");
		return -1;
	  }

	  // Check if the current character is a new line (the line ends here)
	  if (ch == '\n') {
		buffer[index] = ch;
		buffer[index + 1] = '\0';
		ch = 0;
		index = 0;

		// Print the line
		wresult = 0;
		ssize_t buffer_length = strlen(buffer);
		while (wresult != buffer_length) {
		  ssize_t res = write(STDOUT_FILENO, buffer + wresult, buffer_length - wresult);

		  if (wresult < 0) {
			perror("write");
			return -1;
		  }

		  wresult += res;
		}

		// Stop if we read 10 lines already
		if (lines == LINES_TO_READ) {
		  break;
		}

		lines++;
	  } else {
		buffer[index++] = ch;
	  }
	}

	if (close(fd) < 0) {
	  perror("close");
	  return -1;
	}
  }

  return 0;
}