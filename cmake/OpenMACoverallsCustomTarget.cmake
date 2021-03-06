IF(WITH_COVERALLS_ENABLED)
  IF (NOT BUILD_UNIT_TESTS OR NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE(FATAL_ERROR "Coveralls can be only be activated if the code is compiled in debug mode with unit tests enabled.")
  ENDIF()
  # Adapt the CXX flags
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  # Create a custom target which generate the files required by Coveralls
  SET(COVERALLS_FILE "${PROJECT_BINARY_DIR}/coveralls.json")
  ADD_CUSTOM_TARGET(coveralls
    # Generate the output
    COMMAND ${CMAKE_COMMAND}
      -DCOVERAGE_SRC_PATH="${PROJECT_SOURCE_DIR}/modules"
      -DCOVERAGE_SRC_EXCLUDED=".*_p.h::.*test.*::.*bindings.*::.*plugins.*"
      -DCOVERAGE_BIN_EXCLUDED=".*3rdparty.*"
      -DCOVERALLS_OUTPUT_FILE="${COVERALLS_FILE}"
      -DCOV_PATH="${PROJECT_BINARY_DIR}"
      -DPROJECT_ROOT="${PROJECT_SOURCE_DIR}"
      -P "${CMAKE_MODULE_PATH}/CoverallsGenerateGcov.cmake"
    # Upload the result
    COMMAND curl -S -F json_file=@${COVERALLS_FILE} https://coveralls.io/api/v1/jobs && echo
    COMMENT "Generating and uploading coveralls output...")
ELSE()
  ADD_CUSTOM_TARGET(coveralls COMMAND echo "Nothing to do.")
ENDIF()