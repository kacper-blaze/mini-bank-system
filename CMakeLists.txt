cmake_minimum_required(VERSION 4.1)
project(mini_bank_system)

set(CMAKE_CXX_STANDARD 14)

include_directories(gui)
include_directories(include)

# Main application executable
add_executable(mini_bank_system
        gui/MainWindow.cpp
        gui/MainWindow.hpp
        include/Account.hpp
        include/BankSystem.hpp
        include/Transaction.hpp
        include/Customer.hpp
        include/Logger.hpp
        include/Utils.hpp
        src/Account.cpp
        src/BankSystem.cpp
        src/main.cpp
        src/Customer.cpp
        src/Transaction.cpp
        src/Logger.cpp)

# Test executable
add_executable(mini_bank_system_tests
        include/Account.hpp
        include/BankSystem.hpp
        include/Transaction.hpp
        include/Customer.hpp
        include/Logger.hpp
        include/Utils.hpp
        src/Account.cpp
        src/BankSystem.cpp
        src/Customer.cpp
        src/Transaction.cpp
        src/Logger.cpp
        tests/BankSystemTests.hpp
        tests/AccountTests.hpp
        tests/RunTests.cpp)
