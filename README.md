# udemos
HTTP server 

The HttpServer class is a templated class that is used to create an HTTP server. It takes a router object as a template parameter that is used to route incoming HTTP requests to their corresponding handlers.

Generate priveate key:
openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:2048 -out private_key.pem

Public Members

The public members of the HttpServer class are:

Logger::getInstance().log(LogLevel::Info, "Server started");
    
RouteHandlerFactory::getInstance().registerHandler(std::make_shared<GetRouteHandler>(HttpMethod::POST, "/api/simple"));
RouteHandlerFactory::getInstance().registerHandler(std::make_shared<GetSecretRouteHandler>(HttpMethod::POST, "/api/secret"));
RouteHandlerFactory::getInstance().registerHandler(std::make_shared<GetLongResponseRouteHandler>(HttpMethod::GET, "/api/long"));
    
Configuration& config = Configuration::getInstance();
config.set("host", "127.0.0.1");
config.set("port", 8080);
    
std::shared_ptr<HttpService> httpService = std::make_shared<HttpService>(config);
httpService->registerProtocolHandler(ServiceType::HTTP, std::make_unique<HttpProtocolHandler>());
