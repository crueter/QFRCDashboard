#include "Globals.h"

#include <QStringList>

nt::NetworkTableInstance Globals::inst = nt::NetworkTableInstance::GetDefault();
ServerData Globals::server{false, "0.0.0.0", NT_DEFAULT_PORT4};

QMap<Globals::TopicTypes, QString> Globals::typeDisplayNames = {
    {Globals::TopicTypes::SendableChooser, "Sendable Chooser"},
    {Globals::TopicTypes::Double, "Double"},
    {Globals::TopicTypes::String, "String"},
    {Globals::TopicTypes::Boolean, "Boolean"}
};

QStringList Globals::ntTopics{};
QMap<QString, Globals::TopicTypes> Globals::availableTopics;

bool operator==(const WidgetData &a, const WidgetData &b) {
    return (a.row == b.row) &&
           (a.col == b.col) &&
           (a.rowSpan == b.rowSpan) &&
           (a.colSpan == b.rowSpan) &&
           (a.tabIdx == b.tabIdx);
}
