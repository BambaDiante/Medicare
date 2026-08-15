cmake_minimum_required(VERSION 3.16)

project(Hospital_Management VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# =========================================
# Qt
# =========================================

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Sql
)


# =========================================
# Fichiers du projet
# =========================================

set(PROJECT_SOURCES
    main.cpp

    MainWindow.cpp
    MainWindow.h

    MedecinForm.cpp
    MedecinForm.h
)


# =========================================
# Création de l'exécutable
# =========================================

qt_add_executable(Hospital_Management
    ${PROJECT_SOURCES}
    MedicamentForm.h MedicamentForm.cpp
    PatientForm.h PatientForm.cpp
)


# =========================================
# Bibliothèques Qt
# =========================================

target_link_libraries(Hospital_Management
    PRIVATE
    Qt6::Widgets
    Qt6::Sql
)


# =========================================
# Finalisation Qt
# =========================================

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Hospital_Management)
endif()