#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent to child
    // Second pipe to send concatenated string from child to parent 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu"; 
    char fixed_str2[] = "gobison.org";
    char input_str[100]; 
    char input_str2[100]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str); 
  
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
        // Close reading end of second pipe
        close(fd2[0]);
  
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        
  
        // Wait for child to send a string 
        wait(NULL); 
  
        // Close both reading and writing ends of first pipe
        close(fd1[0]);
        close(fd1[1]);

        // Read concatenated string from child
        read(fd2[1], input_str2, 100);

        // Concatenate "gobison.org" to the string
        strcat(input_str2, fixed_str2);

        // Print the concatenated string to stdout
        printf("Concatenated string: %s\n", input_str2);

        // Close reading end of second pipe
        close(fd2[0]);
    } 
  
    // child process 
    else
    { 
        // Close writing end of first pipe and reading end of second pipe
        close(fd1[1]);
        close(fd2[0]);

        // Read input string from parent
        char concat_str[100]; 
        read(fd1[0], concat_str, 100); 
  
        // Concatenate "howard.edu" to the string 
        strcat(concat_str, fixed_str);

        // Print the concatenated string to stdout
        printf("Concatenated string: %s\n", concat_str);

        // Prompt user for a second input
        printf("Enter a second string to concatenate:");
        scanf("%s", input_str2);

        // Write the second input string to the parent
        write(fd2[1], input_str2, strlen(input_str2)+1);

        // Close both writing and reading ends of second pipe
        close(fd1[0]);
        close(fd2[1]);

        exit(0); 
    } 
}
