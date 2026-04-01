# 🐦 The Pigeon: UDP Linux Remote
A low-latency, cross-platform remote control system designed for Linux power users. This project turns an Android device into a high-precision trackpad and media controller for KDE/GNOME desktops using the Linux kernel's `uinput` interface.

## 🚀 The Problem
I wanted to control my PC from the couch (mostly for *Family Guy* marathons) without using bloated, ad-filled proprietary apps or fiddling with manual IP entries every time my router decided to reboot.
Unfortunately, this was before I discovered KDE Connect for Fedora-based systems. I still use my own app, though.

## 🛠️ Features
* **Zero-Config Discovery:** Uses a custom UDP Handshake protocol. The phone "shouts" into the network, and the PC identifies itself—no IP typing required.
* **Kernel-Level Input:** Interfaces directly with `/dev/uinput` to create a virtual hardware device (Mouse + Keyboard + Media Keys).
* **Smart Trackpad:** High-frequency UDP packets ensure the cursor movement feels native and lag-free.
* **Media Grid:** Dedicated controls for Play/Pause, Volume, and Track navigation.
* **Modern UI:** Built with Jetpack Compose and Material 3 for a clean, responsive mobile experience.

---

## 🏗️ Architecture
The system consists of two main components:
1.  **The Server (C):** A lightweight background daemon that listens for handshake requests and translates incoming UDP envelopes into Linux input events.
2.  **The Client (Kotlin/Compose):** An Android app that captures touch gestures and media commands, packaging them into a custom binary protocol.

---

## 🔧 Setup & Installation

### 1. PC (Linux)
**Requirements:** `gcc`, `libudev-dev`, and `pthread`.
1.  Clone the repo.
2.  Compile the server:
    ```bash
    make
    ```
3.  Run the server:
    ```bash
    sudo ./pigeon_server
    ```
    *(Note: Sudo is required to access `/dev/uinput` unless udev rules are configured.)*

### 2. Mobile (Android)
1.  Open the project in Android Studio.
2.  Build and install the APK to your device.
3.  Ensure both devices are on the same Wi-Fi network.
4.  Hit **"Search for PC"** and start controlling.

---

## 📜 Protocol Definition
The "Pigeon" uses a custom 12-byte envelope for maximum speed and minimal overhead:
* **Magic Number:** `0xAFAF` (Safety check)
* **Command ID:** 1 byte (Move, Click, Media, etc.)
* **Data:** X/Y coordinates or specific keycodes.

---

## ⚖️ License
Educational purpose only. Built with a touch of wit and a lot of C.