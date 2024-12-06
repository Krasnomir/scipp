cmake_minimum_required(VERSION 3.22.2)

project(scipp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS off)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)

file(GLOB_RECURSE COMPILE_LIST "src/*.cpp")

# Old
# set(COMPILE_LIST src/Camera.cpp src/Scriptable/Entity.cpp src/Scriptable/Component.cpp src/Util.cpp src/Scriptable/EventObject.cpp src/Game.cpp src/GameState.cpp src/InitState.cpp src/Scriptable/State.cpp src/StateManager.cpp src/Scriptable/Components/RenderComponent.cpp)

add_compile_options(-fconcepts-diagnostics-depth=2)
add_executable(${PROJECT_NAME} src/main.cpp ${COMPILE_LIST})
add_compile_definitions(DEBUG=1)


include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1)
FetchContent_MakeAvailable(SFML)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-window)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-audio)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system)



install(TARGETS ${PROJECT_NAME})

add_compile_options(-fPIC)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${CMAKE_SOURCE_DIR}/out/scipp")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

