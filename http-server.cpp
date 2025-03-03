#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

class HTTPServer {
private:
    int server_fd;
    int port;
    struct sockaddr_in address;
    const int BUFFER_SIZE = 4096;

public:
    HTTPServer(int port) : port(port) {
        // socket file descriptor
        // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = default protocol
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            throw std::runtime_error("Failed to create socket!");
        // socket options for port reuse
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
            throw std::runtime_error("Failed to set socket options!");
        // server address structure
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_ANY);
        address.sin_port = htons(port);
        // bind socket
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
            throw std::runtime_error("Failed to bind socket!");
        // listen for incoming connections
        if (listen(server_fd, 10) < 0)
            throw std::runtime_error("Failed to listen to incoming connections!");

        std::cout << "HTTP Server started on http://127.0.0.1:" << port << std::endl;
    }

    ~HTTPServer() {
        close(server_fd);
    }

    void start() {
        while (true) {
            struct sockaddr_in client_address;
            socklen_t addrlen = sizeof(client_address);
            // accept incoming connections
            int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &addrlen);
            if (new_socket < 0) {
                std::cerr << "Failed to accept connection!" << std::endl;
                continue;
            } 
            std::cout << "New connection accepted" << std::endl;
            
            std::thread clientThread(&HTTPServer::handleClient, this, new_socket);
            clientThread.detach();
        }
    }

private:
    void handleClient(int client_socket) {
        char buffer[4096] = {0};
        int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
        
        if (bytes_read < 0) {
            std::cerr << "Failed to read from socket!" << std::endl;
        } else if (bytes_read == 0) {
            std::cerr << "Client closed connection" << std::endl;
        } else {
            std::cout << "Read " << bytes_read << " bytes from client" << std::endl;
            
            std::string request(buffer);
            std::string response = handleRequest(request);
            
            size_t firstLineEnd = request.find("\r\n");
            if (firstLineEnd != std::string::npos) {
                std::cout << "Request: " << request.substr(0, firstLineEnd) << std::endl;
            }
            
            int bytes_sent = send(client_socket, response.c_str(), response.length(), 0);
            if (bytes_sent < 0) {
                std::cerr << "Failed to send response!" << std::endl;
            } else {
                std::cout << "Sent " << bytes_sent << " bytes to client" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        
        close(client_socket);
    }

    std::string handleRequest(const std::string& request) {
        std::string method, path;
        std::istringstream req_stream(request);
        req_stream >> method >> path;
        
        std::cout << "Method: " << method << ", Path: " << path << std::endl;
        
        std::string status = "200 OK";
        std::string content_type = "text/html";
        std::string content;

        if (path == "/" || path == "/index.html") {
            content = "<html><body><h1>Welcome</h1><p>This is the home page.</p></body></html>";
        } else if (path == "/about") {
            content = "<html><body><h1>About</h1><p>This is the about page.</p></body></html>";
        } else {
            status = "404 Not Found";
            content = "<html><body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body></html>";
        }

        std::stringstream response_stream;
        response_stream << "HTTP/1.1 " << status << "\r\n";
        response_stream << "Content-Type: " << content_type << "\r\n";
        response_stream << "Content-Length: " << content.length() << "\r\n";
        response_stream << "Connection: close\r\n\r\n";
        response_stream << content;
        
        return response_stream.str();
    }
};

int main() {
    try {
        HTTPServer server(8080);
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
