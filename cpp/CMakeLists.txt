cmake_minimum_required(VERSION 3.22)
project(LoggerLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

find_package(Threads REQUIRED)
add_subdirectory(src)
add_subdirectory(tests)

# Installation logic
install(EXPORT LoggerTargets
    FILE LoggerTargets.cmake
    NAMESPACE Logger::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Logger
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/LoggerConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/LoggerConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Logger
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/LoggerConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/LoggerConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/LoggerConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Logger
)

