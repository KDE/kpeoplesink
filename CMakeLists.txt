cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)

project(KPeopleSink)

set(QT5_REQUIRED_VERSION 5.12)
set(KF5_REQUIRED_VERSION 5.60)

find_package(ECM ${KF5_REQUIRED_VERSION} REQUIRED NO_MODULE)
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

find_package(Qt5 ${QT5_REQUIRED_VERSION} REQUIRED COMPONENTS Core Widgets)
find_package(KF5 ${KF5_REQUIRED_VERSION} REQUIRED COMPONENTS CoreAddons People Contacts)
find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Network)
find_package(Sink CONFIG REQUIRED)
find_package(KAsync CONFIG REQUIRED)

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
