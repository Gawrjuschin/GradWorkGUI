/****************************************************************************
** Meta object code from reading C++ file 'loading_widget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/Widgets/loading_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loading_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Loading_Widget_t {
    const uint offsetsAndSize[20];
    char stringdata0[91];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Loading_Widget_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Loading_Widget_t qt_meta_stringdata_Loading_Widget = {
    {
QT_MOC_LITERAL(0, 14), // "Loading_Widget"
QT_MOC_LITERAL(15, 13), // "pixmapChanged"
QT_MOC_LITERAL(29, 0), // ""
QT_MOC_LITERAL(30, 9), // "slot_stop"
QT_MOC_LITERAL(40, 10), // "slot_start"
QT_MOC_LITERAL(51, 9), // "setPixmap"
QT_MOC_LITERAL(61, 3), // "pic"
QT_MOC_LITERAL(65, 9), // "next_text"
QT_MOC_LITERAL(75, 8), // "next_pic"
QT_MOC_LITERAL(84, 6) // "pixmap"

    },
    "Loading_Widget\0pixmapChanged\0\0slot_stop\0"
    "slot_start\0setPixmap\0pic\0next_text\0"
    "next_pic\0pixmap"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Loading_Widget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       1,   58, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   51,    2, 0x0a,    3 /* Public */,
       4,    0,   52,    2, 0x0a,    4 /* Public */,
       5,    1,   53,    2, 0x0a,    5 /* Public */,
       7,    0,   56,    2, 0x09,    7 /* Protected */,
       8,    0,   57,    2, 0x09,    8 /* Protected */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPixmap,    6,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       9, QMetaType::QPixmap, 0x00015103, uint(0), 0,

       0        // eod
};

void Loading_Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Loading_Widget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->pixmapChanged(); break;
        case 1: _t->slot_stop(); break;
        case 2: _t->slot_start(); break;
        case 3: _t->setPixmap((*reinterpret_cast< const QPixmap(*)>(_a[1]))); break;
        case 4: _t->next_text(); break;
        case 5: _t->next_pic(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Loading_Widget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Loading_Widget::pixmapChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<Loading_Widget *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QPixmap*>(_v) = _t->pixmap(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<Loading_Widget *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPixmap(*reinterpret_cast< QPixmap*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Loading_Widget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Loading_Widget.offsetsAndSize,
    qt_meta_data_Loading_Widget,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Loading_Widget_t
, QtPrivate::TypeAndForceComplete<QPixmap, std::true_type>, QtPrivate::TypeAndForceComplete<Loading_Widget, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QPixmap &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Loading_Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Loading_Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Loading_Widget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Loading_Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Loading_Widget::pixmapChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
