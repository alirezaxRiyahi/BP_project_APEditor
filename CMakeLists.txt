cmake_minimum_required(VERSION 3.20)
project(AP_Editor)

# استاندارد C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- مسیر نصب SDL2 و کتابخانه‌ها ---
set(SDL2_PATH "C:/MinGW/MinGW")   # این مسیر رو متناسب با محل نصب خودت اصلاح کن

# include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${SDL2_PATH}/include
)

# library directories
link_directories(
        ${SDL2_PATH}/lib
)

# --- فایل‌های پروژه ---
add_executable(AP_Editor
        ${CMAKE_SOURCE_DIR}/src/main.cpp
        ${CMAKE_SOURCE_DIR}/src/resource.rc
)

# --- لینک کردن کتابخانه‌ها ---
target_link_libraries(AP_Editor
        mingw32
        SDL2main
        SDL2
        SDL2_ttf
        SDL2_image
        SDL2_mixer
        ws2_32
)

# --- Resource ها (کپی assets و data کنار exe) ---
set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)
set(DATA_DIR   ${CMAKE_SOURCE_DIR}/data)

add_custom_command(TARGET AP_Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_DIR}
        $<TARGET_FILE_DIR:AP_Editor>/assets
)

add_custom_command(TARGET AP_Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${DATA_DIR}
        $<TARGET_FILE_DIR:AP_Editor>/data
)
