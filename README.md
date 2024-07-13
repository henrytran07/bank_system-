
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

    cd US_citizen_database

    ```
    
    - Now you can access debugging.cpp. 

    **for compiling the program (US_citizen folder)**

    - debugging.cpp file is for visualizing name, SSN, DOB. 

    ```
    g++ -std=c++11 -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -lssl -lcrypto debugging.cpp -o debugging

    ./debugging  

    ```

    - debugging2.cpp file is for visualizing how unordered_map inserts names, SSN, SOB into the index, and then we search for the name. 

    ```
    g++ -std=c++11 -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -lssl -lcrypto debugging2.cpp -o debugging2

    ./debugging2
    ```

    **for compiling the program(customer_information folder)**

    ```
    g++ -std=c++11 -o customer_information.o customer_information/debugging.cpp US_citizen_database/us_citizen.cpp -lssl -lcrypto

    ./customer_information.o
    ```

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
