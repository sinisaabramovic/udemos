#ifndef ud_server_hpp
#define ud_server_hpp

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <functional>

#include "common/ud_result.hpp"
#include "router/ud_http_router.hpp"
#include "common/ud_result_failure.hpp"
#include "common/ud_result_success.hpp"


class ud_server
{
public:
  ud_server(uint32_t port, const std::string& host) : m_port(port), m_host(host), m_sock_fd(0) {}
  virtual ~ud_server() {}

  using status_delegate = std::function<void(const ud_result<ud_result_success, ud_result_failure> &)>;  
  virtual void start_listen(std::shared_ptr<ud_http_router> router, status_delegate delegate) = 0;
  virtual void pause_listen(bool pause) = 0;
  virtual void stop_listen() = 0;
  virtual bool is_running() = 0;
  virtual bool is_paused() = 0;

protected:
  std::atomic<bool> stop_flag = false;
  std::atomic<bool> pause_flag = false;
  bool stoped = false;
  bool paused = false;
  std::condition_variable pause_cv;
  std::mutex pause_mutex;
  std::unique_ptr<std::thread> m_listener_thread;
  std::string m_host;
  uint32_t m_port;

  int32_t m_sock_fd;
};

#endif