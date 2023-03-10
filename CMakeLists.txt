##
## Created By:  Javier Castorena
## Year:        2023
##

CMAKE_MINIMUM_REQUIRED(VERSION 3.20.0)

project(wixra VERSION 0.0.2)

set(CMAKE_EXE_LINKER_FLAGS "-rdynamic -Wall -ludev")

find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GLIB REQUIRED glib-2.0)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(UDEV REQUIRED libudev)
pkg_check_modules(BLKID REQUIRED blkid)

add_executable(${PROJECT_NAME} 
     main.c
     gui.c
     storage_device.c
)

include_directories(
    include
    ${GLIB_INCLUDE_DIRS}
    ${GTK3_INCLUDE_DIRS}
    ${UDEV_INCLUDE_DIRS}
    ${BLKID_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${GLIB_LIBRARIES}
    ${GTK3_LIBRARIES}
    ${UDEV_LIBRARIES}
    ${BLKID_LIBRARIES}
)