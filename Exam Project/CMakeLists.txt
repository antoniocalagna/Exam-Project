cmake_minimum_required(VERSION 3.10)
project(Social_Network)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#QUI le cartelle
include_directories(Libraries/Headers               #Qui vengono aggiunti i percorsi di ricerca per la #include
        Libraries/Headers/Account\ Hierarchy        #Nota come si fa con lo spazio
        )

set(SRC Libraries/Sources)                          #Aliases (SRC -> Source, HDR -> Header)
set(HDR Libraries/Headers)

#QUI i .cpp
set(                                                #Qui si fa una sorta di assegnazione: chiamando "${SOURCE_FILES}"
        SOURCE_FILES                                #è come se scrivessi tutta questa roba
        ${HDR}/Graph.hpp
        ${HDR}/TemplateFunctions.hpp

        ${HDR}/Clock.h          ${SRC}/Clock.cpp
        ${HDR}/Date.h           ${SRC}/Date.cpp
        ${HDR}/FileHandler.h    ${SRC}/FileHandler.cpp
        ${HDR}/Manager.h        ${SRC}/Manager.cpp
        ${HDR}/Post.h           ${SRC}/Post.cpp
        ${HDR}/IOBuffer.h       ${SRC}/IOBuffer.cpp
        ${HDR}/Shell.h          ${SRC}/Shell.cpp
        ${HDR}/Account\ Hierarchy/Account.h     ${SRC}/Account\ Hierarchy/Account.cpp
        ${HDR}/Account\ Hierarchy/Company.h     ${SRC}/Account\ Hierarchy/Company.cpp
        ${HDR}/Account\ Hierarchy/Group.h       ${SRC}/Account\ Hierarchy/Group.cpp
        ${HDR}/Account\ Hierarchy/User.h        ${SRC}/Account\ Hierarchy/User.cpp
        main_di_clara.hpp
        main_2.hpp
        )


add_executable(Social_Network main.cpp ${SOURCE_FILES})     #I .cpp vengono linkati "in automatico" qui