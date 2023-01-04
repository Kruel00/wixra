##
## Created By:  Javier Castorena
## Year:        2023
##

CMAKE_MINIMUM_REQUIRED(VERSION 3.20.0)

project(wixra VERSION 0.0.2)

set(CMAKE_EXE_LINKER_FLAGS "-rdynamic")

find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GLIB REQUIRED glib-2.0)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

add_executable(${PROJECT_NAME} 
     main.c

)

include_directories(
    include
    ${GLIB_INCLUDE_DIRS}
    ${GTK3_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${GLIB_LIBRARIES}
    ${GTK3_LIBRARIES}
)