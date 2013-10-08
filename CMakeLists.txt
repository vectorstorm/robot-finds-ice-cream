cmake_minimum_required( VERSION 2.6 )

if (DEFINED CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel")
else()
	message(STATUS "No build type selected, default to Debug")
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel")
endif()

project( RobotFindsIceCream )

add_subdirectory( vectorstorm )
find_package(OpenGL)
set(VS_LIBRARIES
	${OPENGL_LIBRARIES}
	${SDL_LIBRARY}
	${SDL_IMAGE_LIBRARY}
	${SDL_MIXER_LIBRARY}
	${SDL_TTF_LIBRARY}
	${PNG_LIBRARY}
	${GLEW_LIBRARY}
	${BOX2D_LIBRARY}
	)

# find . -type d
include_directories(
	Games
	Games/Preferences
	Games/RobotPal
	Games/RobotPal/Modes
	)

set(ROBOT_SOURCES
	Games/Preferences/PREF_Game.cpp
	Games/RobotPal/Modes/RP_ModeCredits.cpp
	Games/RobotPal/Modes/RP_ModeInGame.cpp
	Games/RobotPal/RP_Fader.cpp
	Games/RobotPal/RP_Game.cpp
	Games/RobotPal/RP_Map.cpp
	Games/RobotPal/RP_Player.cpp
	Games/RobotPal/RP_Robot.cpp
	Games/RobotPal/RP_VendingMachine.cpp
	)

set(PLATFORM_SOURCES)
SET(PLATFORM_LIBRARIES)

if ( APPLE )
	SET( PLATFORM_SOURCES osx/SDLMain.m )
	find_library(OPENGL_LIBRARY OpenGL)
	find_library(COCOA_LIBRARY Cocoa)
	set(PLATFORM_LIBRARIES ${COCOA_LIBRARY} ${IOKIT_LIBRARY})
endif ( APPLE )

include_directories( SYSTEM
	${OPENGL_INCLUDE_DIR}
	${SDL_INCLUDE_DIR}
	${SDL_MIXER_INCLUDE_DIR}
	${SDL_IMAGE_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${BOX2D_INCLUDE_DIR}
	)
include_directories( .
	vectorstorm
	vectorstorm/VS
	)

add_executable(
	RobotFindsIceCream
	MACOSX_BUNDLE
	main.cpp
	${ROBOT_SOURCES}
	${PLATFORM_SOURCES}
	)

if (UNIX)
	set(CMAKE_CXX_FLAGS_DEBUG "-include VS_VectorStorm_Debug.h ${CMAKE_CXX_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-include VS_VectorStorm_Debug.h ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
	set(CMAKE_CXX_FLAGS_RELEASE "-include VS_VectorStorm.h ${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_MINSIZEREL "-include VS_VectorStorm.h ${CMAKE_CXX_FLAGS_MINSIZEREL}")
else (UNIX)
	add_definitions("/FI\"${PREFIX}\"")
endif(UNIX)

source_group("Games\\Robot" FILES ${ROBOT_SOURCES})
source_group("Games\\Preferences" FILES ${PREFERENCES_SOURCES})
source_group("Platform" FILES ${PLATFORM_SOURCES})

IF(APPLE)
	add_definitions("-D__apple_cc__")
	SET(CMAKE_INSTALL_NAME_DIR @executable_path)
	SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

	set_target_properties(RobotFindsIceCream
		PROPERTIES
		#INSTALL_RPATH "@executable_path/../Frameworks"
		INSTALL_RPATH_USER_LINK_PATH TRUE
		#LINK_FLAGS -Wl
		#LINK_FLAGS -Wl,-rpath,@executable_path/../Frameworks/
		XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
		XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "YES"
        XCODE_ATTRIBUTE_FRAMEWORK_SEARCH_PATHS "/Library/Frameworks/"
		#XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH_DEBUG "YES"
		#XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH_RELEASE "NO"
		)

ENDIF(APPLE)
if ( UNIX )
	add_definitions("-D UNIX")
endif ( UNIX )


target_link_libraries( RobotFindsIceCream
	vectorstorm
	${PLATFORM_LIBRARIES}
	${VS_LIBRARIES}
	)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS RobotFindsIceCream
	BUNDLE DESTINATION bin
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)
if(APPLE)
	set_target_properties(RobotFindsIceCream PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/osx/Info.plist")
	add_custom_command(TARGET RobotFindsIceCream
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CFG_INTDIR}/RobotFindsIceCream.app/Contents/Resources
		COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/Data ${CMAKE_CFG_INTDIR}/RobotFindsIceCream.app/Contents/Resources/
		COMMENT "Copying Data files into place"
		)

	# Do a proper app bundle on 'install', taking libraries from /usr/local/lib.
	# (Note that this assumes that all our libraries will be found inside that
	# directory tree.  Should probably extract the directories from our various
	# library variables (SDL_LIBRARY, SDL_MIXER_LIBRARY, etc) and include those
	# directories in the DIRS list as well, just to ensure that everything gets
	# covered, even if people have installed those libraries in other odd places
	# on OSX.  (Folks who have installed SDL etc. through MacPorts probably have
	# these libraries located in /opt/local/lib or somewhere similar, for example)
	set(BUNDLE "${CMAKE_CURRENT_BINARY_DIR}/RobotFindsIceCream.app")
	set(DIRS "/usr/local/lib")
	install(CODE "
		set(BU_CHMOD_BUNDLE_ITEMS ON)
		include(BundleUtilities)
		fixup_bundle(\"${BUNDLE}\" \"\" \"${DIRS}\")
		" COMPONENT Runtime)

endif(APPLE)


