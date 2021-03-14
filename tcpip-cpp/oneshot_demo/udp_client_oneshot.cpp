#include <demo_config.h>
#include <core/tcpipCommon.h>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>

#elif defined(__unix__)

#include <netdb.h>
#include <unistd.h>

#endif

#include <vector>
#include <iostream>

int udp_client_oneshot(std::string server_address) {
    struct addrinfo *result = nullptr;
    struct addrinfo hints = {};

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    socket_t server_socket = INVALID_SOCKET;

    /* Resolve the server address and port */
    int retval = 0;
    if(server_address == "any" or server_address == "") {
        retval = getaddrinfo(nullptr, tcpip::SERVER_PORT, &hints, &result);
    }
    else {
        retval = getaddrinfo(server_address.c_str(), tcpip::SERVER_PORT,  &hints, &result);
    }

    if (retval != 0) {
        std::cerr << "udp_client_oneshot: getaddrinfo failed with error: " <<
                retval << std::endl;
        return 1;
    }
    /* Create a socket for connecting to server */
    server_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (server_socket < 0) {
        std::cerr << "udp_client_oneshot: socket failed with error: " <<
                tcpip::get_last_error() << std::endl;
        freeaddrinfo(result);
        return 1;
    }

    std::string string = "This is a UDP test";
    std::cout << "Client: Sending string with " << string.size() << " bytes: " <<
            string << std::endl;
    retval = sendto(server_socket, string.data(), string.size(), 0, result->ai_addr,
            result->ai_addrlen);
    if(retval > 0) {
        std::cout << "Client: Sent string with " << string.size() << " bytes successfully"
                << std::endl;
    }
    else {
        int error = tcpip::get_last_error();
        std::cerr << "UdpClientClass::perform_send_operation: sendto failed with error: " <<
                 error << std::endl;
    }
    return 0;
}
