#ifndef udp_http_defines
#define udp_http_defines

#include <unistd.h>

const uint32_t MAX_BUFFER_SIZE = 1024;

const uint32_t MAX_REQUEST_SIZE = 1024;

static constexpr int MAX_BACKLOG_SIZE = 4096;

static constexpr int TIMEOUT_DELAY = 30000;

#endif