#include "io.h"

/**
 * Closes FD, if FD presents
 *
 * @param file FD
 */
void close_file_descriptor(FILE* file) {
    if (file == NULL) return;
    if (file == stdin || file == stdout || file == stderr) return;

    fclose(file);
}
