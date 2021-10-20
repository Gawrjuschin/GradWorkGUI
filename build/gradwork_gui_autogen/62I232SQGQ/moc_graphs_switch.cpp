/****************************************************************************
** Meta object code from reading C++ file 'graphs_switch.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/Widgets/Graphs/graphs_switch.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphs_switch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Graphs_Switch_t {
    const uint offsetsAndSize[24];
    char stringdata0[169];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Graphs_Switch_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Graphs_Switch_t qt_meta_stringdata_Graphs_Switch = {
    {
QT_MOC_LITERAL(0, 13), // "Graphs_Switch"
QT_MOC_LITERAL(14, 11), // "signal_show"
QT_MOC_LITERAL(26, 0), // ""
QT_MOC_LITERAL(27, 18), // "signal_approximate"
QT_MOC_LITERAL(46, 20), // "signal_deapproximate"
QT_MOC_LITERAL(67, 11), // "signal_save"
QT_MOC_LITERAL(79, 14), // "signal_zoom_in"
QT_MOC_LITERAL(94, 15), // "signal_zoom_out"
QT_MOC_LITERAL(110, 17), // "signal_zoom_reset"
QT_MOC_LITERAL(128, 18), // "slot_approximation"
QT_MOC_LITERAL(147, 9), // "slot_save"
QT_MOC_LITERAL(157, 11) // "slot_update"

    },
    "Graphs_Switch\0signal_show\0\0"
    "signal_approximate\0signal_deapproximate\0"
    "signal_save\0signal_zoom_in\0signal_zoom_out\0"
    "signal_zoom_reset\0slot_approximation\0"
    "slot_save\0slot_update"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Graphs_Switch[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x06,    1 /* Public */,
       3,    1,   77,    2, 0x06,    3 /* Public */,
       4,    1,   80,    2, 0x06,    5 /* Public */,
       5,    1,   83,    2, 0x06,    7 /* Public */,
       6,    0,   86,    2, 0x06,    9 /* Public */,
       7,    0,   87,    2, 0x06,   10 /* Public */,
       8,    0,   88,    2, 0x06,   11 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    0,   89,    2, 0x09,   12 /* Protected */,
      10,    0,   90,    2, 0x09,   13 /* Protected */,
      11,    1,   91,    2, 0x09,   14 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void Graphs_Switch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Graphs_Switch *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signal_show((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->signal_approximate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->signal_deapproximate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->signal_save((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->signal_zoom_in(); break;
        case 5: _t->signal_zoom_out(); break;
        case 6: _t->signal_zoom_reset(); break;
        case 7: _t->slot_approximation(); break;
        case 8: _t->slot_save(); break;
        case 9: _t->slot_update((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Graphs_Switch::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Graphs_Switch::signal_show)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Graphs_Switch::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Graphs_Switch::signal_approximate)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Graphs_Switch::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Graphs_Switch::signal_deapproximate)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Graphs_Switch::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Graphs_Switch::signal_save)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Graphs_Switch::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Graphs_Switch::signal_zoom_in)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Graphs_Switch::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Graphs_Switch::signal_zoom_out)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Graphs_Switch::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Graphs_Switch::signal_zoom_reset)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject Graphs_Switch::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Graphs_Switch.offsetsAndSize,
    qt_meta_data_Graphs_Switch,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Graphs_Switch_t
, QtPrivate::TypeAndForceComplete<Graphs_Switch, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>


>,
    nullptr
} };


const QMetaObject *Graphs_Switch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Graphs_Switch::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Graphs_Switch.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Graphs_Switch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Graphs_Switch::signal_show(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Graphs_Switch::signal_approximate(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Graphs_Switch::signal_deapproximate(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Graphs_Switch::signal_save(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Graphs_Switch::signal_zoom_in()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Graphs_Switch::signal_zoom_out()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void Graphs_Switch::signal_zoom_reset()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
