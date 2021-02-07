cmake_minimum_required(VERSION 3.16)

project(KPeopleSink CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT5_REQUIRED_VERSION 5.15)
set(KF5_REQUIRED_VERSION 5.78)

find_package(ECM ${KF5_REQUIRED_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(ECMSetupVersion)
include(CMakePackageConfigHelpers)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMAddTests)
include(KDEClangFormat)

find_package(Qt5 ${QT5_REQUIRED_VERSION} REQUIRED COMPONENTS Core Widgets Network)
find_package(KF5 ${KF5_REQUIRED_VERSION} REQUIRED COMPONENTS CoreAddons People Contacts)
find_package(Sink REQUIRED)
find_package(KAsync REQUIRED)
find_package(KAccounts REQUIRED)

add_definitions(-DQT_NO_FOREACH)

add_subdirectory(src)

# add clang-format target for all our real source files
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
