#ifndef udp_http_defines
#define udp_http_defines

#include <unistd.h>

constexpr uint32_t MAX_BUFFER_SIZE = 1024U;

constexpr uint32_t MAX_REQUEST_SIZE = 1024U;

static constexpr int32_t MAX_BACKLOG_SIZE = 4096;

static constexpr std::size_t MAX_TASK_QUEUE_SIZE = 100U;

static constexpr uint32_t TIMEOUT_DELAY = 5000U;

#endif