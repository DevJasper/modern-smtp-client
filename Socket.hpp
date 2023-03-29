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


template <typename T>
concept ByteReader = requires (T c) {
    typename T::value_type;
    { c.size() } -> std::convertible_to<std::size_t>;
    { c.capacity() } -> std::convertible_to<std::size_t>;
    { c.begin() } -> std::same_as<typename T::iterator>;
    { c.end() } -> std::same_as<typename T::iterator>;
};


class Socket{
    
public:
    explicit Socket() = default;
    explicit Socket(const std::string&, int);
    explicit Socket(int);
    ~Socket();
    void bind(SocketAddress&);
    void connect(SocketAddress&);
    void connect(SocketAddress&, int);
    void disconnect();
    SocketAddress getAddress() const noexcept{return address;};
    inline int getFd() const noexcept{return fd;};
    inline bool isConnected() const noexcept{return connected;};
    template<ByteReader T> int read(T&);
    void setSoTimeout(int);
    int write(const std::string&&);
private:
    SocketAddress address;
    int fd = -1;
    bool connected = false;
};


#endif /* Socket_hpp */
