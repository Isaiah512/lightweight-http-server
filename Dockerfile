FROM gcc:latest

WORKDIR /usr/src/app

COPY http_server.cpp .

# Install any dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

RUN g++ -o http_server http_server.cpp -std=c++11

EXPOSE 8080

CMD ["./http_server"]
