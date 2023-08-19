#pragma once
#include <filesystem>
#include <fstream>
#include <source_location>
#include <memory>
#include <mutex>
#include <iostream>

namespace lab {

// thread safe + singleton
class ErrorLogger {
    public:
        explicit ErrorLogger(std::filesystem::path log_file_path, bool print_to_console);
        ~ErrorLogger();

        // if throw_on_error is true throw a runtime_error, otherwise just log the error and call assert
        static void init(std::filesystem::path log_file_path, bool print_to_console = true);
        static ErrorLogger& get() { return *m_error_logger; }

        static void log(std::string_view caller, std::string_view message);
        static void logFatalError(std::string_view caller, std::string_view message, std::source_location location = std::source_location::current());

        // don't copy or move error loggers
        ErrorLogger(const ErrorLogger&) = delete;
        ErrorLogger& operator=(const ErrorLogger&) = delete;
        ErrorLogger(ErrorLogger&&) = delete;
        ErrorLogger& operator=(ErrorLogger&&) = delete;

    private:
        static std::unique_ptr<ErrorLogger> m_error_logger;
        std::filesystem::path m_log_file_path;
        std::ofstream m_log_file;
        bool m_print_to_console;
        std::mutex m_output_mutex{};
};

} // namespace lab
