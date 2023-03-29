//
//  TLSTLSSocket.cpp
//  smtp
//
//  Created by JASPER on 28/03/2023.
//

#include "TLSSocket.hpp"

#include <algorithm>
#include <iterator>
#include <cstring>
#include <iostream>
#include <poll.h>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>


TLSSocket::TLSSocket(){
    init();
}

TLSSocket::TLSSocket(const std::string& _host, int _port){
    init();
    SocketAddress address{.host=_host, .port=_port};
    connect(address);
};

TLSSocket::TLSSocket(Socket& socket){
    init();
    upgrade(socket);
};

TLSSocket::~TLSSocket(){
    disconnect();
};

void TLSSocket::bind(SocketAddress& _address){}

void TLSSocket::connect(SocketAddress& _address){
    constexpr int timeout = 1;
    connect(_address, timeout);
    
}

void TLSSocket::connect(SocketAddress& _address, int _timeoutInSeconds){
    
    if (::tls_connect(ctx, _address.host.data(), std::to_string(_address.port).data()) < 0) throw std::runtime_error("Failed to establish a socket connection");
    
//    setSoTimeout(_timeoutInSeconds);
    
    
    connected = true;
    
}

void TLSSocket::disconnect(){
    if(!connected) return;
    ::tls_close(ctx);
    ::tls_free(ctx);
    ::tls_config_free(config);
    connected = false;
    
}

void TLSSocket::init(){
    
    ::tls_init();
    
    if ((ctx = ::tls_client()) == nullptr) throw std::runtime_error("Failed to create TLS client");
    
    if ((config = ::tls_config_new()) == nullptr) throw std::runtime_error("Failed to create TLS config");
    
//    if(::tls_config_set_ca_file(config, "/Users/jasper/Projects/CPP/Vanilla/smtp/smtp/ca-cert.pem") < 0) throw std::runtime_error("Failed to set TLS certificate");
    
    if (::tls_config_set_protocols(config, TLS_PROTOCOLS_DEFAULT) < 0) throw std::runtime_error("Failed to set TLS protocol");
    
    if (::tls_configure(ctx, config) < 0) throw std::runtime_error("Failed to configure TLS");
}


template<ByteReader T> int TLSSocket::read(T& _reader){
    const std::size_t lengthOfBytesToRead = _reader.capacity();
    std::size_t bytesLeftToRead = lengthOfBytesToRead;
    
    std::string buffer;
    buffer.reserve(lengthOfBytesToRead);
    
    std::size_t totalBytesRead = 0;
    
    do {
        const ssize_t bytesRead = ::tls_read(ctx, buffer.data(), bytesLeftToRead);
        
        if (bytesRead < 0) {
            // handle read error
            
            break;
        } else if (bytesRead == 0) {
            // EOF
            break;
        }
        
        std::copy(buffer.begin(), buffer.begin()+bytesRead, std::back_inserter(_reader));
        
        totalBytesRead += static_cast<std::size_t>(bytesRead);
        bytesLeftToRead-=bytesRead;
        
        
    } while (totalBytesRead < lengthOfBytesToRead);
    
    return static_cast<int>(totalBytesRead);
}

//Explicit instantiations for supported types to reduce code bloat
template int TLSSocket::read<std::vector<char>>(std::vector<char>& reader);
template int TLSSocket::read<std::string>(std::string& reader);


void TLSSocket::setSoTimeout(int _seconds){
    //    if (::tls_config_set_bio_timeout(config, 5000) < 0) throw "Failed to set TLS timeout";
}

void TLSSocket::upgrade(Socket& socket){
    
    
    
    if (::tls_connect_socket(ctx, socket.getFd(), socket.getAddress().host.data()) < 0) throw std::runtime_error("Failed to attach socket to TLS session");
    
    connected = true;
    
    //    fd = socket.getFd();
    //    address = socket.getAddress();
}

int TLSSocket::write(const std::string&& buffer){
    ssize_t bytesSent = ::tls_write(ctx, buffer.data(), buffer.length());
    if(bytesSent == -1) throw std::runtime_error("Error sending to socket");
    return static_cast<int>(bytesSent);
}


