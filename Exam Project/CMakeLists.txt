cmake_minimum_required(VERSION 3.10)
project(Social_Network)

set(CMAKE_CXX_STANDARD 11)

include_directories(Libraries/Headers               #Qui vengono aggiunti i percorsi di ricerca per la #include
        Libraries/Headers/Account\ Hierarchy        #Nota come si fa con lo spazio
        )

set(                                                #Qui si fa una sorta di assegnazione: chiamando "${SOURCE_FILES}"
        SOURCE_FILES                                #è come se scrivessi tutta questa roba
        Libraries/Sources/Clock.cpp
        Libraries/Sources/Date.cpp
        Libraries/Sources/MainFunctions.cpp
        Libraries/Sources/Manager.cpp
        Libraries/Sources/Post.cpp
        Libraries/Sources/Account\ Hierarchy/Account.cpp
        Libraries/Sources/Account\ Hierarchy/Company.cpp
        Libraries/Sources/Account\ Hierarchy/Group.cpp
        Libraries/Sources/Account\ Hierarchy/User.cpp
        )

add_executable(Social_Network main.cpp ${SOURCE_FILES})     #I .cpp vengono linkati "in automatico" qui