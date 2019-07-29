kcoreaddons_add_plugin(KPeopleSink SOURCES kpeoplesink.cpp JSON "kpeoplesink.json" INSTALL_NAMESPACE "kpeople/datasource")
target_link_libraries(
    KPeopleSink 
    Qt5::Network
    KF5::PeopleBackend 
    KF5::CoreAddons 
    KF5::Contacts
    sink
    KAsync
    )
