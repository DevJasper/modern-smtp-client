//
//  SocketType.hpp
//  smtp
//
//  Created by JASPER on 01/04/2023.
//

#ifndef SocketType_hpp
#define SocketType_hpp

#include "SocketAddress.hpp"


template <typename T>
concept ByteReader = requires(T c) {
  typename T::value_type;
  { c.size() } -> std::convertible_to<std::size_t>;
  { c.capacity() } -> std::convertible_to<std::size_t>;
  { c.begin() } -> std::same_as<typename T::iterator>;
  { c.end() } -> std::same_as<typename T::iterator>;
};


class SocketType{
public:
    virtual void bind(SocketAddress&) = 0;
    virtual void connect(SocketAddress&) = 0;
    virtual void connect(SocketAddress&, int)  = 0;
    virtual void disconnect() = 0;
    virtual inline bool isConnected()const noexcept = 0;
    virtual void setSoTimeout(int) = 0;
    virtual int write(const std::string&&) = 0;
};

#endif /* SocketType_hpp */
