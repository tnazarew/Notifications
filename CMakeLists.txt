cmake_minimum_required(VERSION 3.3)
project(Notificator)

#find_package(Boost REQUIRED COMPONENTS system thread)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive -pthread")
include_directories("/usr/local/include/pqxx /usr/local/pgsql/include /usr/local/pgsql/lib")
include_directories(
        utilities
        layers
        database
        #        ${Boost_INCLUDE_DIRS}
)


set(SOURCE_FILES
        layers/Layer.cpp
        layers/Layer.h
        Server.cpp
        Server.h
        layers/TransportLayer.cpp
        layers/TransportLayer.h
        layers/SecurityLayer.cpp
        layers/SecurityLayer.h
        layers/ApplicationLayer.cpp
        layers/ApplicationLayer.h
        utilities/TCPHeader.cpp
        utilities/TCPHeader.h
        utilities/Configuration.cpp
        utilities/Configuration.h
        utilities/RSA.cpp
        utilities/RSA.h
        utilities/RC4.cpp
        utilities/RC4.h
        utilities/MD5.cpp
        utilities/MD5.h
        utilities/Util.cpp
        utilities/Util.h
        database/jsoncpp.cpp
        database/json/json.h
        database/json/json-forwards.h
        database/DatabaseService.cpp
        database/DatabaseService.h
        database/DatabaseObjects.h
        database/JsonResolver.cpp
        database/JsonResolver.h
        database/JsonBuilder.cpp
        database/JsonBuilder.h
        database/TableSQLs.h)

add_executable(Notificator main.cpp ${SOURCE_FILES})
target_link_libraries(Notificator pqxx pq)

add_executable(db_test test/db_test.cpp ${SOURCE_FILES})
target_link_libraries(db_test pqxx pq)