#include "Logger.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

#include "Utils.hpp"

void Logger::log(const std::string& message) {
    std::ofstream logFile("bank_audit.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTimestamp() << "] " << message << "\n";
        logFile.close();
    }
}
