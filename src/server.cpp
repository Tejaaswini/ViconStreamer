#include "vicon_streamer/vicon_streamer.h"

int main() {
    vicon_streamer::ViconStreamer streamer("127.0.0.1", 5556);
    streamer.connect();
    streamer.streamData();
    streamer.disconnect();
    return 0;
}