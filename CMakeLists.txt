cmake_minimum_required(VERSION 3.16)
project(CampusEsportsArena LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(CampusEsportsArena
    main.cpp
    MainWindow.cpp
    MainWindow.h
    TournamentManager.cpp
    TournamentManager.h
    Models.h
)

target_link_libraries(CampusEsportsArena PRIVATE Qt6::Widgets)
