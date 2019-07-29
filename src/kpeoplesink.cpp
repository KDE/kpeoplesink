#include "kpeoplesink.h"
#include <QDebug>
#include <KPeopleBackend/BasePersonsDataSource>

#include <KPluginFactory>
#include <KPluginLoader>

#include <sink/store.h>

using namespace KPeople;

class KPeopleSinkDataSource : public KPeople::BasePersonsDataSource
{
public:
    KPeopleSinkDataSource(QObject *parent, const QVariantList &data);
    virtual ~KPeopleSinkDataSource();
    QString sourcePluginId() const override;

    KPeople::AllContactsMonitor* createAllContactsMonitor() override;
};

class SinkContact : public AbstractContact
{
public:
    SinkContact() {}
    SinkContact(const Sink::ApplicationDomain::Contact & contact) : mContact(contact) {}

    QVariant customProperty(const QString & key) const override
    {
        QVariant ret;
        if (key == NameProperty) {
            const QString name = mContact.getProperty("fn").toString();
            if (!name.isEmpty()) {
                return name;
            }
            return QVariant();
        } else if (key == EmailProperty)
            return mContact.getProperty("emails");
        else if (key == PictureProperty)
            return mContact.getProperty("photo");       

        return ret;
    }

private:
    Sink::ApplicationDomain::Contact mContact;
};

KPeopleSink::KPeopleSink()
    : KPeople::AllContactsMonitor()
{
}

void KPeopleSink::getContactstoKpeople(){
    qDebug()<<"KPEOPLESINK";
    const QList<Sink::ApplicationDomain::Contact> sinkContacts = Sink::Store::read<Sink::ApplicationDomain::Contact>(Sink::Query());
    Q_FOREACH (const Sink::ApplicationDomain::Contact sinkContact, sinkContacts){

        QString uid = sinkContact.getProperty("uid").toString();
        QString resourceId = sinkContact.resourceInstanceIdentifier();
        const QString uri = "sink://" + resourceId + "/" + uid;
        qDebug()<<uri;
        KPeople::AbstractContact::Ptr contact(new SinkContact(sinkContact));
        Q_EMIT contactAdded(uri,contact);
    }
}

KPeopleSink::~KPeopleSink()
{}

KPeopleSinkDataSource::KPeopleSinkDataSource(QObject *parent, const QVariantList &args)
: BasePersonsDataSource(parent)
{
    Q_UNUSED(args);
}

KPeopleSinkDataSource::~KPeopleSinkDataSource()
{
}

QString KPeopleSinkDataSource::sourcePluginId() const
{
    return QStringLiteral("vcard");
}

AllContactsMonitor* KPeopleSinkDataSource::createAllContactsMonitor()
{
    return new KPeopleSink();
}

K_PLUGIN_FACTORY_WITH_JSON( KPeopleSinkDataSourceFactory, "kpeoplesink.json", registerPlugin<KPeopleSinkDataSource>(); )
K_EXPORT_PLUGIN( KPeopleSinkDataSourceFactory("kpeoplesink") )

#include "kpeoplesink.moc"
