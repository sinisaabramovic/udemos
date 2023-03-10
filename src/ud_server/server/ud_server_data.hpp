#ifndef ud_server_data_hpp
#define ud_server_data_hpp

#include <thread>
#include <chrono>
#include <atomic>
#include <random>

#include "../common/ud_thread_manager.hpp"
#include "../acceptor/ud_http_acceptor.hpp"
#include "../connection/ud_http_connection.hpp"
#include "../router/ud_http_router.hpp"
#include "../common/ud_http_thread_pool.hpp"

class ud_server_data
{
public:
    ud_server_data() : m_stop_flag(false),
                       m_stoped(false),
                       m_pause_flag(false),
                       m_pause_mutex(),
                       m_pause_cv(),
                       m_port(8080),
                       m_host("0.0.0.0"),
                       m_sock_fd(0),
                       m_router(nullptr),
                       m_acceptor(nullptr),
                       m_thread_pool(nullptr),
                       m_listener_thread(nullptr),
                       m_rng(std::chrono::steady_clock::now().time_since_epoch().count()),
                       m_sleep_times(10U, 100U) {}

    void set_stop_flag(bool flag) { m_stop_flag = flag; }
    bool get_stop_flag() const { return m_stop_flag; }

    void set_stoped(bool stopped) { m_stoped = stopped; }
    bool get_stoped() const { return m_stoped; }

    void set_pause_flag(bool flag) { m_pause_flag = flag; }
    bool get_pause_flag() const { return m_pause_flag; }

    std::mutex& get_pause_mutex() { return m_pause_mutex; }
    std::condition_variable& get_pause_cv() { return m_pause_cv; }

    void set_port(uint32_t port) { m_port = port; }
    uint32_t get_port() const { return m_port; }

    void set_host(const std::string& host) { m_host = host; }
    const std::string& get_host() const { return m_host; }

    void set_sock_fd(int sock_fd) { m_sock_fd = sock_fd; }
    int get_sock_fd() const { return m_sock_fd; }

    void set_router(std::shared_ptr<ud_http_router> router) { m_router = std::move(router); }
    std::shared_ptr<ud_http_router> get_router() const { return m_router; }

    void set_acceptor(std::shared_ptr<ud_http_acceptor> acceptor) { m_acceptor = std::move(acceptor); }
    std::shared_ptr<ud_http_acceptor> get_acceptor() const { return m_acceptor; }

    void set_thread_pool(std::unique_ptr<ud_http_thread_pool> thread_pool) { m_thread_pool = std::move(thread_pool); }
    ud_http_thread_pool* get_thread_pool() const { return m_thread_pool.get(); }

    void set_listener_thread(std::shared_ptr<std::thread> listener_thread) { m_listener_thread = std::move(listener_thread); }
    std::shared_ptr<std::thread> get_listener_thread() const { return m_listener_thread; }

    std::mt19937& get_rng() { return m_rng; }

    std::uniform_int_distribution<int>& get_sleep_times() { return m_sleep_times; }

    void setup_socket()
    {
        if ((m_sock_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
        {
            throw std::runtime_error("Failed to create a TCP socket");
        }
    }

private:
    std::atomic<bool> m_stop_flag;
    std::atomic<bool> m_stoped;
    std::atomic<bool> m_pause_flag;
    std::mutex m_pause_mutex;
    std::condition_variable m_pause_cv;
    uint32_t m_port;
    std::string m_host;
    int m_sock_fd;
    std::shared_ptr<ud_http_router> m_router;
    std::shared_ptr<ud_http_acceptor> m_acceptor;
    std::unique_ptr<ud_http_thread_pool> m_thread_pool;
    std::shared_ptr<std::thread> m_listener_thread;
    std::mt19937 m_rng;
    std::uniform_int_distribution<int> m_sleep_times;
};

#endif