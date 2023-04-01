//
//  Socket.hpp
//  smtp
//
//  Created by JASPER on 21/03/2023.
//

#ifndef Socket_hpp
#define Socket_hpp

#include <concepts>
#include <string>

#include "SocketAddress.hpp"
#include "SocketType.hpp"


class Socket : public SocketType {
public:
    explicit Socket() = default;
    explicit Socket(const std::string&, int);
    explicit Socket(int);
    ~Socket();
    void bind(SocketAddress&) override;
    void connect(SocketAddress&) override;
    void connect(SocketAddress&, int) override;
    void disconnect()override;
    SocketAddress getAddress() const noexcept { return address; };
    inline int getFd() const noexcept { return fd; };
    inline bool isConnected() const noexcept override{ return connected; };
    template <ByteReader T>
    int read(T&);
    void setSoTimeout(int)override;
    int write(const std::string&&)override;
    
private:
    SocketAddress address;
    int fd = -1;
    bool connected = false;
};

#endif /* Socket_hpp */
