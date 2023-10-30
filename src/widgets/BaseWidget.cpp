#include "widgets/BaseWidget.h"
#include "Globals.h"

#include "misc/ResizeDialog.h"

#include "widgets/BooleanCheckboxWidget.h"
#include "widgets/BooleanDisplayWidget.h"
#include "widgets/CameraViewWidget.h"
#include "widgets/DoubleDialWidget.h"
#include "widgets/NumberDisplayWidget.h"
#include "widgets/StringChooserWidget.h"
#include "widgets/StringDisplayWidget.h"

#include <QMenu>
#include <QFontDialog>
#include <QStyleOption>
#include <QPainter>
#include <QJsonArray>

BaseWidget::BaseWidget(const QString &title, const QString &topic)
{
    m_layout = new QGridLayout(this);
    m_title = new QLineEdit(title, this);
    m_entry = Globals::inst.GetEntry(topic.toStdString());

    m_layout->addWidget(m_title, 0, 0);

    setStyleSheet("BaseWidget { background-color: black; border: 1px solid white; color: white; }");
    m_title->setStyleSheet("QLineEdit { border: none; border-bottom: 2px solid white; }");

    QFont titleFont = this->titleFont();
    titleFont.setBold(true);
    setTitleFont(titleFont);

    m_title->setAlignment(Qt::AlignHCenter);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QFont BaseWidget::titleFont()
{
    return m_title->font();
}

void BaseWidget::setTitleFont(const QFont &font)
{
    m_title->setFont(font);
}

BaseWidget::~BaseWidget() {}

QString BaseWidget::title()
{
    return m_title->text();
}

void BaseWidget::setTitle(const QString &title)
{
    m_title->setText(title);
}

std::string BaseWidget::topic()
{
    return m_entry.GetName();
}

void BaseWidget::setTopic(const std::string &topic)
{
    m_entry.Unpublish();
    m_entry = Globals::inst.GetEntry(topic);
}

QMenu *BaseWidget::constructContextMenu(WidgetData data) {
    QMenu *menu = new QMenu(this);

    QAction *resizeAction = new QAction("Resize", menu);

    menu->addAction(resizeAction);

    connect(resizeAction, &QAction::triggered, this, [this, data](bool) {
        ResizeDialog *dialog = new ResizeDialog(data);
        dialog->show();

        connect(dialog, &ResizeDialog::resizeReady, this, [this](WidgetData data) {
            emit resizeRequested(data);
        });
    });

    QAction *deleteAction = new QAction("Delete Widget", menu);
    menu->addAction(deleteAction);

    connect(deleteAction, &QAction::triggered, this, [this](bool) {
        emit deleteRequested();
    });

    QAction *titleFontAction = new QAction("Title Font", menu);
    menu->addAction(titleFontAction);

    connect(titleFontAction, &QAction::triggered, this, [this](bool) {
        setTitleFont(QFontDialog::getFont(0, titleFont(), this, "Set Title Font"));
    });

    return menu;
}

void BaseWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

QJsonObject BaseWidget::saveObject() {
    QJsonObject object{};

    object.insert("title", title());
    object.insert("topic", QString::fromStdString(m_entry.GetName()));
    object.insert("titleFont", titleFont().toString());

    return object;
}

std::pair<BaseWidget *, WidgetData> BaseWidget::fromJson(QJsonObject obj, int tabIdx) {
    WidgetTypes widgetType = (WidgetTypes) obj.value("widgetType").toInt();

    BaseWidget *widget;

    switch (widgetType) {
    case WidgetTypes::BooleanCheckbox: {
        widget = new BooleanCheckboxWidget(
            obj.value("title").toString(),
            obj.value("value").toBool(),
            obj.value("topic").toString());
        break;
    }
    case WidgetTypes::BooleanDisplay: {
        BooleanDisplayWidget *displayWidget = new BooleanDisplayWidget(
            obj.value("title").toString(),
            obj.value("value").toBool(),
            obj.value("topic").toString());

        displayWidget->setTrueColor(QColor::fromString(obj.value("trueColor").toString()));
        displayWidget->setFalseColor(QColor::fromString(obj.value("falseColor").toString()));

        widget = displayWidget;
        break;
    }
    case WidgetTypes::DoubleDisplay: {
        NumberDisplayWidget *displayWidget = new NumberDisplayWidget(
            obj.value("title").toString(),
            obj.value("value").toDouble(),
            obj.value("topic").toString());

        QFont font;
        font.fromString(obj.value("textFont").toString());
        displayWidget->setFont(font);

        widget = displayWidget;
        break;
    }
    case WidgetTypes::DoubleDial: {
        DoubleDialWidget *dialWidget = new DoubleDialWidget(
            obj.value("title").toString(),
            obj.value("value").toDouble(),
            obj.value("topic").toString());

        QFont font;
        font.fromString(obj.value("textFont").toString());
        dialWidget->setFont(font);

        dialWidget->setRange(QPoint(obj.value("min").toDouble(), obj.value("max").toDouble()));

        widget = dialWidget;
        break;
    }
    case WidgetTypes::SendableChooser: {
        widget = new StringChooserWidget(
            obj.value("title").toString(),
            obj.value("topic").toString());

        break;
    }
    case WidgetTypes::CameraView: {
        widget = new CameraViewWidget(
            obj.value("title").toString(),
            QUrl(obj.value("url").toString()));

        break;
    }
    case WidgetTypes::StringDisplay:
    default: {
        StringDisplayWidget *displayWidget = new StringDisplayWidget(
            obj.value("title").toString(),
            obj.value("value").toString(),
            obj.value("topic").toString());

        QFont font;
        font.fromString(obj.value("textFont").toString());
        displayWidget->setFont(font);

        widget = displayWidget;
        break;
    }
    } // switch

    QFont titleFont;
    titleFont.fromString(obj.value("titleFont").toString());
    widget->setTitleFont(titleFont);

    QJsonArray geometry = obj.value("geometry").toArray();
    WidgetData data;
    data.tabIdx = tabIdx;
    data.row = geometry.at(0).toInt();
    data.col = geometry.at(1).toInt();
    data.rowSpan = geometry.at(2).toInt();
    data.colSpan = geometry.at(3).toInt();

    return std::make_pair(widget, data);
}
