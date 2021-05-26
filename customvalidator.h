#ifndef CUSTOMVALIDATOR_H
#define CUSTOMVALIDATOR_H

#include <QDoubleValidator>

class CustomDoubleValidator : public QDoubleValidator {
public:
    CustomDoubleValidator (QObject * parent = 0) : QDoubleValidator(parent) {}
    CustomDoubleValidator (double bottom, double top, int decimals, QObject * parent) :
    QDoubleValidator(bottom, top, decimals, parent) {}

    QValidator::State validate(QString & s, int & pos) const {
        if (s.isEmpty() || (s.startsWith("-") && s.length() == 1)) {
            // allow empty field or standalone minus sign
            return QValidator::Intermediate;
        }
        // check length of decimal places
        QChar point = locale().decimalPoint();
        if(s.indexOf(point) != -1) {
            int lengthDecimals = s.length() - s.indexOf(point) - 1;
            if (lengthDecimals > decimals()) {
                return QValidator::Invalid;
            }
        }
        // check range of value
        bool isNumber;
        double value = locale().toDouble(s, &isNumber);
        if (isNumber && bottom() <= value && value <= top()) {
            return QValidator::Acceptable;
        }
        return QValidator::Invalid;
    }

};

class CustomIntValidator : public QIntValidator {
public:
    CustomIntValidator (QObject * parent = 0) : QIntValidator(parent) {}
    CustomIntValidator (int bottom, int top, QObject * parent) :
    QIntValidator(bottom, top, parent) {}

    QValidator::State validate(QString & s, int & pos) const {
        if (s.isEmpty() || (s.startsWith("-") && s.length() == 1)) {
            // allow empty field or standalone minus sign
            return QValidator::Intermediate;
        }
        // check range of value
        bool isNumber;
        int value = locale().toDouble(s, &isNumber);
        if (isNumber && bottom() <= value && value <= top()) {
            return QValidator::Acceptable;
        }
        return QValidator::Invalid;
    }

};

#endif // CUSTOMVALIDATOR_H
