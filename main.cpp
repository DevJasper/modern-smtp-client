//
//  main.cpp
//  smtp
//
//  Created by JASPER on 21/03/2023.
//

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "MailTransport.hpp"
#include "Socket.hpp"
#include "SocketAddress.hpp"
#include "TLSSocket.hpp"

int main(int argc, const char* argv[]) {
  try {
    //        Socket s;
    //        SocketAddress address{.host = "gmail.com", .port=80};
    //        s.connect(address);
    //        std::vector<char> data;
    //        int bytesSent= s.write("GET / HTTP/1.1\r\n\r\n");
    //        std::cout << "Bytes Sent " << bytesSent << '\n';
    //        s.read(data);
    //        std::cout << "wow2" << '\n';
    //        std::string str(data.begin(), data.end());
    //        std::cout << str << '\n';

    //        Socket s;
    //        s.connect(address);

    //        std::cout << s.getFd() << '\n';
    //        TLSSocket s1(s);

    //        SocketAddress address{.host = "alt1.gmail-smtp-in.l.google.com",
    //        .port=587};
    SocketAddress address{.host = "gmail.com", .port = 443};

    TLSSocket s1;
    s1.connect(address);

    //        std::vector<char> data;
    //        data.reserve(2048);
    //
    //        int bytesSent= s1.write("GET / HTTP/1.1\r\n\r\n");
    //        std::cout << "Bytes Sent " << bytesSent << '\n';
    //        int bytesRead = s1.read(data);
    //        std::cout << "Bytes Read " << bytesRead << '\n';
    //        std::cout << data[0] << '\n';
    //        std::string str(data.begin(), data.end());
    //        std::cout << str << '\n';
    //
    MailTransport transport;

    //        Email mail;
    //        mail.setSender("jasper@trygot.com");
    //        mail.addRecipient("developerjasper@gmail.com");
    //        mail.addRecipient("famewind@gmail.com");
    //        mail.setSubject("My test email");
    //        mail.write("<p>Hello <h2>Jasper</h2>, This is a test email </p>");

    //        mail.text("Hello Jasper, this is a test mail");
    //        mail.html("<p>Hello <h2>Jasper</h2>, This is a test email </p>");

    //        transport.send(mail);

    //        transport.add();

    transport.send();

  } catch (std::runtime_error& err) {
    std::cout << err.what() << '\n';
  }
  return 0;
}
