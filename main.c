#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include "scanner.h"

static void repl() {
  char line[1024];
  for (;;) {
    printf("> ");

    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }

    interpret(line);
  }
}

static char* readFile(const char* path){
    FILE* file = fopen(path,"rb"); //opens the path file as a the pointer 'file'

    if(file == NULL){
        printf("Could not open file \"%s\".\n",path); // \" is a escape character to print the ' " 'too in the output 
        exit(74);
    }
    
    fseek(file,0L,SEEK_END);  //seeks to the end of the file, 0L means long int 0 that means it says move 0 bytes from the reference point that is end of file (cuz of seek_end)
    size_t filesize = ftell(file); //tells at what byte we are so technically it is the size of the file.
    rewind(file); //points back to the starting of the file

    char* buffer = (char*)malloc(filesize+1); //creats a buffer to hold whats in the file

    if(buffer == NULL)
    {
        fprintf(stderr,"Not enough memory to read \"%s\".\n",path);
        exit(74);
    }

    size_t bytesRead = fread(buffer,sizeof(char),filesize,file);  //reads from file into the buffer. fread() outputs number items read 
    if(bytesRead < filesize)
    {
        fprintf(stderr,"could not read file \"%s\".\n",path);
        exit(74);
    }
    buffer[bytesRead] = '\0'; //null termination of a string

    fclose(file);
    return buffer;
}

static void runFILE(const char* path){
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    if(result == INTERPRET_COMPILE_ERROR) exit(65);
    if(result == INTERPRET_RUNTIME_ERROR) exit(70);
}




int main(int argc, const char* argv[])     //argc and argv are command line inputs where argc keeps count and agrv takes the string input
{
    initVM();
    
    //taking command line arguments if 1 then restart  
    if(argc == 1){
        repl(); //read evaluate print loop
    }

    //if 2 arguments then that means 2nd argument is the file we running
    else if(argc==2){
        runFILE(argv[1]);
    }
    else{
        fprintf(stderr,"Usage: clox [path]\n");
        exit(64);
    }
   
   

    freeVM();
    return 0;

    
}