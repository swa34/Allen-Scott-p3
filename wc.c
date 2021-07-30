#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<stdlib.h>
#include <fcntl.h>

struct message{
  int lines;
  int words;
  int max_line_length;
  int size;
  int chars;
}info;

void error_print(char str[]){
  printf("Error:%s",str);
}

void init(char filename[]){
  struct stat get_message = {0};
  int fp;
  int ret_stat = stat(filename,&get_message);/*Use the stat function to read the information of the filenmae file and write the result to the get_message*/
  if(ret_stat == -1){//If there is no error in the stat function, output information
	error_print(filename);
	return ;
  }
  mode_t mode = get_message.st_mode; //Receive file information, used to determine whether it is a directory below
  int length = 0;
  if(S_ISDIR(mode)) //If it is a directory, output error
	printf("Error %s is dir\n0\t0\t0\t%s",filename,filename);
  else{
	info.size = get_message.st_size; //File byte size wc -c
	fp = open (filename,O_RDONLY); //Open the specified file in read-only mode
	char ch;
	int flag = 0;
	while(read(fp, &ch, 1) > 0){ //Read until the end of the file
	  info.chars++; //The number of characters plus 1

	  if(ch != '\n'){

		length++; //Record the length of the current line
	  }
	  if(ch == '\n'){
		info.lines ++; //Add 1 to the number of lines wc -l
		if(length>info.max_line_length)
		  info.max_line_length = length; //Update the maximum length
		length = 0;
	  }
	  if(ch == '\t' || ch == ' ' || ch == '\n'){
		flag = 0; //Calculate the number of words wc -w
		continue;
	  }
	  else{

		if(flag == 0){
		  info.words++; //Calculate the number of words wc -w
		  flag = 1;
		}
	  }
	}
	close(fp);
  }

}
//Calculate the relevant information of the keyboard input, if the file to be opened is not specified in the parameter
void EmptyFile(){
  char ch;
  int flag = 0;
  int length = 0;

  while(read(flag, &ch, 1) > 0){
	info.chars++;
	info.size += sizeof(ch); //byte accumulation
	if(ch != '\n'){
	  length++;
	}
	if(ch == '\n'){
	  info.lines ++;
	  if(length>info.max_line_length)
		info.max_line_length = length;
	  length = 0;
	}
	if(ch == '\t' || ch == ' ' || ch == '\n'){
	  flag = 0;
	  continue;
	}
	else{

	  if(flag == 0){
		info.words++;
		flag = 1;
	  }
	}

  }
}
int main(int argc,char *argv[]){

  if(argc == 2){
	if(argv[1][0] != '-'){
	  init(argv[1]);
	  printf("%d %d %d %s\n",info.lines,info.words,info.size,argv[1]);
	  return 0;
	}
	else{ //Unspecified open file, similar to wc -lwc
	  EmptyFile();

	}
  }
  else if(argc == 1){ //The open file and the parameters to be output are not specified, (default output -lwc)
	EmptyFile();
	printf("%d\t%d\t%d\n",info.lines,info.words,info.size);
	return 0;
  }
  else if(argc == 3){
	init(argv[2]);
  }
  int num;
  while((num = getopt(argc,argv,"lwc"))!=-1){
	switch(num){
	  case 'l':
		printf("%d\t",info.lines);
		break;
	  case 'w':
		printf("%d\t",info.words);
		break;
	  case 'c':
		printf("%d\t",info.size);
		break;
	}
  }
  if(argc != 2 && argv[1][0] !='-') //Be sure to judge, otherwise it will cross the boundary
	printf("%s\n",argv[2]);


  return 0;
}
