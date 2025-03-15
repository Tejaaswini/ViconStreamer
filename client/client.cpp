#include <iostream>
#include <zmq.hpp>
#include <atomic>
#include <csignal>
#include <thread>

std::atomic<bool> running(true);

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received. Stopping subscriber..." << std::endl;
    running = false;
}

int main() {
    std::signal(SIGINT, signalHandler);

    // ZeroMQ context and subscriber socket
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);

    try {
        // Connect to publisher
        std::string server_address = "tcp://127.0.0.1:5556";
        subscriber.connect(server_address);
        std::cout << "Connected to " << server_address << std::endl;

        // Subscribe to all messages
        subscriber.set(zmq::sockopt::subscribe, "");

        while (running) {
            zmq::message_t message;
            zmq::recv_result_t result = subscriber.recv(message, zmq::recv_flags::none);

            if (result.has_value() && *result > 0) {
                std::string receivedData(static_cast<char*>(message.data()), message.size());
                std::cout << "Received: " << receivedData << std::endl;
            } else {
                std::cerr << "Failed to receive message. Retrying..." << std::endl;
            }
        }
    } catch (const zmq::error_t& e) {
        std::cerr << "ZeroMQ error: " << e.what() << std::endl;
    }

    std::cout << "Subscriber shutting down..." << std::endl;
    subscriber.close();
    context.shutdown();
    context.close();

    return 0;
}
