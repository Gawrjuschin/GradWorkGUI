/****************************************************************************
** Meta object code from reading C++ file 'backend_object.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/Backend/backend_object.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'backend_object.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Backend_Object_t {
    const uint offsetsAndSize[22];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Backend_Object_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Backend_Object_t qt_meta_stringdata_Backend_Object = {
    {
QT_MOC_LITERAL(0, 14), // "Backend_Object"
QT_MOC_LITERAL(15, 12), // "signal_start"
QT_MOC_LITERAL(28, 0), // ""
QT_MOC_LITERAL(29, 17), // "signal_table_fill"
QT_MOC_LITERAL(47, 11), // "signal_done"
QT_MOC_LITERAL(59, 15), // "signal_progress"
QT_MOC_LITERAL(75, 10), // "slot_start"
QT_MOC_LITERAL(86, 10), // "slot_pause"
QT_MOC_LITERAL(97, 11), // "slot_resume"
QT_MOC_LITERAL(109, 9), // "slot_stop"
QT_MOC_LITERAL(119, 13) // "slot_generate"

    },
    "Backend_Object\0signal_start\0\0"
    "signal_table_fill\0signal_done\0"
    "signal_progress\0slot_start\0slot_pause\0"
    "slot_resume\0slot_stop\0slot_generate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Backend_Object[] = {

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
       7,    0,   73,    2, 0x0a,    6 /* Public */,
       8,    0,   74,    2, 0x0a,    7 /* Public */,
       9,    0,   75,    2, 0x0a,    8 /* Public */,
      10,    0,   76,    2, 0x0a,    9 /* Public */,

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

void Backend_Object::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Backend_Object *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signal_start(); break;
        case 1: _t->signal_table_fill(); break;
        case 2: _t->signal_done(); break;
        case 3: _t->signal_progress(); break;
        case 4: _t->slot_start(); break;
        case 5: _t->slot_pause(); break;
        case 6: _t->slot_resume(); break;
        case 7: _t->slot_stop(); break;
        case 8: _t->slot_generate(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Backend_Object::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Backend_Object::signal_start)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Backend_Object::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Backend_Object::signal_table_fill)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Backend_Object::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Backend_Object::signal_done)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Backend_Object::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Backend_Object::signal_progress)) {
                *result = 3;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject Backend_Object::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Backend_Object.offsetsAndSize,
    qt_meta_data_Backend_Object,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Backend_Object_t
, QtPrivate::TypeAndForceComplete<Backend_Object, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Backend_Object::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Backend_Object::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Backend_Object.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Backend_Object::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void Backend_Object::signal_start()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Backend_Object::signal_table_fill()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Backend_Object::signal_done()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Backend_Object::signal_progress()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
