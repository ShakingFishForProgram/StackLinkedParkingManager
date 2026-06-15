cmake_minimum_required(VERSION 3.19)
project(StackLinkedParkingManager LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(StackLinkedParkingManager
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Car.h
    testCar.cpp
    DualStack.h
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
    testCar.cpp
    Car.h
)
