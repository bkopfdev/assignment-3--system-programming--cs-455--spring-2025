#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int getFile(char *fileName);
int getLine(int fileDescriptor, char *buffer);

int main(int argc, char *argv[]) {
    int fileDescriptor, fileDescriptor2;

    if (argc != 3) {
        write(STDERR_FILENO, "Can only compare two files.\n", 28);

        return 1;
    }

    fileDescriptor = getFile(argv[1]);
    fileDescriptor2 = getFile(argv[2]);

    if (fileDescriptor < 0 || fileDescriptor2 < 0) {
        write(STDOUT_FILENO, "One or both of the files cannot be accessed.\n", 22);

        return 1;
    }

    char lineFileOne[BUFFER_SIZE], lineFileTwo[BUFFER_SIZE];
    int line_len1, line_len2;
    int lineNumber = 1;

    while (1) {
        line_len1 = getLine(fileDescriptor, lineFileOne);
        line_len2 = getLine(fileDescriptor2, lineFileTwo);

        if (line_len1 == 0 && line_len2 == 0) { //If it's the end of both files
            write(STDOUT_FILENO, "Files are the same!\n", 21);

            break;
        }

        if (strcmp(lineFileOne, lineFileTwo) != 0) { //Compare the two lines
            write(STDOUT_FILENO, "Files are different!", 20);
            write(STDOUT_FILENO, "\nFile 1 line: ", 15);
            write(STDOUT_FILENO, lineFileOne, line_len1);
            write(STDOUT_FILENO, "\nFile 2 line: ", 15);
            write(STDOUT_FILENO, lineFileTwo, line_len2);
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        lineNumber++; //Onto the next line
    }

    close(fileDescriptor);
    close(fileDescriptor2);

    return 0;
}

/**
 * getFile
 * Take in a file name and return the file descriptor
 *
 * @param fileName The name of the file to open
 * @return The file descriptor
 */
int getFile(char *fileName) {
    int fileDescriptor = open(fileName, O_RDONLY);

    if (fileDescriptor < 0) {
        write(STDERR_FILENO, "Cat: Cannot open ", 16);
        write(STDERR_FILENO, fileName, strlen(fileName));
        write(STDERR_FILENO, "\n", 1);

        return -1;
    }
    return fileDescriptor;
}

/**
 * getLine
 * Take in a file and a buffer, then read a line from the file and return it
 *
 * @param fileDescriptor The file descriptor to read from
 * @param buffer The buffer to store the line in
 * @return The line
 */
int getLine(int fileDescriptor, char *buffer) {
    int bufferIndex = 0;
    char currentChar;

    while (bufferIndex < BUFFER_SIZE - 1) {
        int bytesRead = read(fileDescriptor, &currentChar, 1);

        if (bytesRead == 0 || currentChar == '\n') {
            break; // End of file or line
        }

        if (bytesRead < 0) {
            return -1; // Error
        }

        buffer[bufferIndex++] = currentChar;
    }
    buffer[bufferIndex] = '\0';

    return bufferIndex;
}
