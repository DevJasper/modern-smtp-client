//
//  Socket.cpp
//  smtp
//
//  Created by JASPER on 21/03/2023.
//

#include "Socket.hpp"

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

Socket::Socket(const std::string& _host, int _port) {
  SocketAddress address{.host = _host, .port = _port};
  connect(address);
};

Socket::Socket(int _fd) : fd(_fd){};

Socket::~Socket() { disconnect(); };

void Socket::bind(SocketAddress& _address) {}

void Socket::connect(SocketAddress& _address) {
  constexpr int timeout = 1;
  connect(_address, timeout);
}

void Socket::connect(SocketAddress& _address, int _timeoutInSeconds) {
  address = _address;
  struct addrinfo hints, *results;
  std::memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  int errorCode =
      ::getaddrinfo(address.host.data(), std::to_string(address.port).data(),
                    &hints, &results);

  if (errorCode)
    throw std::runtime_error("Failed to get address information: " +
                             std::string(gai_strerror(errorCode)));

  std::string error;

  for (auto result = results; result; result = result->ai_next) {
    fd = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (fd < 0) {
      error = "Failed to establish a socket connection";
      continue;
    }

    setSoTimeout(_timeoutInSeconds);

    if (::connect(fd, result->ai_addr, result->ai_addrlen) < 0) {
      error = "Failed to connect to socket";
      ::close(fd);
      fd = -1;
      continue;
    }

    break;
  }

  if (fd < 0) throw std::runtime_error(error);

  connected = true;
}

void Socket::disconnect() {
  if (!connected) return;

  ::close(fd);
  fd = -1;
  connected = false;
}

template <ByteReader T>
int Socket::read(T& _reader) {
  const std::size_t lengthOfBytesToRead = _reader.capacity();
  std::size_t bytesLeftToRead = lengthOfBytesToRead;

  std::string buffer;
  buffer.reserve(lengthOfBytesToRead);

  std::size_t totalBytesRead = 0;

  do {
    const ssize_t bytesRead = ::recv(fd, buffer.data(), bytesLeftToRead, 0);

    if (bytesRead < 0) {
      // handle read error

      break;
    } else if (bytesRead == 0) {
      // EOF
      break;
    }

    std::copy(buffer.begin(), buffer.begin() + bytesRead,
              std::back_inserter(_reader));

    totalBytesRead += static_cast<std::size_t>(bytesRead);
    bytesLeftToRead -= bytesRead;

  } while (totalBytesRead < lengthOfBytesToRead);

  return static_cast<int>(totalBytesRead);
}

// Explicit instantiations for supported types to reduce code bloat
template int Socket::read<std::vector<char>>(std::vector<char>& reader);
template int Socket::read<std::string>(std::string& reader);

void Socket::setSoTimeout(int _seconds) {
  struct timeval tv {
    .tv_sec = _seconds
  };
  setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);
}

int Socket::write(const std::string&& buffer) {
  ssize_t bytesSent = ::send(fd, buffer.data(), buffer.length(), 0);
  if (bytesSent == -1) throw std::runtime_error("Error sending to socket");
  return static_cast<int>(bytesSent);
}
