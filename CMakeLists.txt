cmake_minimum_required(VERSION 3.19)

project(aus1
    VERSION     0.0.1
    DESCRIPTION "Cvičenia z predmetu Algoritmy a údajové štruktúry 1"
    LANGUAGES   CXX
)

add_subdirectory(complexities)

add_subdirectory(dependencies)

add_subdirectory(libds)

add_subdirectory(libds.tests)

add_subdirectory(semestral_project)
