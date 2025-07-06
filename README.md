# 🐉 StealthLogger-X11

> A minimal, obfuscated Linux keylogger for **educational**, **research**, and **red team simulation** purposes.

---

## 🚨 Legal & Ethical Disclaimer

This tool is provided **strictly for educational and authorized testing** within controlled environments (e.g., cybersecurity labs, red team simulations).  
**Do not use** this software on unauthorized systems or without explicit permission.  
The author takes **no responsibility** for misuse.

---

## 🎯 Features

- 📡 Periodic exfiltration of captured keystrokes via TCP (to a listening server)
- 🔍 Logs stored temporarily in a stealthy location (`~/.cache/.update.log`)
- 🧠 XOR-encrypted strings for evasion
- 🎭 Daemon-like stealth mode (forks into background)
- 💻 Built for X11-based Linux environments

---

## ✅ Tested Environment

- Kali Linux (Debian-based)
- X11 Display Server (required)
- Local listener at `127.0.0.1:4444`

---

## ⚙️ Setup

### 🔧 1. Compile the Keylogger
```bash
g++ main_linux.cpp -o keylogger -lX11

