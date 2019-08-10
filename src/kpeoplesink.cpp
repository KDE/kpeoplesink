#include "kpeoplesink.h"
#include <QDebug>
#include <QImage>
#include <QTimer>
#include <KPeopleBackend/BasePersonsDataSource>
#include <KContacts/VCardConverter>
#include <KContacts/Picture>
#include <KPluginFactory>
#include <KPluginLoader>
#include <sink/store.h>
#include <sink/notifier.h>
#include <sink/notification.h>

using namespace KPeople;
using namespace Sink;
using namespace Sink::ApplicationDomain;
using Sink::ApplicationDomain::SinkResource;

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
    SinkContact(const Sink::ApplicationDomain::Contact & contact)
    {
        auto vcard = contact.getVcard();
        KContacts::VCardConverter converter;
        m_addressee = converter.parseVCard(vcard);
    }

    QVariant customProperty(const QString & key) const override
    {
        QVariant ret;
        if (key == NameProperty) {
            const QString name = m_addressee.realName();
            if (!name.isEmpty()) {
                return name;
            }
            if (!m_addressee.preferredEmail().isEmpty()) {
                return m_addressee.preferredEmail();
            }
            if (!m_addressee.phoneNumbers().isEmpty()) {               
                 // convert from KContacts specific format to QString
                return m_addressee.phoneNumbers().at(0).number();
            }
            return QVariant();
        } 
        else if (key == PictureProperty){
            // convert from KContacts specific format to QString
            return m_addressee.photo().data();
        }
        else if (key == EmailProperty)
            return m_addressee.preferredEmail();

        else if (key == AllPhoneNumbersProperty) {
            QVariantList numbers;
            Q_FOREACH (const KContacts::PhoneNumber &phoneNumber, m_addressee.phoneNumbers()) {
                numbers << phoneNumber.number();
            }
            return numbers;
        } 
        else if (key == PhoneNumberProperty) {
            return m_addressee.phoneNumbers().isEmpty() ? QVariant() : m_addressee.phoneNumbers().at(0).number();
        }

        return ret;
    }

private:
    KContacts::Addressee m_addressee;
};

KPeopleSink::KPeopleSink()
    : KPeople::AllContactsMonitor()
{
    qDebug()<<"KPEOPLESINK";
    QTimer::singleShot(500, this, &KPeopleSink::getContactstoKpeople);
}

void KPeopleSink::getContactstoKpeople(){
    const QList<Sink::ApplicationDomain::Contact> sinkContacts = Sink::Store::read<Sink::ApplicationDomain::Contact>(Sink::Query());
    Q_FOREACH (const Sink::ApplicationDomain::Contact sinkContact, sinkContacts){
        //to get resourceId
        QByteArray resourceId = sinkContact.resourceInstanceIdentifier();
        //get uri
        const QString uri = getUri(sinkContact, resourceId);

        //add uri of contact to set
        m_contactUriHash.insert(uri, sinkContact);

        KPeople::AbstractContact::Ptr contact(new SinkContact(sinkContact));

        m_notifier = new Notifier(resourceId);
        m_notifier->registerHandler([=] (const Sink::Notification &notification) {
            if (notification.type == Notification::Info && notification.code == SyncStatus::SyncSuccess) {
                processRecentlySyncedContacts(resourceId);
            }
        });

        Q_EMIT contactAdded(uri,contact);
    }
}

void KPeopleSink::processRecentlySyncedContacts(QByteArray resourceId){
    const QList<Sink::ApplicationDomain::Contact> sinkContacts = Sink::Store::read<Sink::ApplicationDomain::Contact>(Sink::Query().resourceFilter(resourceId));
    QSet<QString> contactUri;
    Q_FOREACH (const Sink::ApplicationDomain::Contact sinkContact, sinkContacts){
        //get uri
        const QString uri = getUri(sinkContact, resourceId);
        contactUri.insert(uri);
        if(!m_contactUriHash.contains(uri)){
            qDebug()<<"ADD CONTACT";
            m_contactUriHash.insert(uri, sinkContact);
            KPeople::AbstractContact::Ptr contact(new SinkContact(sinkContact));
            Q_EMIT contactAdded(uri,contact);
        }
        else if(m_contactUriHash.value(uri).getVcard() != sinkContact.getVcard()){
            qDebug()<<"CHANGE CONTACT"; 
            m_contactUriHash.insert(uri, sinkContact);
            KPeople::AbstractContact::Ptr contact(new SinkContact(sinkContact));
            Q_EMIT contactChanged(uri,contact);
        }

    }
    toRemoveContact(contactUri);
}

void KPeopleSink::toRemoveContact(QSet<QString> contactUri){
    QHashIterator<QString, Sink::ApplicationDomain::Contact> i(m_contactUriHash);
    while (i.hasNext()) {
        i.next();
        QString uri = i.key();
        if(!contactUri.contains(uri)){
            qDebug()<<" REMOVE CONTACT";
            m_contactUriHash.remove(uri);
            Q_EMIT contactRemoved(uri);
        }
    }
}

QString KPeopleSink::getUri(Sink::ApplicationDomain::Contact sinkContact, QByteArray resourceId)
{
    //to get uid of contact
    QString uid = sinkContact.getUid();
    //to get accountId of contact
    auto resource = Store::readOne<ApplicationDomain::SinkResource>(Sink::Query().filter(resourceId));
    QString accountId = resource.getAccount();
    //create uri for sink contact
    QString uri = "sink://" + accountId + "/" + uid;
    return uri;
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
    return QStringLiteral("sink");
}

AllContactsMonitor* KPeopleSinkDataSource::createAllContactsMonitor()
{
    return new KPeopleSink();
}

K_PLUGIN_FACTORY_WITH_JSON( KPeopleSinkDataSourceFactory, "kpeoplesink.json", registerPlugin<KPeopleSinkDataSource>(); )
K_EXPORT_PLUGIN( KPeopleSinkDataSourceFactory("kpeoplesink") )

#include "kpeoplesink.moc"
