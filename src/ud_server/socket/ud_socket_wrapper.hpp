#ifndef ud_socket_wrapper_hpp
#define ud_socket_wrapper_hpp

class ud_socket_wrapper
{
public:
    explicit ud_socket_wrapper(int32_t fd) : m_fd(fd) {}
    ~ud_socket_wrapper()
    {
        if (m_fd != -1)
        {
            close(m_fd);
        }
    }
    int32_t get_fd() const { return m_fd; }

private:
    int32_t m_fd;
};

#endif