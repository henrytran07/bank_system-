
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
     
- This will download the repository to your local machine

3. **Download OpenSSL**
    - Install OpenSSL 
    ```
        brew install openssl
    ```
    - Verify Installation
    ```
        openssl version
    ```

4. **Debugging the Program (for Mario):**

    ```
    cd bank_system-

    cd customer-information

    ```
    
    - Now you can access debugging.cpp. 
5. **Compile the Program:**
    - Navigate to the directory of the specific component (e.g.,    customer_information) using cd.
    - Example: 
    ```
    cd bank_system-

    cd customer-information

    g++ -std=c++11 -o customer_information customer_information.cpp -I$(brew --prefix openssl)/include -L$(brew --prefix openssl)/lib -lssl -lcrypto

    ```

6. **Run the Program:**
    - Execute the compiled program:
    ```
        ./customer_information
    ```
