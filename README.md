# udemos
HTTP server 

The HttpServer class is a templated class that is used to create an HTTP server. It takes a router object as a template parameter that is used to route incoming HTTP requests to their corresponding handlers.

Public Members

The public members of the HttpServer class are:

Constructor
HttpServer(uint32_t port, std::shared_ptr<T> router, const std::vector<std::string>& ip_address_to_ignore = {})
Constructs a new HttpServer object. The constructor takes the following arguments:

port: the port number to listen for incoming HTTP requests on
router: a shared pointer to the router object to use for routing incoming HTTP requests
ip_address_to_ignore: an optional vector of IP addresses to ignore
  
void start()
Starts the HTTP server. This method listens for incoming HTTP requests on the port specified in the constructor and routes them to the appropriate handlers using the router object.

Private Members

The private members of the HttpServer class are:

uint32_t m_port: the port number to listen for incoming HTTP requests on
std::shared_ptr<T> m_router: a shared pointer to the router object to use for routing incoming HTTP requests
std::unique_ptr<HttpThreadPool> m_thread_pool: a unique pointer to a thread pool object used to manage connections to the server
std::unique_ptr<HttpAcceptor> m_acceptor: a unique pointer to the acceptor object used to accept incoming connections.
