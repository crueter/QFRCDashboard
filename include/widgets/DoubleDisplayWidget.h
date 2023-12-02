#pragma once

#include "TextWidget.h"

class BaseWidget;

class DoubleDisplayWidget : public TextWidget
{
    Q_OBJECT

    Q_PROPERTY(double value MEMBER m_value)
protected:
    double m_value = 0.;

    void keyPressEvent(QKeyEvent *event) override;
public:
    DoubleDisplayWidget(const QString &topic = "", const double &defaultValue = 0., const QString &title = "");
    ~DoubleDisplayWidget();

    void setValue(const nt::Value &value) override;
};
