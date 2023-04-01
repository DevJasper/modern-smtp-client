//
//  MailTransport.cpp
//  smtp
//
//  Created by JASPER on 22/03/2023.
//

#include "MailTransport.hpp"

#include <arpa/nameser.h>
#include <netdb.h>
#include <resolv.h>

#include <array>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "SocketAddress.hpp"


template <class SocketType>
void readFromSocket(SocketType& socket) {
  std::vector<char> buffer;
  buffer.reserve(1024);

  socket.read(buffer);
  std::string response(buffer.begin(), buffer.end());

  std::cout << response << '\n';
  //    return std::stoi(std::string(buffer.begin(), buffer.begin()+3));
}

//
//void readFromSocket(TLSSocket& socket) {
//  std::vector<char> buffer;
//  buffer.reserve(1024);
//
//  socket.read(buffer);
//  std::string response(buffer.begin(), buffer.end());
//
//  std::cout << response << '\n';
//  //    return std::stoi(std::string(buffer.begin(), buffer.begin()+3));
//}

template <class SocketType>
void writeToSocket(SocketType& socket, std::string&& buffer) {
  socket.write(buffer + "\r\n");
  std::cout << buffer << '\n';
}

//void writeToSocket(TLSSocket& socket, std::string&& buffer) {
//  socket.write(buffer + "\r\n");
//  std::cout << buffer << '\n';
//}

MailTransport::MailTransport() { res_init(); }

template<class SocketType>
void MailTransport::handle(SocketType& socket) {
  //    int statusCode;

  readFromSocket(socket);
  writeToSocket(socket, "EHLO trygot.com");
  readFromSocket(socket);
  writeToSocket(socket, "STARTTLS");
  readFromSocket(socket);
  TLSSocket tlsSocket(socket);
  writeToSocket(tlsSocket, "EHLO trygot.com");
  readFromSocket(tlsSocket);
  writeToSocket(tlsSocket, "MAIL FROM: <jasper@trygot.com>");
  readFromSocket(tlsSocket);
  writeToSocket(tlsSocket, "RCPT TO: <developerjasper@gmail.com>");
  readFromSocket(tlsSocket);
  writeToSocket(tlsSocket, "DATA");
  readFromSocket(tlsSocket);
  //    writeToSocket(tlsSocket,"MIME-Version: 1.0"
  //                  "Subject: Test message"
  //                  "From: Senior Tester <jasper@trygot.com>"
  //                  "Content-Type: text/html; charset=\"UTF-8\""
  //                  "Content-Transfer-Encoding: quoted-printable"
  //                  "To: developerjasper@gmail.com>"
  //                  "<b>Cool email body</b>"
  //                  ""
  //                  "."
  //                  );

  writeToSocket(
      tlsSocket,
      "MIME-Version: 1.0\r\nSubject: TEST EMAIL\r\nFrom: Jasper "
      "Bernerd<jasper@trygot.com>\r\nContent-Type: text/html; "
      "charset='UTF-8'\r\nContent-Transfer-Encoding: quoted-printable\r\nTo: "
      "<developerjasper@gmail.com>\r\nThis is my test email\r\n.");
  readFromSocket(tlsSocket);
  writeToSocket(tlsSocket, "QUIT");
  readFromSocket(tlsSocket);
}

std::vector<std::string> MailTransport::resolveMXRecords(
    const std::string& _host) {
  std::vector<std::string> records;

  std::array<unsigned char, NS_MAXDNAME + 1> responseBuffer;

  const int responseLength =
      res_query(_host.c_str(), ns_c_in, ns_t_mx, responseBuffer.data(),
                responseBuffer.size());

  if (responseLength < 0) {
    std::string error;
    switch (h_errno) {
      case HOST_NOT_FOUND:
        error = "Host not found";
        break;
      case NO_DATA:
        error = "No data for the requested query type";
        break;
      case TRY_AGAIN:
        error = "Temporary DNS error, try again later";
        break;
      default:
        error = "Unknown error";
        break;
    }

    throw std::runtime_error("Failed to query DNS for MX record for (" + _host +
                             "): " + error);
  }

  ns_msg handle;
  ns_rr rr;
  ns_initparse(responseBuffer.data(), responseLength, &handle);
  const int mxCount = ns_msg_count(handle, ns_s_an);

  for (int i = 0; i < mxCount; ++i) {
    if (ns_parserr(&handle, ns_s_an, i, &rr) == 0 &&
        ns_rr_type(rr) == ns_t_mx) {
      char mxName[NS_MAXDNAME];
      if (ns_name_uncompress(
              responseBuffer.data(), responseBuffer.data() + responseLength,
              ns_rr_rdata(rr) + NS_INT16SZ, mxName, sizeof(mxName)) < 0) {
        continue;
      }

      records.emplace_back(mxName);
    }
  }

  return records;
}

void MailTransport::send() {
  std::string host = "gmail.com";

  auto records = resolveMXRecords(host);

  if (records.size() == 0)
    throw std::runtime_error("Failed to resolve MX records for " + host);

    Socket socket;

  for (const auto& record : records) {
    std::cout << record << '\n';
    SocketAddress address{.host = record, .port = 25};
    socket.connect(address);

    if (socket.isConnected()) break;
  }

  handle(socket);
}
