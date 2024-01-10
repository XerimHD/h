cmake_minimum_required(VERSION 3.28)  # Минимальная версия CMake

project(ctest)  # Название вашего проекта


# Указание исходных файлов
set(SOURCES
    src/main.cpp
    
)

find_package(Curses REQUIRED)
message(NOTICE "CURSES_INCLUDE_DIR: ${CURSES_INCLUDE_DIR}")

# Если библиотека Curses найдена, добавляем ее включение и линковку
if(CURSES_FOUND)
    include_directories(${CURSES_INCLUDE_DIR})
    add_executable(${PROJECT_NAME} ${SOURCES})
    target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
endif()


