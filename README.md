# 🛒 GTK-Based Shopping Cart Application (C, GTK, SQLite)

A **desktop shopping cart application** developed in **C using GTK 3 and SQLite**, featuring user authentication, dynamic product handling, and checkout billing.
This project demonstrates **system-level programming**, **GUI development**, and **database integration** in a real-world application.

---

## 📌 Why This Project Matters

This application showcases my ability to:

* Build **GUI-based desktop software in C**
* Integrate **SQLite databases** with front-end logic
* Design **event-driven applications**
* Manage **state (cart & quantities)** efficiently
* Write **clean, modular, and maintainable C code**

Ideal for roles involving **software development**, **systems programming**, or **desktop application development**.

---

## 🚀 Key Features

### 🔐 Authentication System

* Secure **Login & Signup** flow
* User credentials stored in SQLite
* Automatic database initialization

### 🛍️ Product Management

* Product catalog with:

  * Images
  * Prices
  * Descriptions
* Scrollable grid layout using `GtkFlowBox`

### ➕➖ Cart Functionality

* Increment / decrement item quantities
* Centralized quantity state management
* Add multiple items in one action

### 🧾 Checkout & Billing

* Dynamic bill generation
* Item-wise cost calculation
* Displays total items & total amount
* Payment QR display at checkout

### 🎨 User Interface

* GTK 3 widgets
* External CSS-based styling
* Responsive layout using GTK containers

---

## 🧠 Technical Skills Demonstrated

* **C Programming (Pointers, Memory, Structuring)**
* **GTK 3 GUI Development**
* **SQLite Database Operations**
* **Event-Driven Architecture**
* **Desktop Application Design**
* **Cross-Platform Build Setup**
* **UI/UX Structuring in Low-Level Languages**

---

## 🛠️ Tech Stack

| Component | Technology      |
| --------- | --------------- |
| Language  | C               |
| GUI       | GTK 3           |
| Database  | SQLite 3        |
| Styling   | GTK CSS         |
| Platform  | Windows / Linux |

---

## 📂 Project Structure

```
GTK-Shopping-Cart/
│
├── main.c               # Core application logic
├── style.css            # GTK UI styling
├── shopping_cart.db     # SQLite database (auto-created)
│
├── images/
│   ├── apple.jpg
│   ├── banana.jpg
│   ├── milk.jpg
│   ├── bread.jpg
│   ├── eggs.jpg
│   └── qr.jpg
│
└── README.md
```

---

## ⚙️ Setup & Installation

### 🔹 Prerequisites

* GCC / MinGW
* GTK 3
* SQLite 3

### Linux (Ubuntu/Debian)

```bash
sudo apt install libgtk-3-dev sqlite3 libsqlite3-dev
```

### Windows (MSYS2 / MinGW)

```bash
pacman -S mingw-w64-x86_64-gtk3 mingw-w64-x86_64-sqlite3
```

---

## ▶️ Build & Run

### Linux

```bash
gcc main.c -o shopping_cart `pkg-config --cflags --libs gtk+-3.0 sqlite3`
./shopping_cart
```

### Windows

```bash
gcc main.c -o shopping_cart.exe ^
    `pkg-config --cflags --libs gtk+-3.0 sqlite3`
shopping_cart.exe
```

> Ensure image paths in the source code match your local directory.

---

## 🗄️ Database Schema

```sql
CREATE TABLE Users (
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
    Username TEXT UNIQUE,
    Password TEXT
);
```

---

## 📈 Possible Enhancements

* Password hashing (bcrypt / SHA-256)
* Per-user cart persistence
* Admin dashboard
* Order history tracking
* Real payment gateway integration
* Cross-platform packaging

---

## 👨‍💻 Author

**Prakhyat Mittal**
B.Tech Student | Software Developer
Interests: Systems Programming, GUI Applications, Databases

---
