# Vicon Streamer Library

## Overview
The **Vicon Streamer Library** is a C++ library designed to stream data from the Vicon DataStream SDK over a network using ZeroMQ. It provides a simple and efficient way to capture motion-tracking data from a Vicon system and publish it to clients in real-time.

---

## Features
- **Real-time Data Streaming**: Stream Vicon motion-tracking data over a network.
- **Cross-Platform**: Works on Windows, Linux, and macOS.
- **ZeroMQ Integration**: Uses ZeroMQ for high-performance messaging.
- **Easy-to-Use API**: Simple and intuitive API for integrating into your projects.

---

## Architecture
The library follows a **server-client architecture**:
1. **Server**:
   - Runs on the machine connected to the Vicon system.
   - Reads data from the Vicon DataStream SDK.
   - Publishes data to clients using ZeroMQ.

2. **Client**:
   - Runs on any machine (e.g., Jetson, Laptop).
   - Subscribes to the server to receive data.
   - Processes the received data.

---

## Dependencies
- **Vicon DataStream SDK**: Required for reading data from the Vicon system.
- **ZeroMQ**: Used for network communication.
- **CMake**: For building the library and examples.

---

## API Reference

### Server API
The `ViconStreamer` class provides the server-side functionality.

#### Class: `vicon_streamer::ViconStreamer`
- **Constructor**:
  ```cpp
  ViconStreamer(const std::string& host, int port);
  ```
  - `host`: The IP address or hostname to bind the server to (e.g., `"127.0.0.1"`).
  - `port`: The port to bind the server to (e.g., `5556`).

- **Methods**:
  ```cpp
  void connect();
  ```
  - Connects to the Vicon system.

  ```cpp
  void streamData();
  ```
  - Starts streaming data from the Vicon system and publishes it over ZeroMQ.

  ```cpp
  void disconnect();
  ```
  - Disconnects from the Vicon system.

---

### Client API
The client uses ZeroMQ to subscribe to the server and receive data.

#### Example Client Code
```cpp
#include <zmq.hpp>

int main() {
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://127.0.0.1:5556"); // Replace with server IP
    subscriber.set(zmq::sockopt::subscribe, "");
    while (true) {
        zmq::message_t message;
        subscriber.recv(message);

        std::string receivedData(static_cast<char*>(message.data()), message.size());
        std::cout << "Received: " << receivedData << std::endl;
        ....
        ....
        ....
    }

    return 0;
}
```

---

## Build Instructions

### 1. Prerequisites
- Install **ZeroMQ**:
  - On Ubuntu:
    ```bash
    sudo apt-get install libzmq3-dev
    ```
  - On Windows: Download from [ZeroMQ's website](https://zeromq.org/download/).
- Install **CMake**:
  - On Ubuntu:
    ```bash
    sudo apt-get install cmake
    ```
  - On Windows: Download from [CMake's website](https://cmake.org/download/).

### 2. Clone the Repository
```bash
git clone https://github.com/Tejaaswini/vicon_streamer.git
cd vicon_streamer
```

### 3. Build the Library
1. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```
2. Run CMake:
   ```bash
   cmake ..
   ```
3. Build the library and examples:
   ```bash
   make
   ```

### 4. Run the Examples
- Start the server:
  ```bash
  ./vicon_server
  ```
- Run the client:
  ```bash
  ./vicon_client
  ```

---

## Usage

### 1. Include the Library in Your Project
Add the following to your `CMakeLists.txt`:
```cmake
find_package(vicon_streamer REQUIRED)
target_link_libraries(your_target PRIVATE vicon_streamer)
```

### 2. Example Usage
#### Server
```cpp
#include "vicon_streamer/vicon_streamer.h"

int main() {
    vicon_streamer::ViconStreamer streamer("127.0.0.1", 5556);
    streamer.connect();
    streamer.streamData();
    streamer.disconnect();
    return 0;
}
```

#### Client
```cpp
#include <zmq.hpp>

int main() {
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://127.0.0.1:5556"); // Replace with server IP
    subscriber.set(zmq::sockopt::subscribe, "");

    while (true) {
        zmq::message_t message;
        subscriber.recv(message);

        std::string receivedData(static_cast<char*>(message.data()), message.size());
        std::cout << "Received: " << receivedData << std::endl;
        ....
        ....
        ....
    }

    return 0;
}
```

---

## Data Format
The server publishes data in the following format:
```
<subject_name>,<x>,<y>,<z>
```
- `subject_name`: Name of the Vicon subject.
- `x`, `y`, `z`: Global translation coordinates of the subject.

---

## Troubleshooting
1. **Server Not Publishing Data**:
   - Ensure the Vicon system is connected and streaming data.
   - Check the server logs for errors.

2. **Client Not Receiving Data**:
   - Ensure the server IP and port are correct.
   - Verify that the server is running.

3. **ZeroMQ Errors**:
   - Ensure ZeroMQ is installed correctly.
   - Check for firewall or network issues.

---

## License
This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

## Contact
For questions or feedback, please open an issue on GitHub or contact Tejaaswini Narendran at [tejnaren07@gmail.com].

---
