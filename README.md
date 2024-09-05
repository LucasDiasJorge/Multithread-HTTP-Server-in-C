# Simple Multithreaded HTTP Server

This project implements a basic HTTP server in C that can handle multiple connections concurrently using threads. The server listens on port 8080 and responds with a "Hello, World!" message to any HTTP request it receives.

## Features

- **Multithreading**: Uses POSIX threads (pthreads) to handle multiple client connections simultaneously.
- **HTTP Response**: Sends a basic HTTP 200 OK response with a "Hello, World!" message.
- **Socket Programming**: Utilizes socket programming to accept and manage client connections.

## Requirements

- **C Compiler**: A C compiler such as GCC.
- **POSIX Threads Library**: Ensure that your system supports pthreads.

## Compilation

To compile the server code, use the following command:

```sh
gcc -o http_server http_server.c -lpthread
```

## Running the Server

To start the server, run the following command:

```sh
./http_server
```

The server will start listening on port 8080 and will print messages to the console about incoming connections.

## Code Explanation

### `handle_connection`

This function is executed in a new thread for each incoming connection. It:

1. Reads the HTTP request from the client.
2. Sends a simple HTTP 200 OK response with "Hello, World!".
3. Closes the connection.

### `main`

The `main` function sets up and runs the server:

1. **Socket Creation**: Creates a socket using `socket()`.
2. **Socket Options**: Configures the socket to reuse the address and port using `setsockopt()`.
3. **Binding**: Binds the socket to the specified port using `bind()`.
4. **Listening**: Begins listening for incoming connections using `listen()`.
5. **Accepting Connections**: Accepts new connections using `accept()` and creates a new thread for each connection to handle it using `pthread_create()`.

## Error Handling

The server checks for errors at various stages (socket creation, binding, listening, and accepting) and prints appropriate error messages if something goes wrong.

## Notes

- The server is single-threaded for handling connections but uses threads to handle each client request concurrently.
- The server assumes all incoming connections are HTTP requests and responds with a static message.