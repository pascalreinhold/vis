#include "error_logger.hpp"

#include <iostream>
#include <sstream>

namespace lab {

std::unique_ptr<ErrorLogger> ErrorLogger::m_error_logger = nullptr;

void ErrorLogger::init(std::filesystem::path log_file_path, bool print_to_console) {
    m_error_logger = std::make_unique<ErrorLogger>(std::move(log_file_path), print_to_console);
    ErrorLogger::log("ErrorLogger", "Initialized");
}

ErrorLogger::ErrorLogger(std::filesystem::path log_file_path, bool print_to_console)
: m_log_file_path(std::move(log_file_path)), m_print_to_console(print_to_console) {
    m_log_file.open(m_log_file_path);
    if (!m_log_file.is_open()) {
        throw std::runtime_error("Failed to open log file");
    }
}

ErrorLogger::~ErrorLogger() {
    ErrorLogger::log("ErrorLogger", "Destroyed");
    m_log_file.close();
}

void ErrorLogger::log(std::string_view caller, std::string_view message) {
    // assure that only one thread can write to the log file at a time
    auto& logger = ErrorLogger::get();
    std::lock_guard<std::mutex> lock(logger.m_output_mutex);

    logger.m_log_file << caller << ": " << message << std::endl;
    if (logger.m_print_to_console) {
        std::cout << caller << ": " << message << std::endl;
    }
}

void ErrorLogger::logFatalError(std::string_view caller, std::string_view message, std::source_location location) {
    auto& logger = ErrorLogger::get();
    {
        std::lock_guard<std::mutex> lock(logger.m_output_mutex);

        std::stringstream error_message;
        error_message << caller << ": " << "Fatal Error: " << message << "\n";
        error_message << "\tFile: " << location.file_name() << "\n";
        error_message << "\tLine: " << location.line() << "\n";
        error_message << "\tFunction: " << location.function_name() << "\n";

        logger.m_log_file << error_message.str();
        if (logger.m_print_to_console) {
            std::cout << error_message.str();
        }
    }

    exit(EXIT_FAILURE);
}


} // namespace lab