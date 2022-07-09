#ifndef SHAMSI_H
#define SHAMSI_H

#include <QObject>
#include <QStringList>

class Shamsi : public QObject
{
    Q_OBJECT
public:
    explicit Shamsi(QObject *parent = nullptr);
    Shamsi(short year, short month, short day, short hours, short minutes, short seconds);
    Shamsi(qint64 timestamp);

    Q_PROPERTY(unsigned short year READ getYear WRITE setYear NOTIFY yearChanged NOTIFY change);
    Q_PROPERTY(unsigned short month READ getMonth WRITE setMonth NOTIFY monthChanged NOTIFY change);
    Q_PROPERTY(QString monthName READ getMonthName NOTIFY monthNameChanged NOTIFY change);
    Q_PROPERTY(unsigned short day READ getDay WRITE setDay NOTIFY dayChanged NOTIFY change);
    Q_PROPERTY(unsigned short hours MEMBER hours NOTIFY hoursChanged NOTIFY change);
    Q_PROPERTY(unsigned short minutes MEMBER minutes NOTIFY minutesChanged NOTIFY change);
    Q_PROPERTY(unsigned short seconds MEMBER seconds NOTIFY secondsChanged NOTIFY change);
    Q_PROPERTY(unsigned short dayofyear READ getDayofYear NOTIFY dayofyearChanged NOTIFY change);
    Q_PROPERTY(unsigned short dayOfWeek READ getDayOfWeek NOTIFY dayOfWeekChanged NOTIFY change);
    Q_PROPERTY(QString weekday READ getWeekDayName NOTIFY weekdayChanged NOTIFY change);
    Q_PROPERTY(bool isLeap READ getIsLeap NOTIFY isLeapChanged NOTIFY change);
    Q_PROPERTY(bool summerClock READ getSummerClock WRITE setSummerClock NOTIFY summerClockChanged NOTIFY change);
    Q_PROPERTY(long timestamp READ getTimestamp WRITE calculate NOTIFY timestampChanged NOTIFY change);

    Q_INVOKABLE void calculate(long);
    Q_INVOKABLE void fromGregorian(unsigned short,unsigned short,unsigned short);
    Q_INVOKABLE QString format(QString);

    Q_INVOKABLE short getMonthDays();
    Q_INVOKABLE QString getDayName();

    QString getWeekDayName();
    QString getMonthName();
    unsigned short getDayofYear();
    unsigned short getDayOfWeek();
    bool getIsLeap();
    long getTimestamp();
    unsigned short getYear();
    unsigned short getMonth();
    unsigned short getDay();
    unsigned short getSummerClock();
    void setYear(unsigned short);
    void setMonth(unsigned short);
    void setDay(unsigned short);
    void setSummerClock(unsigned short);

    const static QStringList MonthNames;
    const static QStringList WeekDays;

private:
    unsigned short year, month, day, hours, minutes, seconds, dayofyear, dayOfWeek;
    bool isLeap;
    bool summerClock = true;
    long timestamp;

    void reCalculate();

signals:
    void yearChanged();
    void monthChanged();
    void monthNameChanged();
    void dayChanged();
    void hoursChanged();
    void minutesChanged();
    void secondsChanged();
    void dayofyearChanged();
    void dayOfWeekChanged();
    void weekdayChanged();
    void isLeapChanged();
    void summerClockChanged();
    void timestampChanged();

    void change();
};

#endif // SHAMSI_H
