#ifndef ud_acceptor_data_hpp
#define ud_acceptor_data_hpp

class ud_acceptor_data {
public:
    ud_acceptor_data(int port, int sock_fd) : m_port(port), m_socket_fd(sock_fd) {}
    int getPort() const { return m_port; }
    int getSocketFd() const { return m_socket_fd; }
private:
    int m_port;
    int m_socket_fd;
};


#endif