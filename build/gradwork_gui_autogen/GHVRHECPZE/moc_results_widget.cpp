/****************************************************************************
** Meta object code from reading C++ file 'results_widget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/Widgets/results_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'results_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Results_Widget_t {
    const uint offsetsAndSize[18];
    char stringdata0[98];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Results_Widget_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Results_Widget_t qt_meta_stringdata_Results_Widget = {
    {
QT_MOC_LITERAL(0, 14), // "Results_Widget"
QT_MOC_LITERAL(15, 14), // "signal_prepare"
QT_MOC_LITERAL(30, 0), // ""
QT_MOC_LITERAL(31, 12), // "signal_ready"
QT_MOC_LITERAL(44, 10), // "slot_start"
QT_MOC_LITERAL(55, 10), // "slot_pause"
QT_MOC_LITERAL(66, 11), // "slot_resume"
QT_MOC_LITERAL(78, 9), // "slot_stop"
QT_MOC_LITERAL(88, 9) // "slot_done"

    },
    "Results_Widget\0signal_prepare\0\0"
    "signal_ready\0slot_start\0slot_pause\0"
    "slot_resume\0slot_stop\0slot_done"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Results_Widget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x06,    1 /* Public */,
       3,    0,   57,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,   58,    2, 0x0a,    3 /* Public */,
       5,    0,   59,    2, 0x0a,    4 /* Public */,
       6,    0,   60,    2, 0x0a,    5 /* Public */,
       7,    0,   61,    2, 0x0a,    6 /* Public */,
       8,    0,   62,    2, 0x0a,    7 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Results_Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Results_Widget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signal_prepare(); break;
        case 1: _t->signal_ready(); break;
        case 2: _t->slot_start(); break;
        case 3: _t->slot_pause(); break;
        case 4: _t->slot_resume(); break;
        case 5: _t->slot_stop(); break;
        case 6: _t->slot_done(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Results_Widget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Results_Widget::signal_prepare)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Results_Widget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Results_Widget::signal_ready)) {
                *result = 1;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject Results_Widget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Results_Widget.offsetsAndSize,
    qt_meta_data_Results_Widget,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Results_Widget_t
, QtPrivate::TypeAndForceComplete<Results_Widget, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Results_Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Results_Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Results_Widget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Results_Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Results_Widget::signal_prepare()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Results_Widget::signal_ready()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
