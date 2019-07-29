#ifndef KPEOPLESINK_H
#define KPEOPLESINK_H

#include <KPeopleBackend/AllContactsMonitor>
#include <KPeopleBackend/AbstractContact>

class Q_DECL_EXPORT KPeopleSink : public KPeople::AllContactsMonitor
{
    Q_OBJECT

public:
    KPeopleSink();
    virtual ~KPeopleSink();

public slots:
    void getContactstoKpeople();
};

#endif