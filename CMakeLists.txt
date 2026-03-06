cmake_minimum_required(VERSION 4.0)
project(sqlite_ejemplo)



include(FetchContent)

message(STATUS "Downloading sqlite3...")
FetchContent_Declare(
  sqlite
  URL      https://sqlite.org/2026/sqlite-amalgamation-3510200.zip
  URL_HASH SHA3_256=9a9dd4eef7a97809bfacd84a7db5080a5c0eff7aaf1fc1aca20a6dc9a0c26f96
)
FetchContent_MakeAvailable(sqlite)
message(STATUS "Done downloading: " ${sqlite_SOURCE_DIR})

message(STATUS "Compiling sqlite3...")
add_library( sqlite "${sqlite_SOURCE_DIR}/sqlite3.c" "${sqlite_SOURCE_DIR}/sqlite3.h" )
set_target_properties(sqlite PROPERTIES LINKER_LANGUAGE C)
message(STATUS "Done compiling")


message(STATUS "Getting app sources")
file(GLOB APP_SOURCES src/*/*.cpp )
foreach(source ${APP_SOURCES})
    get_filename_component(nombre ${source} NAME_WE)
    message(STATUS "Source: " ${source} " Name: " ${nombre})
    add_executable(${nombre} ${source})
    target_link_libraries(${nombre} sqlite)
    target_include_directories(${nombre} PUBLIC "${sqlite_SOURCE_DIR}")
endforeach(source ${APP_SOURCES})
