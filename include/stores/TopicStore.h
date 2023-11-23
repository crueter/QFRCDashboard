#ifndef TopicStore_H
#define TopicStore_H

#include "networktables/NetworkTableEntry.h"

#include <QMultiHash>
#include <QHash>
#include <QObject>

class BaseWidget;

class TopicStore : public QObject
{
    Q_OBJECT
private:
    static QHash<std::string, nt::NetworkTableEntry *> topicEntryMap;
    static QMultiHash<std::string, BaseWidget *> topicSubscriberMap;
public:
    TopicStore();

    static nt::NetworkTableEntry *subscribe(std::string ntTopic, BaseWidget *subscriber);

    static void unsubscribe(std::string ntTopic, BaseWidget *subscriber);
    static void unsubscribe(nt::NetworkTableEntry *entry, BaseWidget *subscriber);
};

#endif // TopicStore_H
