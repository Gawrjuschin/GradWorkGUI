/****************************************************************************
** Meta object code from reading C++ file 'input_widget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/Widgets/input_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'input_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Input_Widget_t {
    const uint offsetsAndSize[22];
    char stringdata0[122];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Input_Widget_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Input_Widget_t qt_meta_stringdata_Input_Widget = {
    {
QT_MOC_LITERAL(0, 12), // "Input_Widget"
QT_MOC_LITERAL(13, 12), // "signal_start"
QT_MOC_LITERAL(26, 0), // ""
QT_MOC_LITERAL(27, 12), // "signal_pause"
QT_MOC_LITERAL(40, 13), // "signal_resume"
QT_MOC_LITERAL(54, 11), // "signal_stop"
QT_MOC_LITERAL(66, 9), // "slot_done"
QT_MOC_LITERAL(76, 6), // "update"
QT_MOC_LITERAL(83, 10), // "slot_start"
QT_MOC_LITERAL(94, 17), // "slot_pause_resume"
QT_MOC_LITERAL(112, 9) // "slot_stop"

    },
    "Input_Widget\0signal_start\0\0signal_pause\0"
    "signal_resume\0signal_stop\0slot_done\0"
    "update\0slot_start\0slot_pause_resume\0"
    "slot_stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Input_Widget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x06,    1 /* Public */,
       3,    0,   69,    2, 0x06,    2 /* Public */,
       4,    0,   70,    2, 0x06,    3 /* Public */,
       5,    0,   71,    2, 0x06,    4 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,   72,    2, 0x0a,    5 /* Public */,
       7,    0,   73,    2, 0x09,    6 /* Protected */,
       8,    0,   74,    2, 0x09,    7 /* Protected */,
       9,    0,   75,    2, 0x09,    8 /* Protected */,
      10,    0,   76,    2, 0x09,    9 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
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

void Input_Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Input_Widget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signal_start(); break;
        case 1: _t->signal_pause(); break;
        case 2: _t->signal_resume(); break;
        case 3: _t->signal_stop(); break;
        case 4: _t->slot_done(); break;
        case 5: _t->update(); break;
        case 6: _t->slot_start(); break;
        case 7: _t->slot_pause_resume(); break;
        case 8: _t->slot_stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Input_Widget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Input_Widget::signal_start)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Input_Widget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Input_Widget::signal_pause)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Input_Widget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Input_Widget::signal_resume)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Input_Widget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Input_Widget::signal_stop)) {
                *result = 3;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject Input_Widget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Input_Widget.offsetsAndSize,
    qt_meta_data_Input_Widget,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Input_Widget_t
, QtPrivate::TypeAndForceComplete<Input_Widget, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Input_Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Input_Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Input_Widget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Input_Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Input_Widget::signal_start()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Input_Widget::signal_pause()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Input_Widget::signal_resume()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Input_Widget::signal_stop()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE