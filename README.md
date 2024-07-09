
# Bank System

This repository contains a C++ program for bank system 

# Installation 

1. **Install C++ Compiler:** 
- Ensure you have a C++ compiler on your machine. For Windows, you can use MinGW or Visual Studio with C++ support. For macOS, you can use Xcode Command Line Tools. for Linux, you can use GCC or Clang. 

2. **Clone the Repository**
    - Open you temerinal or command prompt [cmd + space]
    - Clone the repository using the following command: 
```
    git clone https://github.com/henrytran07/bank_system-.git
```
    - this will download the repository to your local machine

3. **Download OpenSSL**
    - Install OpenSSL 
    ```
        brew install openssl
    ```
    - Verify Installation
    ```
        openssl version
    ```
4. **Compile the Program:**
    - Example: 
    ```
        g++ -std=c++11 -o customer_information customer_information.cpp -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -lssl -lcrypto
    ```

5. **Run the Program:**
    - Execute the compiled program:
    ```
        ./customer_information
    ```
