#include "shamsi.h"
#include <QMap>
#include <QDateTime>

const QStringList Shamsi::MonthNames = {"فروردین", "اردیبهشت", "خرداد", "تیر", "اَمرداد", "شهریور", "مهر", "آبان", "آذر", "دی", "بهمن", "اسفند"};
const QStringList Shamsi::WeekDays = {"شنبه", "یک شنبه", "دوشنبه", "سه شنبه", "چهارشنبه", "پنج شنبه", "جمعه"};

Shamsi::Shamsi(QObject *parent)
    : QObject{parent}
{

}

Shamsi::Shamsi(qint64 _timestamp)
{
    this->calculate(_timestamp);
}

void Shamsi::calculate(long _timestamp)
{
    if(_timestamp == 0){
        _timestamp = QDateTime::currentSecsSinceEpoch();
    }
    this->timestamp = _timestamp;
    //* timestamp of 1349/1/1 is 6813000
    _timestamp -= 6813000;
    this->seconds = _timestamp % 60;
    _timestamp /= 60;
    this->minutes = _timestamp % 60;
    _timestamp /= 60;
    this->hours = _timestamp % 24;
    _timestamp /= 24;

    this->dayOfWeek = _timestamp % 7;

    int ty = _timestamp/ 365; //total years past since 1349
    int tl = (ty - (ty+12)/33) / 4;// total of leap years since 1349, every 33 years leep is 5 years
    _timestamp-= tl;

    this->dayofyear = _timestamp % 365;
    this->year = 1349 + (_timestamp/ 365);
    this->isLeap = (ty - (ty+13)/33) % 4 == 3;
    if (this->dayofyear > 186) {
        this->month = 6 + ((this->dayofyear - 186) / 30);
        this->day = this->dayofyear - (6 + this->month * 30);
    } else {
        if(this->summerClock){ // summer +1 clock
            if(this->hours == 23){
                this->hours = 0;
                this->dayofyear++;
            }else
                this->hours++;
        }
        this->month = (this->dayofyear / 31);
        this->day = this->dayofyear - (this->month * 31);
    }
    emit yearChanged();
    emit monthChanged();
    emit monthNameChanged();
    emit dayChanged();
    emit hoursChanged();
    emit minutesChanged();
    emit secondsChanged();
    emit dayofyearChanged();
    emit dayOfWeekChanged();
    emit weekdayChanged();
    emit isLeapChanged();
    emit change();
}
void Shamsi::reCalculate() {
    int ty = this->year - 1349;
    int tl = (ty - (ty+12)/33) / 4;// total of leap years
    this->dayofyear = (this->month * 30 + (this->month < 7 ? this->month : 6) + this->day);
    int days = ty *365 + tl + this->dayofyear;
    this->dayOfWeek = days % 7;
    this->isLeap = (ty - (ty+13)/33) % 4 == 3;
    this->timestamp = ((days *24 + hours)*60+minutes)*60+seconds + 6813000;
    if (this->dayofyear < 186 && this->summerClock) {
        this->timestamp -= 3600;
    }
}
unsigned short Shamsi::getYear(){
    return this->year;
}

unsigned short Shamsi::getMonth(){
    return this->month+1;
}

unsigned short Shamsi::getDay(){
    return this->day;
}

unsigned short Shamsi::getSummerClock(){
    return this->summerClock;
}

void Shamsi::setYear(unsigned short v){
    if(v < 1349)
        return ;
    this->year = v;
    this->reCalculate();
    emit yearChanged();
    emit change();
}

void Shamsi::setMonth(unsigned short v){
    if(v < 0 || v > 12)
        return ;
    this->month = v-1;
    this->reCalculate();
    emit monthChanged();
    emit change();
}

void Shamsi::setDay(unsigned short v){
    this->day = v;
    this->reCalculate();
    emit dayChanged();
    emit change();
}

void Shamsi::setSummerClock(unsigned short v){
    this->summerClock = v;
    this->reCalculate();
    emit summerClockChanged();
    emit change();
}

void Shamsi::fromGregorian(unsigned short gy, unsigned short gm, unsigned short gd){
    int g_d_m[]={0,31,59,90,120,151,181,212,243,273,304,334};
    this->year = (gy<=1600)?0:979;
    gy-=(gy<=1600)?621:1600;
    int gy2=(gm>2)?(gy+1):gy;
    int days=(365*gy) +(((gy2+3)/4)) -(((gy2+99)/100))
            +(((gy2+399)/400)) -80 +gd +g_d_m[gm-1];
    this->year+=33*((days/12053));
    days%=12053;
    this->year+=4*((days/1461));
    days%=1461;
    this->year+=((days-1)/365);
    if(days > 365)days=(days-1)%365;
    this->month =(days < 186)?1+(days/31):7+((days-186)/30);
    this->day =1+((days < 186)?(days%31):((days-186)%30));
    this->reCalculate();
    emit change();
}
QString Shamsi::format(QString str)
{
    QString result = "";
    for (auto chr : str) {
        switch (chr.unicode()) {
        case 'd':
            result += (this->day < 10 ? "0" + QString::number(this->day) : QString::number(this->day));
            break;
        case 'D':
            result += Shamsi::WeekDays[this->dayOfWeek].mid(0,1);
            break;
        case 'j':
            result += QString::number(this->day);
            break;
        case 'l':
            result += this->getWeekDayName();
            break;
        case 'N':
            result += QString::number((this->day - 2) % 7 + 1);
            break;
        case 'S':
            result += this->getDayName();
            break;
        case 'w':
            result += QString::number(this->day);
            break;
        case 'W':
            result += this->getDayName();
            break;
        case 'z':
            result += QString::number(this->dayofyear);
            break;
        case 'F':
            result += this->getMonthName();
            break;
        case 'M':
            result += this->getMonthName().mid(0, 3);
            break;
        case 'm':
            result += (this->month < 9 ? "0" + QString::number(this->month + 1) : QString::number(this->month + 1));
            break;
        case 'n':
            result += QString::number(this->month+1);
            break;
        case 't':
            result += QString::number(this->getMonthDays());
            break;
        case 'L':
            if(this->isLeap) result += "کبیسه است";
            break;
        case 'Y':
            result += QString::number(this->year);
            break;
        case 'y':
            result += QString::number(this->year).mid(2,2);
            break;
        case 'a':
            result += (this->hours < 12 ? "ق.ظ" : "ب.ظ");
            break;
        case 'A':
            result += (this->hours < 12 ? "AM" : "PM");
            break;
        case 'g':
            result += QString::number(this->hours % 12 ? this->hours % 12 : 12);
            break;
        case 'G':
            result += QString::number(this->hours);
            break;
        case 'h':{
            short h = (this->hours % 12 ? this->hours % 12 : 12);
            result += (h < 10 ? "0" + QString::number(h) : QString::number(h));
        }break;
        case 'H':
            result += (this->hours < 10 ? "0" + QString::number(this->hours) : QString::number(this->hours));
            break;
        case 'i':
            result += (this->minutes < 10 ? "0" + QString::number(this->minutes) : QString::number(this->minutes));
            break;
        case 's':
            result += (this->seconds < 10 ? '0' + QString::number(this->seconds) : QString::number(this->seconds));
            break;
        default:
            result += chr;
            break;
        }
    }
    return result;
}

short Shamsi::getMonthDays()
{
    if(this->month < 6) return 31;
    if(this->month < 10 || this->isLeap) return 30;
    return 29;
}
QString Shamsi::getDayName() {
    static QMap<short, QString> names{
        {1, "یکم"},
        {2, "دوم"},
        {3, "سوم"},
        {4, "چهارم"},
        {5, "پنجم"},
        {6, "ششم"},
        {7, "هفتم"},
        {8, "هشتم"},
        {9, "نهم"},
        {10, "دهم"},
        {11, "یازدهم"},
        {12, "دوازدهم"},
        {13, "سیزدهم"},
        {14, "چهاردهم"},
        {15, "پانزدهم"},
        {16, "شانزدهم"},
        {17, "هفدهم"},
        {18, "هجدهم"},
        {19, "نوزدهم"},
        {20, "بیست"},
        {30, "سی"}
    };
    if (this->day == 1) {
        return "اول";
    } else if (this->day > 19) {
        auto r = this->day % 10;
        return names[this->day - r] + (r > 0 ? " و " + names[r] : "م");
    } else {
        return names[this->day];
    }
}
QString Shamsi::getWeekDayName()
{
    return Shamsi::WeekDays[this->dayOfWeek];
}
QString Shamsi::getMonthName()
{
    return Shamsi::MonthNames[this->month];
}

unsigned short Shamsi::getDayofYear()
{
    return this->dayofyear;
}
unsigned short Shamsi::getDayOfWeek()
{
    return this->dayOfWeek;
}
bool Shamsi::getIsLeap()
{
    return this->isLeap;
}
long Shamsi::getTimestamp()
{
    this->reCalculate();
    return this->timestamp;
}
