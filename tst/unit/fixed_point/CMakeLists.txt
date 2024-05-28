add_library( fixed_point_unit_tests OBJECT
  test_fp.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/solve.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/solver_opts.hpp
  )

target_link_libraries( fixed_point_unit_tests
  PRIVATE
    Catch2::Catch2
  )

target_include_directories( fixed_point_unit_tests PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../src"
  "${CMAKE_CURRENT_SOURCE_DIR}/../"
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
  )

target_link_libraries( fixed_point_unit_tests PRIVATE Catch2::Catch2 )
target_sources( unit_tests PRIVATE $<TARGET_OBJECTS:fixed_point_unit_tests> )
