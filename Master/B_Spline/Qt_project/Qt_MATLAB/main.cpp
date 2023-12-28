
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"
#define  BUFSIZE 256

int main()

{
    Engine *ep;
    mxArray *T = NULL, *result = NULL;
    char buffer[BUFSIZE+1];
    double time[10] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
    if (!(ep = engOpen(""))) {
        fprintf(stderr, "\nCan't start MATLAB engine\n");
        return EXIT_FAILURE;
    }
    T = mxCreateDoubleMatrix(1, 10, mxREAL);
    memcpy((void *)mxGetPr(T), (void *)time, sizeof(time));
    engPutVariable(ep, "T", T);
    engEvalString(ep, "D = .5.*(-9.8).*T.^2;");
    engEvalString(ep, "plot(T,D);");
    engEvalString(ep, "title('Position vs. Time for a falling object');");
    engEvalString(ep, "xlabel('Time (seconds)');");
    engEvalString(ep, "ylabel('Position (meters)');");
    printf("Hit return to continue\n\n");
    fgetc(stdin);
    printf("Done for Part I.\n");
    mxDestroyArray(T);
    engEvalString(ep, "close;");
    buffer[BUFSIZE] = '\0';
    engOutputBuffer(ep, buffer, BUFSIZE);
    while (result == NULL) {
        char str[BUFSIZE+1];
        printf("Enter a MATLAB command to evaluate.  This command should\n");
        printf("create a variable X.  This program will then determine\n");
        printf("what kind of variable you created.\n");
        printf("For example: X = 1:5\n");
        printf(">> ");

        fgets(str, BUFSIZE, stdin);
        engEvalString(ep, str);
        printf("%s", buffer);
        printf("\nRetrieving X...\n");
        if ((result = engGetVariable(ep,"X")) == NULL)
          printf("Oops! You didn't create a variable X.\n\n");
        else {
        printf("X is class %s\t\n", mxGetClassName(result));
        }
    }
    printf("Done!\n");
    mxDestroyArray(result);
    engClose(ep);

    return EXIT_SUCCESS;
}
