#pragma once
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

/**
 * @brief Utility function to get the current system date and time.
 * @return A string formatted as "YYYY-MM-DD HH:MM:SS" (e.g., "2026-05-29 20:15:30").
 */
inline std::string getCurrentTimestamp() {
    // Capture the exact current point in time from the system clock
    auto now = std::chrono::system_clock::now();
    // Convert the time point to a standard time_t structure
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
    // Thread-safe and platform-specific conversion to local calendar time
#if defined(_MSC_VER)
    // Windows
    localtime_s(&localTime, &currentTime);
#else
    // POSIX (Linux, macOS, etc.)
    localtime_r(&currentTime, &localTime);
#endif
    // Format the time as a string
    std::stringstream ss;
    ss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}