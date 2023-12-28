#pragma once

#include <QCoreApplication>
#include <QVector>
#include "MotionAxisList.h"

class MOTION_EXPORT MotionPose
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(MotionPose)
    QVector<float> d;

public:
    typedef QVector<float> Base;
    typedef Base::iterator iterator;
    typedef Base::const_iterator const_iterator;
    typedef Base::value_type value_type;

    MotionPose() = default;
    MotionPose(int count);
    MotionPose(const QVector<float> &vector);

    inline bool isEmpty() const {return d.isEmpty();}
    inline int count() const {return d.count();}
    inline void resize(int size) {d.resize(size);}
    inline float value(int index) const {return d.value(index);}
    inline float value(int index, const float &defaultValue) const {return d.value(index, defaultValue);}
    inline void append(const float &value) {d.append(value);}
    inline iterator begin() {return d.begin();}
    inline iterator end() {return d.end();}
    inline const_iterator begin() const {return d.begin();}
    inline const_iterator end() const {return d.end();}
    inline float takeAt(int i) {return d.takeAt(i);}
    inline MotionPose &operator <<(float v) {d << v; return *this;}
    inline float &operator [](int index) {return d[index];}
    inline const float &operator [](int index) const {return d[index];}

    static MotionPose fromJson(const QJsonValue &json);
    QJsonValue toJson() const;

    QString toString() const;
    static MotionPose fromString(const QString &text);
    QList<qreal> filterPose(const MotionAxisList &axes) const;
    qreal distanceOnAxis(const MotionPose &other, int axis);
    void addOffset(qreal offset, int axis);
    void ensureSize(int count);
    bool isEqualTo(const MotionAxisList &list, const MotionPose &other, double limit = 0.01) const;  /* 与另一个位置是否相同 */

    inline MotionPose operator +(const MotionPose &other) const;
    inline MotionPose operator -(const MotionPose &other) const;
    inline MotionPose operator *(qreal c) const;
    inline MotionPose operator /(qreal c) const;
    MotionPose &operator +=(const MotionPose &other);
    MotionPose &operator +=(qreal c);
    MotionPose &operator -=(const MotionPose &other);
    MotionPose &operator -=(qreal c);
    MotionPose &operator *=(qreal c);
    MotionPose &operator /=(qreal c);
    MotionPose operator -() const;

    inline operator QVector<float> () const {return d;}
    bool operator ==(const MotionPose &other) const;
    inline bool operator !=(const MotionPose &other) const {return !(*this == other);}
};

inline QDebug operator <<(QDebug stream, const MotionPose &pose)
{
    return stream << QVector<float>(pose);
}

MotionPose MotionPose::operator +(const MotionPose &other) const
{
    MotionPose copy(*this);
    copy += other;
    return copy;
}

MotionPose MotionPose::operator -(const MotionPose &other) const
{
    MotionPose copy(*this);
    copy -= other;
    return copy;
}

MotionPose MotionPose::operator *(qreal c) const
{
    MotionPose copy(*this);
    copy *= c;
    return copy;
}

MotionPose MotionPose::operator /(qreal c) const
{
    MotionPose copy(*this);
    copy /= c;
    return copy;
}
