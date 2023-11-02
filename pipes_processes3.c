#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat scores | grep Lakers | sort".
 *
 */

int main(int argc, char **argv)
{
  int pipefd1[2];
  int pipefd2[2];
  int pid1, pid2;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL};

  // make two pipes (fds go in pipefd[0] and pipefd[1])

  pipe(pipefd1);
  pipe(pipefd2);

  pid1 = fork();

  if (pid1 == 0)
    {
      // child gets here and handles "grep <argument>"

      // replace standard input with input part of first pipe

      dup2(pipefd1[0], 0);

      // close unused half of first pipe

      close(pipefd1[1]);

      // replace standard output with output part of second pipe

      dup2(pipefd2[1], 1);

      // close unused half of second pipe

      close(pipefd2[0]);

      // execute grep

      execvp("grep", grep_args);
    }
  else
    {
      pid2 = fork();

      if (pid2 == 0)
        {
          // child's child gets here and handles "sort"

          // replace standard input with input part of second pipe

          dup2(pipefd2[0], 0);

          // close unused half of second pipe

          close(pipefd2[1]);

          // execute sort

          execvp("sort", sort_args);
        }
      else
        {
          // parent gets here and handles "cat scores"

          // replace standard output with output part of first pipe

          dup2(pipefd1[1], 1);

          // close unused input half of first pipe

          close(pipefd1[0]);

          // execute cat

          execvp("cat", cat_args);
        }
    }
}
