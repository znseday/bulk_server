cmake_minimum_required(VERSION 3.2)

if(WIN32 OR WIN64)
	message(STATUS "My Windows detected")
	project(bulk_server VERSION 0.0.1000)
	set(BOOST_ROOT "C:/Boost")
else()
	project(bulk_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
endif()

#find_package(Threads REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

if (WIN32 OR WIN64)
    # disable autolinking in boost
    add_definitions( -DBOOST_ALL_NO_LIB )

	set (Boost_USE_STATIC_LIBS OFF CACHE BOOL "use static libraries from Boost")

    add_definitions( -DBOOST_ALL_DYN_LINK )
endif()

add_executable(bulk_server main.cpp common.h)

add_executable(my_tests my_test.cpp common.h)

if(WIN32 OR WIN64)
	set_target_properties(bulk_server my_tests PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON)
else()
	set_target_properties(bulk_server my_tests PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
		COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")

		set(CMAKE_BUILD_TYPE Release)
endif()

set_target_properties(my_tests PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)


target_link_libraries(my_tests
    ${Boost_LIBRARIES}
#    my_aux
)

install(TARGETS bulk_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT znseday)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()

add_test(my_tests_ my_tests)
