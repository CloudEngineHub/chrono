SET(LIBRARIES Chrono_core)

SET(TESTS
    utest_CH_slider_pend
    utest_CH_double_pend
    utest_CH_shafts
    utest_CH_compute_contact
    utest_CH_assembly
    utest_CH_composite_inertia
)

MESSAGE(STATUS "Add unit test programs for PHYSICS module")

FOREACH(PROGRAM ${TESTS})
    # MESSAGE(STATUS "...add ${PROGRAM}")

    ADD_EXECUTABLE(${PROGRAM}  "${PROGRAM}.cpp")
    SOURCE_GROUP(""  FILES "${PROGRAM}.cpp")

    SET_TARGET_PROPERTIES(${PROGRAM} PROPERTIES FOLDER tests)
    SET_PROPERTY(TARGET ${PROGRAM} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROGRAM}>")
    TARGET_LINK_LIBRARIES(${PROGRAM} ${LIBRARIES} gtest_main)

    if(MSVC)
      set_target_properties(${PROGRAM} PROPERTIES MSVC_RUNTIME_LIBRARY ${CH_MSVC_RUNTIME_LIBRARY})
    endif()

    INSTALL(TARGETS ${PROGRAM} DESTINATION ${CH_INSTALL_DEMO})
    ADD_TEST(${PROGRAM} ${PROJECT_BINARY_DIR}/bin/${PROGRAM})
ENDFOREACH(PROGRAM)
