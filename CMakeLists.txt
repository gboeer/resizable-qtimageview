cmake_minimum_required(VERSION 3.1)

project(ResizableImageView LANGUAGES CXX)

# helper to copy DLL to target output dir
MACRO (COPY_DLL trgt libpath)
        ADD_CUSTOM_COMMAND (
        TARGET ${trgt}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy_if_different "${libpath}" $<TARGET_FILE_DIR:${trgt}>
    )
ENDMACRO (COPY_DLL)


#define build options
option(BUILD_TEST "Build test project for the widget" ON)

if (BUILD_TEST)

	set(CMAKE_AUTOMOC ON)
	
	add_executable(widget_test include/resizableimageview.h src/resizableimageview test/test.cpp test/mainwindow.h test/mainwindow.cpp)
	
	# Find the QtWidgets library
	# Instruct CMake to run moc automatically when needed.
	find_package(Qt5Widgets REQUIRED)
	# Use the Widgets module from Qt 5.
	target_link_libraries(widget_test Qt5::Widgets)
	
	# Qt bins
	COPY_DLL(widget_test $<TARGET_FILE:Qt5::Widgets>)
	COPY_DLL(widget_test $<TARGET_FILE:Qt5::Core>)
	COPY_DLL(widget_test $<TARGET_FILE:Qt5::Gui>)
	# We need the plugin platforms folder for QT 
	get_target_property(_loc Qt5::QOffscreenIntegrationPlugin LOCATION)
	get_filename_component(_folder ${_loc} DIRECTORY)
	add_custom_command(TARGET widget_test POST_BUILD
					   COMMAND ${CMAKE_COMMAND} -E copy_directory
						   ${_folder} $<TARGET_FILE_DIR:widget_test>/platforms)   
	
endif()

