# 🏦 Banking Management System (C++)

## 📖 Overview
This project is a **Banking Management System** built in C++ that simulates core banking operations such as managing clients, handling transactions, and controlling user access.

The system uses file-based storage to persist data and implements a permission-based user system for secure access control.

---

## 🚀 Features

### 👤 Client Management
- Add new clients
- Update client information
- Delete clients
- Search for clients
- Display all clients

### 💰 Transactions
- Deposit money
- Withdraw money with validation
- View total balances

### 🔐 User Management
- Login system with authentication
- Add, update, delete users
- Role-based access control using permissions
- Restrict access to system features

---

## 🧱 Technologies & Concepts
- **C++**
- **Structured Programming**
- **File Handling (fstream)**
- **Data Structures:**
  - Vector
- **Bitmasking (Permissions System)**

---

## 🏗️ System Design

### 🔹 Client Structure (stClient)
Represents a bank client:
- Account Number
- PIN Code
- Name
- Phone
- Account Balance

### 🔹 User Structure (stUser)
Represents system users:
- Username
- Password
- Permissions (bitmask)

### 🔹 Permissions System
Permissions are implemented using bitwise operations:

| Permission        | Value |
|------------------|------|
| List Clients     | 1    |
| Add Client       | 2    |
| Delete Client    | 4    |
| Update Client    | 8    |
| Find Client      | 16   |
| Transactions     | 32   |
| Manage Users     | 64   |
| Full Access      | -1   |

---

## ⚙️ How It Works

1. User logs into the system.
2. System validates credentials from file storage.
3. Based on permissions:
   - Access to specific features is granted or denied.
4. Client data is stored in text files using a custom delimiter.
5. All operations (add, update, delete) modify the file directly.

---

## 📂 Data Storage
- Clients stored in: `MyFile2.txt`
- Users stored in: `B.txt`
- Data is separated using delimiter: `#//#`

---

## ▶️ Example Use Cases
- Bank client management system
- Simple financial transaction simulator
- Role-based access control practice

---

## 📊 What I Learned
- File-based database simulation
- Implementing authentication systems
- Designing permission systems using bitmasking
- Managing structured data in C++
- Building multi-feature console applications

---

## 📌 Future Improvements
- Replace file system with real database (SQL)
- Encrypt passwords instead of plain text
- Improve input validation and error handling
- Apply Object-Oriented Design instead of procedural style
- Add GUI interface

---
