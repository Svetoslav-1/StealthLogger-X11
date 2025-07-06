# 🐉 StealthLogger-X11

> A minimal, obfuscated Linux keylogger for **educational**, **research**, and **red team simulation** purposes.

---

## 🚨 Legal & Ethical Disclaimer

This tool is provided **strictly for educational and authorized testing** within controlled environments (such as cybersecurity labs and red team simulations).  
**Do not use** this software on unauthorized systems or without explicit written permission.  
The author assumes **no responsibility** for misuse.

---

## 🎯 Features

- Periodic exfiltration of captured keystrokes via TCP
- Logs stored in a stealthy location: `~/.cache/.update.log`
- XOR-encrypted strings to bypass static detection
- Daemon-style execution via process forking on Linux
- Built for X11-based environments (required)

---

## ✅ Tested Environment

- Kali Linux (Debian-based)
- X11 Display Server
- Localhost communication using `127.0.0.1:4444`
- **Note:** Windows support is included but **has not been tested**

---

## ⚙️ Setup Instructions

### 1. Compile the Keylogger (Linux)

Use a C++ compiler with X11 support to compile the main file:
- Source file: `main_linux.cpp`
- Output binary: `keylogger`
- Library dependency: `X11`

### 2. Run the Keylogger

Execute the compiled binary in the background.  
It will begin recording keystrokes and sending logs to the listener every 30 seconds (default).

---

## 🖥️ Listener Setup

### 1. Run the Listener Script

Use the `keylogger_server.py` script to receive log data over TCP.  
The listener will store received logs in `/tmp/keylogs/` using timestamped filenames.

---

## 🔓 Decode Keystroke Logs

### 1. Use the Decoder

Run the `decode_log.py` script with the path to a `.log` file to translate raw X11 keycodes into readable characters.

---

## 📂 File Overview

- `main_linux.cpp`: Linux keylogger (obfuscated, tested)
- `main_windows.cpp`: Windows version (included, not yet tested)
- `keylogger_server.py`: TCP server that receives and stores log files
- `decode_log.py`: Keycode-to-character log decoder for Linux logs

---

## 🔬 Lab Notes

- You can replace `127.0.0.1` in the source code with your own IP address for remote testing.
- Persistence can be added using `systemd` for Linux or scheduled tasks on Windows.
- Further obfuscation and packers may be used for red team lab evasion testing.

---

## 🛡️ License

This project is released for **ethical research and educational purposes only**.  
**Unauthorized deployment or misuse is strictly prohibited.**
