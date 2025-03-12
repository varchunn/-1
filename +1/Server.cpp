#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sstream>  // Для конвертації числа в рядок
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Створення сокету
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Помилка створення сокету\n";
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Прив'язка сокету
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
        std::cerr << "Помилка прив’язки сокету\n";
        close(server_fd);
        return 1;
    }

    // Очікування підключення
    if (listen(server_fd, 3) == -1) {
        std::cerr << "Помилка очікування підключення\n";
        close(server_fd);
        return 1;
    }

    std::cout << "Сервер очікує підключення...\n";

    // Прийняття підключення
    new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (new_socket == -1) {
        std::cerr << "Помилка прийняття підключення\n";
        close(server_fd);
        return 1;
    }

    std::cout << "Клієнт підключився.\n";

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int valread = recv(new_socket, buffer, sizeof(buffer), 0);
        if (valread <= 0) {
            std::cout << "Клієнт відключився.\n";
            break;
        }

        int received_number = atoi(buffer);
        int incremented_number = received_number + 1;

        // Використання stringstream замість std::to_string()
        std::stringstream ss;
        ss << incremented_number;
        std::string response = ss.str();

        send(new_socket, response.c_str(), response.length(), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
