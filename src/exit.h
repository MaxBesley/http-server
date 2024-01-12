/* Max Besley. 20 May 2022. */

#ifndef EXIT_H
#define EXIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/* When ctrl+c is pressed (and SIGINT is sent to the
   program) this function will be called and will
   exit the program in a graceful way. */
void interrupt_handler(int signum);

/* For exiting the program whenever something goes wrong. */
void error_and_exit(char *error_str);

#endif
