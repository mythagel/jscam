
MACRO(JSCAM_TEST testname)
	ADD_TEST(${testname} "${CMAKE_BINARY_DIR}/bin/jscam" "${CMAKE_CURRENT_SOURCE_DIR}/${testname}.js")
	ADD_DEPENDENCIES(check jscam)
ENDMACRO()

FOREACH(test basic simple model interface push_state simple_stock bbox args)
	JSCAM_TEST(${test})
ENDFOREACH()

