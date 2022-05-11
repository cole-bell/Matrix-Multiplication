
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> /*pid_t */
#include <sys/wait.h> /*wait */

//global variables that determine matrix sizes
#define M 2
#define N 3
#define O 2

int main()
{
  int value, valParent;
	int matrix1[M][N] = { {1, 2, 3}, {4, 5, 6} }; /*M rows x N columns*/
	int matrix2[N][O] = { {7, 8}, {9,10}, {11, 12} }; /* N x O */
	int Product[M][O]; /* M x O */
  int pipeCount = 0;

  int fd[O*M][2]; // create an array of pipes. 1 pipe per piece of data in the output
  for(int i=0; i < O*M; i++){
    pipe(fd[i]);
  }

	pid_t pid; //true parent child and parent are created
    for(int r = 0; r < M; r++){
      pid = fork(); // p x
      if (pid == 0) {

        for(int i = 0; i < O; i++){// we need to get the values of O in matrix2 O times. This loop achieves this
        value = 0;
          for(int j = 0; j < N; j++){// we need to get the values of N in matrix 1 N times, this loop achieves that
            value = value + matrix1[r][j] * matrix2[j][i]; //the math
            }

        close(fd[pipeCount][0]); //close the read value of the current pipe.
        write(fd[pipeCount][1], &value, sizeof(int)); //write data to the parent.
        pipeCount++; //track which pipe we are using
        }
        exit(0);
    }
    pipeCount+=O;
    }

    int value3=0;

    int count = 0;
    for (int i = 0; i < M; i++){
      for(int j = 0; j < O; j++){
        close(fd[count][1]);
        read(fd[count][0], &value3, sizeof(int)); //read data from current pipe
        Product[i][j] = value3; // store the current pipes data into the products matrix at correct location
        printf("%d ", Product[i][j]); //print the values taht are stored in the current location of the products matrix
        count++; // increment the pipe so we can recieve the next piece of data.
       // printf("The count is: %d\n", count);
      }
      printf("\n"); // create a line break in order to format the matrix properly
    }
    return 0;
}
