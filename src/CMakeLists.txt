set(KPEOPLESINK_SRCS
    kpeoplesink.cpp
    sinkdatasource.cpp
    sinkcontact.cpp
)


kcoreaddons_add_plugin(KPeopleSink SOURCES ${KPEOPLESINK_SRCS} JSON "kpeoplesink.json" INSTALL_NAMESPACE "kpeople/datasource")

kde_enable_exceptions(KPeopleSink)

target_link_libraries(
    KPeopleSink 
    Qt5::Network
    KF5::PeopleBackend 
    KF5::CoreAddons 
    KF5::Contacts
    sink
    KAsync
    )
