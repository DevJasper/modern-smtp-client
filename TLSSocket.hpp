//
//  TLSSocket.hpp
//  smtp
//
//  Created by JASPER on 28/03/2023.
//

#ifndef TLSSocket_hpp
#define TLSSocket_hpp

#include <tls.h>

#include <concepts>
#include <string>

#include "Socket.hpp"
#include "SocketAddress.hpp"


class TLSSocket : public SocketType {
 public:
  explicit TLSSocket();
  explicit TLSSocket(const std::string &, int);
  explicit TLSSocket(Socket &);
  ~TLSSocket();
  void bind(SocketAddress &)override;
  void connect(SocketAddress &)override;
  void connect(SocketAddress &, int)override;
  void disconnect()override;
  //    SocketAddress getAddress() const noexcept{return address;};
  //    inline int getFd() const noexcept{return fd;};
  inline bool isConnected() const noexcept override{ return connected; };
  template <ByteReader T>
  int read(T &);
  void setSoTimeout(int)override;
  int write(const std::string &&)override;

 private:
  void init();
  void upgrade(Socket &);
  //    SocketAddress address;
  //    int fd = -1;
  bool connected = false;
  struct tls *ctx = nullptr;
  struct tls_config *config = nullptr;
};

#endif /* TLSSocket_hpp */
