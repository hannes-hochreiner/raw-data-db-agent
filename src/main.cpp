#include "main.hpp"

int main(int argc, char const *argv[]) {
  try {
    std::cout << "...starting raw data agent...\n";

    repo repo;

    zmq::context_t context (1);
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5555");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    zmq::message_t update;

    while (true) {
      subscriber.recv(&update);
      std::string msg(update.data<char>(), update.size());
    
      try {
        std::cout << msg << "\n";

        auto j = json::parse(msg);

        repo.store_measurement(j);
      } catch (const std::exception& e) {
        std::cerr << "error processing message: " << msg << "\n";
        std::cerr << e.what() << '\n';
      }
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  return 0;
}
