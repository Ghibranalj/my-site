
#ifdef DEBUG
#include <stdio.h>
#endif

#include <raylib.h>
// for raylib logging

void raylib_log(int msgType, const char *text, va_list args) {

#ifdef DEBUG
    switch (msgType) {
    case LOG_INFO:
        printf("[INFO] : ");
        break;
    case LOG_ERROR:
        fprintf(stderr, "[ERROR]: ");
        break;
    case LOG_WARNING:
        printf("[WARN] : ");
        break;
    case LOG_DEBUG:
        printf("[DEBUG]: ");
        break;
    default:
        break;
    }

    if (msgType == LOG_ERROR) {
        vfprintf(stderr, text, args);
        fprintf(stderr, "\n");
    } else {
        vprintf(text, args);
        printf("\n");
    }

#endif
}
