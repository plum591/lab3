cmake_minimum_required(VERSION 3.10)

find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND git rev-list --count HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_COUNT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
else()
    set(GIT_COMMIT_COUNT "0")
endif()

set(PROJECT_VERSION 1.0.${GIT_COMMIT_COUNT})
project(lab3 VERSION ${PROJECT_VERSION})

if(WIN32)
    if(MSVC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    endif()
endif()

# Добавляем все исходные файлы
set(SOURCES
    main.cpp
    DLLContainer.cpp
    SeqContainer.cpp
    SLLContainer.cpp
)

set(HEADERS
    DLLContainer.h
    SeqContainer.h
    SLLContainer.h
)

add_executable(lab3 ${SOURCES} ${HEADERS})

set_target_properties(lab3 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(lab3 PRIVATE /W4 /WX)
else ()
    target_compile_options(lab3 PRIVATE -Wextra -pedantic -Werror -Wall)
endif()

# Указываем директории для поиска заголовочных файлов
target_include_directories(lab3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS lab3 RUNTIME DESTINATION .)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

if(WIN32)
    set(CPACK_GENERATOR ZIP)
    set(CPACK_PACKAGE_NAME "lab3")
else()
    set(CPACK_GENERATOR DEB)
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "lab3")
    set(CPACK_DEBIAN_PACKAGE_NAME "lab3")
endif()

include(CPack)