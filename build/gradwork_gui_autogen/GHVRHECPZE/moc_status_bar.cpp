/****************************************************************************
** Meta object code from reading C++ file 'status_bar.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/Widgets/status_bar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'status_bar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Status_Bar_t {
    const uint offsetsAndSize[12];
    char stringdata0[56];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Status_Bar_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Status_Bar_t qt_meta_stringdata_Status_Bar = {
    {
QT_MOC_LITERAL(0, 10), // "Status_Bar"
QT_MOC_LITERAL(11, 10), // "slot_reset"
QT_MOC_LITERAL(22, 0), // ""
QT_MOC_LITERAL(23, 11), // "slot_update"
QT_MOC_LITERAL(35, 10), // "slot_start"
QT_MOC_LITERAL(46, 9) // "slot_stop"

    },
    "Status_Bar\0slot_reset\0\0slot_update\0"
    "slot_start\0slot_stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Status_Bar[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   38,    2, 0x0a,    1 /* Public */,
       3,    0,   39,    2, 0x0a,    2 /* Public */,
       4,    0,   40,    2, 0x0a,    3 /* Public */,
       5,    0,   41,    2, 0x0a,    4 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Status_Bar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Status_Bar *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->slot_reset(); break;
        case 1: _t->slot_update(); break;
        case 2: _t->slot_start(); break;
        case 3: _t->slot_stop(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject Status_Bar::staticMetaObject = { {
    QMetaObject::SuperData::link<QStatusBar::staticMetaObject>(),
    qt_meta_stringdata_Status_Bar.offsetsAndSize,
    qt_meta_data_Status_Bar,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Status_Bar_t
, QtPrivate::TypeAndForceComplete<Status_Bar, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Status_Bar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Status_Bar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Status_Bar.stringdata0))
        return static_cast<void*>(this);
    return QStatusBar::qt_metacast(_clname);
}

int Status_Bar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QStatusBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
