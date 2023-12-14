#pragma once

#include <ntcore/networktables/NetworkTableInstance.h>

#include <QString>
#include <QMap>
#include <QMetaType>
#include <QMetaEnum>
#include <QFile>

// ENUMS //
enum class WidgetTypes {
    BooleanCheckbox = 0,
    BooleanDisplay = 1,
    StringDisplay = 2,
    DoubleDisplay = 3,
    DoubleDial = 4,
    SendableChooser = 5,
    CameraView = 6,
    EnumWidget = 7,
    IntegerDisplay = 8,
    IntegerDial = 9,
    SendableField = 10, // UNUSED BY SAVE/LOAD
    Field = 11,
    Command = 12
};

Q_DECLARE_FLAGS(WidgetType, WidgetTypes);
Q_DECLARE_OPERATORS_FOR_FLAGS(WidgetType);

enum class TopicTypes {
    SendableChooser,
    Field2d,
    Command,

    Double,
    DoubleArray,
    String,
    Boolean,
    Int
};

Q_DECLARE_FLAGS(TopicType, TopicTypes);
Q_DECLARE_OPERATORS_FOR_FLAGS(TopicType);

// STRUCTS //
typedef struct {
    bool teamNumber;
    std::string server;
    int port;
} ServerData;

typedef struct {
    int tabIdx;
    int row;
    int col;
    int rowSpan;
    int colSpan;
} WidgetData;

class TypeStore;


// NAMESPACES //
namespace Globals {

Q_NAMESPACE

enum class FrameShape {
    Circle,
    Triangle,
    Rectangle,
    Hexagon
};

Q_ENUM_NS(FrameShape)

extern nt::NetworkTableInstance inst;
extern ServerData server;

extern TypeStore typeStore;

extern QMap<TopicTypes, QString> topicTypeDisplayNames;

extern QStringList ntTopics;
extern WidgetData *defaultWidgetData;

extern QMap<QString, FrameShape> shapeNameMap;

typedef struct {
    QString fileName;
} File;

typedef struct Topic {
    QString name;
    TopicTypes type;

    bool operator==(const struct Topic &other) const;
} Topic;
}

Q_DECLARE_METATYPE(Globals::File)
Q_DECLARE_METATYPE(Globals::Topic)

namespace CustomMetaTypes {
static const int FrameShape = qMetaTypeId<Globals::FrameShape>();
static const int File = qMetaTypeId<Globals::File>();
static const int Topic = qMetaTypeId<Globals::Topic>();
}


extern bool operator==(const WidgetData &a, const WidgetData &b);
// extern bool operator==(const Globals::Topic &a, const Globals::Topic &b);
