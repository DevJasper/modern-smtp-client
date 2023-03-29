//
//  MailTransport.hpp
//  smtp
//
//  Created by JASPER on 22/03/2023.
//

#ifndef MailTransport_hpp
#define MailTransport_hpp

#include <string>
#include <vector>

#include "TLSSocket.hpp"
#include "Socket.hpp"

class MailTransport{
public:
    explicit MailTransport();
    void send();
private:
    [[nodiscard]]std::vector<std::string> resolveMXRecords(const std::string&);
    void handle(Socket&);
//    void handle(TLSSocket&);
    
};

#endif /* MailTransport_hpp */
