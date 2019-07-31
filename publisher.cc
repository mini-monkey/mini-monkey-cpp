#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void usage(char *argv[])
{
  std::cerr << argv[0] << " <host> <port> <token> <room> <msg>" << std::endl;
}

class Sender {
public:
  Sender(const std::string& host, const std::string& port) {
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), host, port);
    tcp::resolver::iterator iterator = resolver.resolve(query);

    mSocket = std::make_unique<tcp::socket>(io_service);
    boost::asio::connect(*mSocket, iterator);
  }

  void send(char code, const std::string &data) {
    auto size = data.size();
    char high = size >> 8;
    char low = size & 0xff;

    char header[] = {code, low, high};
    boost::asio::write(*mSocket, boost::asio::buffer(header, sizeof(header)));
    boost::asio::write(*mSocket, boost::asio::buffer(data.c_str(), data.size()));
  }

  void recv() {
    char header[3];
    boost::asio::read(*mSocket, boost::asio::buffer(header, sizeof(header)));
    size_t size = (header[2]>>8) + header[1];
    char buf[size+1] = {};
    boost::asio::read(*mSocket, boost::asio::buffer(buf, size));
    std::cerr << buf << std::endl;
  }

  void auth(const std::string &token) { send(1, token); recv();}
  void enter(const std::string &room) { send(2, room); recv();}
  void publish(const std::string &message) { send(3, message); recv();}

private:
  std::unique_ptr<tcp::socket> mSocket;
};

int main(int argc, char* argv[])
{
  if (argc != 6) {
    usage(argv);
    return -1;
  }

  Sender sender(argv[1], argv[2]);
  sender.auth(argv[3]);
  sender.enter(argv[4]);
  sender.publish(argv[5]);
}
