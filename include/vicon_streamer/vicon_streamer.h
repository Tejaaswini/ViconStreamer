#ifndef VICON_STREAMER_H
#define VICON_STREAMER_H

#include <string>
#include <zmq.hpp>

namespace vicon_streamer {

class ViconStreamer {
public:
    explicit ViconStreamer(const std::string& host, int port);
    ~ViconStreamer() noexcept = default;

    bool connect() noexcept;
    bool streamData() noexcept;
    void disconnect() noexcept;

    [[nodiscard]] bool isConnected() const noexcept;

private:
    std::string host_;
    int port_;
    zmq::context_t context_;
    zmq::socket_t publisher_;
    bool connected_ = false;
};

}

#endif
