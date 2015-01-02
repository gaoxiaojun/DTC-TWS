project(qtws)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-DIB_USE_STD_STRING -DTWS_LIBRARY)

#include_directories(
#    ${CMAKE_CURRENT_SOURCE_DIR}/IBJts/source/PosixClient/Shared
#)

set(LIB_SOURCES
    qtwsclientsocket.h
    qtwsclientsocket.cpp
    qtwsapi.h
    qtwsapi.cpp
    qtwsapilogging.h
    qtwsapilogging.cpp
    qbarfactory.h
    qbarfactory.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/IBJts/source/PosixClient/src/EClientSocketBase.cpp
)

set(TARGET_LIBRARIES
    Qt5::Core
    Qt5::Network
)


add_library(${PROJECT_NAME} SHARED ${LIB_SOURCES})
target_link_libraries(${PROJECT_NAME} ${TARGET_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${APPLICATION_PLUGINS_DIR})

set(${PROJECT_NAME}_INCLUDE_DIRS ${PARENT_DIR} CACHE INTERNAL "${PROJECT_NAME}_include" FORCE)
set(${PROJECT_NAME}_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "${PROJECT_NAME}_library" FORCE)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

set(EXEC_SOURCES
    testqtwsapi.cpp
)
add_executable(${PROJECT_NAME}_test ${EXEC_SOURCES})
target_link_libraries(${PROJECT_NAME}_test ${TARGET_LIBRARIES} ${${PROJECT_NAME}_LIBRARIES})
