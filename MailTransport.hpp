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

#include "Socket.hpp"
#include "TLSSocket.hpp"

class MailTransport {
 public:
  explicit MailTransport();
  void send();

 private:
  [[nodiscard]] std::vector<std::string> resolveMXRecords(const std::string&);
    template<class SocketType>
  void handle(SocketType&);
};

#endif /* MailTransport_hpp */
