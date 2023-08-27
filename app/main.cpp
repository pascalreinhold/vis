#include "render_engine.hpp"
#include "error_logger.hpp"

#include <iostream>
#include <filesystem>

int main(int argc, char* argv[]) {
    std::filesystem::path log_file_path = (argc < 2) ? std::filesystem::current_path() / "lab_engine.log" : std::filesystem::path(argv[1]);
    lab::ErrorLogger::init(log_file_path);

    try {
        lab::RenderEngine engine{};
        engine.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
