cmake_minimum_required(VERSION 3.16)
project(mini_bank_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

include_directories(gui include)

set(CORE_SOURCES
    include/Account.hpp
    include/BankSystem.hpp
    include/CheckingAccount.hpp
    include/Customer.hpp
    include/Logger.hpp
    include/SavingsAccount.hpp
    include/Transaction.hpp
    include/Utils.hpp
    src/Account.cpp
    src/BankSystem.cpp
    src/CheckingAccount.cpp
    src/Customer.cpp
    src/Logger.cpp
    src/SavingsAccount.cpp
    src/Transaction.cpp
)

# Główna aplikacja
add_executable(mini_bank_system
    ${CORE_SOURCES}
    gui/MainWindow.cpp
    gui/MainWindow.hpp
    src/main.cpp
)
target_link_libraries(mini_bank_system Qt6::Widgets)

# Testy
add_executable(mini_bank_system_tests
    ${CORE_SOURCES}
    tests/AccountTests.hpp
    tests/BankSystemTests.hpp
    tests/SavingsAccountTests.hpp
    tests/CheckingAccountTests.hpp
    tests/RunTests.cpp
)
