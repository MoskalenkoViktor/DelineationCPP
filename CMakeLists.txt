cmake_minimum_required(VERSION 2.8)
project(DelineationCPP)
set(_src_root_path "${DelineationCPP_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE ALL_SOURCE LIST_DIRECTORIES false "*.h" "*.cpp")
add_executable(DelineationCPP ${ALL_SOURCE})

foreach(_source IN ITEMS ${ALL_SOURCE})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
