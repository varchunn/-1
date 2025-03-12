#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Створення сокету
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Помилка створення сокету\n";
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Невірна IP-адреса\n";
        close(sock);
        return 1;
    }

    // Підключення до сервера
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        std::cerr << "Помилка підключення до сервера\n";
        close(sock);
        return 1;
    }

    std::cout << "Підключено до сервера!\n";

    while (true) {
        std::string input;
        std::cout << "Введіть ціле число (або 'exit' для виходу): ";
        std::cin >> input;

        if (input == "exit") break;

        send(sock, input.c_str(), input.length() + 1, 0);

        memset(buffer, 0, sizeof(buffer));
        int valread = recv(sock, buffer, sizeof(buffer), 0);
        if (valread <= 0) {
            std::cout << "Сервер закрив з'єднання.\n";
            break;
        }

        std::cout << "Сервер надіслав: " << buffer << std::endl;
    }

    close(sock); // Закриття сокету для macOS
    return 0;
}
