#include "Reactor.h"
#include <string.h>




const size_t kBufferSize = 1024;
char g_read_buffer[kBufferSize];
char g_write_buffer[kBufferSize];


bool IsValidHandle(reactor::handle_t handle)
{
    return handle >= 0;
}


void ReportSocketError(const char * msg)
{
    fprintf(stderr, "%s error: %s\n", msg, strerror(errno));
}

