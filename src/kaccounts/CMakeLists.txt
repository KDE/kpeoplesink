set(sink_carddav_plugin_SRCS
    sink-carddav-plugin.cpp
    sync-contacts.cpp
    )

kcoreaddons_add_plugin(sink_carddav_plugin
    JSON sink-carddav.json
    SOURCES ${sink_carddav_plugin_SRCS}
    INSTALL_NAMESPACE kaccounts/daemonplugins)


kde_enable_exceptions(sink_carddav_plugin)

target_link_libraries(
    sink_carddav_plugin 
    Qt5::Core 
    KAccounts
    KAsync
    sink
    )
