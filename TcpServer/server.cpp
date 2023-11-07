#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

const char kaspersky[] = "KASPERSKY\n";

int main() {
    int portNum = 8080;
    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        cerr << "Can't create a socket! Quitting" << endl;
        return -1;
    }

    // Bind the IP address and port to the socket
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);
    server_addr.sin_addr.s_addr = INADDR_ANY; //  INADDR_ANY to listen on all interfaces
    if (bind(listening, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Can't bind to IP/Port! Quitting" << endl;
        return -1;
    }

    int cnt = 0;

    if (listen(listening, SOMAXCONN) < 0) {
        cerr << "Error during listening! Quitting" << endl;
        return -1;
    }

    cout << "Server ready to accept calls on port: " << ntohs(server_addr.sin_port) << "\n";

    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    // Wait for calls
    while (true) {
        int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        if (clientSocket < 0) {
            cerr << "Can't accept client call\n";
            continue; // Skip the current attempt and continue listening
        }

        send(clientSocket, kaspersky, strlen(kaspersky), 0);
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client.sin_addr, clientIP, INET_ADDRSTRLEN);
        close(clientSocket);

        cnt++;
        cout << "Message sent to client #" << cnt << ": " << clientIP << '\n';
    }

    // Close the server socket
    close(listening);
    return 0;
}
