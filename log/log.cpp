
#include "log/log.h"

int main (const int argc, const char **argv) {
    return 0;
}

int log_update_RelativePath(struct log_RealtiveData *pRData, struct timespec *pNow) {
    struct log_RelativeData *RData = pRData;
    strftime(RData->ActualName, sizeof(RData->ActualName), LOG_FORMAT_RELATIVE_FILE, localtime(&(pNow->tv_sec)));
    return -1;
}