#include "vicon_streamer/vicon_streamer.h"
#include <iostream>
#include <thread>
#include <atomic>
#include "DataStreamClient.h"

namespace vicon_streamer {

ViconStreamer::ViconStreamer(const std::string& host, int port)
    : host_(host), port_(port), context_(1), publisher_(context_, ZMQ_PUB), stop_streaming_(false) {
    publisher_.bind("tcp://" + host_ + ":" + std::to_string(port_));
}

void ViconStreamer::connect() {
    std::cout << "Connecting to Vicon system..." << std::endl;

    client_ = std::make_unique<ViconDataStreamSDK::CPP::Client>();

    if (client_->Connect(host_).Result != ViconDataStreamSDK::CPP::Result::Success) {
        std::cerr << "Failed to connect to Vicon at " << host_ << std::endl;
        client_.reset(); // Ensure client_ is set to nullptr if connection fails
        return;
    }

    client_->EnableSegmentData();
    client_->EnableMarkerData();
    client_->EnableUnlabeledMarkerData();
    client_->EnableDeviceData();

    while (client_->GetFrame().Result != ViconDataStreamSDK::CPP::Result::Success) {
        std::cout << "Waiting for first frame..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Connected to Vicon system at " << host_ << std::endl;
}

void ViconStreamer::streamData() {
    if (!client_) {
        std::cerr << "Error: Not connected to Vicon system. Call connect() first." << std::endl;
        return;
    }

    stop_streaming_ = false; // Ensure streaming starts
    std::cout << "Starting data stream..." << std::endl;

    while (!stop_streaming_) {
        if (client_->GetFrame().Result == ViconDataStreamSDK::CPP::Result::Success) {
            std::string subjectName = client_->GetSubjectName(0).SubjectName;
            double x, y, z;
            client_->GetSegmentGlobalTranslation(subjectName, "SegmentName", x, y, z);

            std::string message = subjectName + "," + std::to_string(x) + "," +
                                  std::to_string(y) + "," + std::to_string(z);

            zmq::message_t zmqMessage(message.size());
            memcpy(zmqMessage.data(), message.data(), message.size());
            publisher_.send(zmqMessage, zmq::send_flags::none);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::cout << "Data streaming stopped." << std::endl;
}

void ViconStreamer::disconnect() {
    std::cout << "Disconnecting from Vicon system..." << std::endl;

    stop_streaming_ = true; // Stop the streaming loop

    if (client_) {
        client_->Disconnect();
        client_.reset();
    }

    publisher_.close();
    context_.shutdown();
    context_.close();

    std::cout << "Disconnected successfully." << std::endl;
}

}
