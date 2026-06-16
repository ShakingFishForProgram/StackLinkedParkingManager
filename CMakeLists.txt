cmake_minimum_required(VERSION 3.19)
project(StackLinkedParkingManager LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(StackLinkedParkingManager
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
)

qt_add_translations(
    TARGETS StackLinkedParkingManager
    TS_FILES StackLinkedParkingManager_zh_CN.ts
)

target_link_libraries(StackLinkedParkingManager
    PRIVATE
        Qt::Core
        Qt::Widgets
)

target_include_directories(StackLinkedParkingManager PRIVATE include)

install(TARGETS StackLinkedParkingManager
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET StackLinkedParkingManager
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

add_executable(testCar
    test/testCar.cpp
    include/Car.h
)

target_include_directories(testCar PRIVATE include)

add_executable(testDualStack
    test/testDualStack.cpp
    include/DualStack.h
    include/ParkingDualStack.h
)

target_include_directories(testDualStack PRIVATE include)

add_executable(testParkingDualStack
    test/testParkingDualStack.cpp
    include/ParkingDualStack.h
    include/DualStack.h
    include/Car.h
)

target_include_directories(testParkingDualStack PRIVATE include)

add_executable(testDoublyLinkedList
    test/testDoublyLinkedList.cpp
    include/DoublyLinkedList.h
)

target_include_directories(testDoublyLinkedList PRIVATE include)

add_executable(testDoublyLinkedListCar
    test/testDoublyLinkedListCar.cpp
    include/DoublyLinkedList.h
    include/Car.h
)

target_include_directories(testDoublyLinkedListCar PRIVATE include)

add_executable(testParkingStructure
    test/testParkingStructure.cpp
    include/ParkingStructure.h
    include/ParkingDualStack.h
    include/ParkingDoublyLinkedList.h
    include/DualStack.h
    include/DoublyLinkedList.h
    include/Car.h
    include/ParkingInterfaces.h
)

target_include_directories(testParkingStructure PRIVATE include)

add_executable(testParkingMessage
    test/testParkingMessage.cpp
    include/ParkingStructure.h
    include/ParkingDualStack.h
    include/ParkingDoublyLinkedList.h
    include/DualStack.h
    include/DoublyLinkedList.h
    include/Car.h
    include/ParkingInterfaces.h
)

target_include_directories(testParkingMessage PRIVATE include)
