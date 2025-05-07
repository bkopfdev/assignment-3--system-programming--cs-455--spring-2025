#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

void fileCopy(int inFd, int outFd);

int main(int argc, char *argv[])
{
    int fileDescriptor;

    if (argc == 1) {
        write(STDERR_FILENO, "No file found.\n", 16);

    } else {
        while (--argc > 0) { // For each argc (file) passed into the program
            fileDescriptor = open(*++argv, O_RDONLY);

            if (fileDescriptor < 0) { //If the file didn't open properly
                write(STDERR_FILENO, "Cat: Cannot open ", 16);
                write(STDERR_FILENO, *argv, strlen(*argv));
                write(STDERR_FILENO, "\n", 1);

                return 1;
            } else {
                fileCopy(fileDescriptor, STDOUT_FILENO);
                close(fileDescriptor);
            }
        }
    }
    return 0;
}

/**
 * fileCopy
 * Copy the contents of a file descriptor to another file descriptor
 * In our case, we are passing the file descriptor of the terminal to output
 *
 * @param inFd: The input file descriptor
 * @param outFd: The output file descriptor
 */
void fileCopy(int inFd, int outFd)
{
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = read(inFd, buffer, BUFFER_SIZE)) > 0) {
        write(outFd, buffer, bytesRead);
    }
}
