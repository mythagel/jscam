
MACRO(JSCAM_TEST testname)
	ADD_TEST(${testname} "${CMAKE_BINARY_DIR}/bin/jscam" "${CMAKE_CURRENT_SOURCE_DIR}/${testname}.js")
	ADD_DEPENDENCIES(check ${testname})
ENDMACRO()

FOREACH(test basic exercise simple)
	JSCAM_TEST(${test})
ENDFOREACH()

