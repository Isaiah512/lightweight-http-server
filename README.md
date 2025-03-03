# C++ HTTP Server

A lightweight HTTP server built from scratch in C++ without any third party libraries, that can be run natively or in a Docker container.

## Features

- Minimal Dependencies
- Multi-threaded request handling
- Dockerized Deployment

## Prerequisites

### For Native Compilation

**On Unix-based systems (Linux/macOS):**
- GCC/G++ compiler (version 4.8+)
- Make (optional)

**On Windows:**
- Visual Studio with C++ tools or MinGW
- Windows SDK

### For Docker Deployment (All Platforms):
- Docker
- Docker Compose (optional, recommended)

## Getting Started

### Option 1: Running in Docker (Recommended for all platforms)

This is the easiest method that works on any OS (Windows, macOS, Linux).

1. **Clone the repository:**
   ```bash
   https://github.com/yourusername/lightweight-http-server.git
   cd lightweight-http-server
   ```

2. **Build and run with Docker:**
   ```bash
   # Using Docker directly
   docker build -t lightweight-http-server .
   docker run -p 8080:8080 lightweight-http-server
   
   # Or using Docker Compose
   docker-compose up
   ```

3. **Access the server:**
   Open your browser and navigate to `http://localhost:8080`

### Option 2: Native Compilation (Platform-specific)

#### On Unix-based systems (Linux/macOS):

1. **Clone the repository:**
   ```bash
   https://github.com/yourusername/lightweight-http-server.git
   cd lightweight-http-server
   ```

2. **Compile the server:**
   ```bash
   g++ -o http-server http-server.cpp -std=c++11
   ```

3. **Run the server:**
   ```bash
   ./http-server
   ```

4. **Access the server:**
   Open your browser and navigate to `http://localhost:8080`

## How It Works

1. **Initialization:**
    - The server creates a TCP socket, sets it up for address reuse, and binds it to a given port (default 8080).
    - It then starts listening for incoming connections.

2. **Handling Connections:**
    - Each incoming connection is accepted and logged.
    - A dedicated thread is created for each client to handle requests concurrently.

3. **Request Processing:**
    - The server reads the client's HTTP request, extracts the HTTP method and requested path, and logs the first line.
    - Depending on the path, it routes the request to serve the homepage (`/` or `index.html`) the about page (`/about`) or returns the 404 response for unkown paths.

4. **Response:**
    - An appropriate HTTP response is generated, including status, headers (Content-Type, Content-Length and Connection) and an HTML content body.
    - The response is sent back to the client before closing the connection.

## Customization

### Changing the Port

1. **For native compilation:**
   - Modify the port number in the main function: `HTTPServer server(YOUR_PORT);`

2. **For Docker:**
   - Modify the port mapping in your Docker command or `docker-compose.yml`:
     ```bash
     docker run -p YOUR_PORT:8080 cpp-http-server
     ```
   - Or in `docker-compose.yml`:
     ```yaml
     ports:
       - "YOUR_PORT:8080"
     ```

### Adding New Routes

To add more endpoints, simply extend the `handleRequest` method:

```cpp
if (path == "/" || path == "/index.html") {
    // Home page content
} else if (path == "/about") {
    // About page content
} else if (path == "/contact") {
    content = "<html><body><h1>Contact</h1><p>Contact us at contact@example.com.</p></body></html>";
} else {
    // 404 Not Found content
}
```
