/* Max Besley. 20 May 2022. */

#include "exit.h"


void
error_and_exit(char *error_str) {
    perror(error_str);
    exit(EXIT_FAILURE);
}

void
interrupt_handler(int signum) {
    char *exit_msg = "\nClosing the server...\n";
    // Note: write() is an 'async-signal-safe' function
    write(STDOUT_FILENO, exit_msg, strlen(exit_msg));
    // Exit the program
    exit(EXIT_SUCCESS);
}
