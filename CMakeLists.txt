cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)

project(KPeopleSink)

################# set KDE specific information #################

find_package(ECM 1.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(ECMSetupVersion)
include(CMakePackageConfigHelpers)
include(FeatureSummary)
include(WriteBasicConfigVersionFile)
include(CheckFunctionExists)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMAddTests)
include(KDEClangFormat)

################# Find dependencies #################

find_package(Qt5 "5.2" REQUIRED COMPONENTS Widgets)
find_package(KF5 "5.8" REQUIRED COMPONENTS CoreAddons People)
find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Network)
find_package(Sink CONFIG REQUIRED)
find_package(KAsync CONFIG REQUIRED)
find_package(KF5Contacts REQUIRED)

################# build and install #################

add_subdirectory(src)

set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5PeopleSink")
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5PeopleSinkConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5PeopleSinkConfig.cmake"
  INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/KF5PeopleSinkConfig.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel )


# add clang-format target for all our real source files
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
