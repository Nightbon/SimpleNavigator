cmake_minimum_required(VERSION 3.16)
project(s21_graph_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ───────── Путь к заголовочным файлам ─────────
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# ───────── Статическая библиотека: s21_stack ─────────
add_library(s21_stack STATIC ${SRC_DIR}/model/Stack.cpp)
target_include_directories(s21_stack PUBLIC ${SRC_DIR}/model/include)

# ───────── Статическая библиотека: s21_queue ─────────
add_library(s21_queue STATIC ${SRC_DIR}/model/Queue.cpp)
target_include_directories(s21_queue PUBLIC ${SRC_DIR}/model/include)

# ───────── Статическая библиотека: s21_graph ─────────
add_library(s21_graph STATIC ${SRC_DIR}/model/graph.cpp)
target_include_directories(s21_graph PUBLIC ${SRC_DIR}/model)

# ───────── Статическая библиотека: s21_graph_algorithms ─────────
add_library(s21_graph_algorithms STATIC ${SRC_DIR}/model/graph_algorithms.cpp)
target_include_directories(s21_graph_algorithms PUBLIC ${SRC_DIR}/model/include)
target_link_libraries(s21_graph_algorithms PUBLIC s21_graph s21_stack s21_queue)

# ───────── Статическая библиотека: controller ─────────
add_library(controller STATIC ${SRC_DIR}/controller/controller.cpp)
target_include_directories(controller PUBLIC ${SRC_DIR}/controller)
target_include_directories(controller PUBLIC ${SRC_DIR}/view)
target_link_libraries(controller PRIVATE s21_graph_algorithms)

# ───────── Статическая библиотека: view ─────────
add_library(view STATIC ${SRC_DIR}/view/view.cpp)
target_include_directories(view PUBLIC ${SRC_DIR}/view)
target_link_libraries(view PRIVATE controller s21_graph_algorithms s21_graph s21_queue s21_stack)

add_executable(simpleNavigator main.cpp)
target_link_libraries(simpleNavigator PRIVATE s21_graph_algorithms controller view)

find_package(GTest REQUIRED)
add_executable(test_model
    ${SRC_DIR}/model/tests/test_model.cpp
    ${SRC_DIR}/model/include/tests/test_list.cpp
    ${SRC_DIR}/model/include/tests/test_vector.cpp 
    ${SRC_DIR}/model/include/tests/test_stack.cpp 
    ${SRC_DIR}/model/include/tests/test_queue.cpp
)
target_include_directories(test_model PUBLIC ${SRC_DIR}/model)
target_link_libraries(test_model PRIVATE s21_graph_algorithms GTest::gtest GTest::gtest_main)