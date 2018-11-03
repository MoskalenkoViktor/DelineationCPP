cmake_minimum_required(VERSION 2.8)
project(DelineationCPP)
set(_src_root_path "${DelineationCPP_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE ALL_SOURCE LIST_DIRECTORIES false "*.h" "*.cpp")
add_executable(DelineationCPP ${ALL_SOURCE} delineation/p/delineation.cpp delineation/p/delineation.h delineation/p/beta/beta.h delineation/p/gamma/gamma.h delineation/p/beta/beta.cpp delineation/p/beta/biphasic.cpp delineation/p/beta/flexure.cpp delineation/p/beta/legacy.cpp delineation/p/beta/offset.cpp delineation/p/beta/onset.cpp delineation/p/beta/peak.cpp delineation/p/delta/addition.cpp delineation/p/delta/data.cpp delineation/p/delta/delta.cpp delineation/p/delta/delta.h delineation/p/delta/matrix.cpp delineation/p/delta/removal.cpp delineation/p/fibrillation/fibrillation.cpp delineation/p/fibrillation/fibrillation.h delineation/p/gamma/gamma.cpp delineation/p/gamma/data.cpp delineation/p/gamma/offset.cpp delineation/p/gamma/onset.cpp delineation/p/gamma/peak.cpp delineation/p/gamma/points.cpp delineation/p/gamma/xtd.cpp delineation/p/onset.cpp delineation/p/peak.cpp delineation/p/routines.cpp delineation/p/zcs.cpp delineation/p/beta/peak.h delineation/p/beta/flexure.h delineation/p/beta/biphasic.h delineation/p/beta/onset.h delineation/p/beta/offset.h delineation/p/beta/legacy.h delineation/p/routines.h delineation/p/zcs.h delineation/peaks_zcs_ids.h modulus_maxima/routines.h delineation/p/delta/addition.h zero_crossing/routines.h delineation/p/delta/removal.h delineation/p/delta/data.h delineation/p/gamma/data.h delineation/p/gamma/points.h delineation/p/gamma/offset.h delineation/p/gamma/onset.h delineation/p/gamma/peak.h delineation/p/gamma/xtd.h delineation/p/offset.h delineation/p/onset.h delineation/p/peak.h)

foreach(_source IN ITEMS ${ALL_SOURCE})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
