#include "stubs.h"

#include <QtCore/QDebug>
#include <QtQml/QQmlPropertyMap>

// converts QVariant to OCaml QVariant.t. Should be ported to the lablqml
/*
// I intentionally skipped CAMLlocalN because intuition says that they are
// not needed because it is done in caller function
*/
extern "C" value Val_QVariant(value _dest, const QVariant& var) {
    CAMLparam1(_dest);
    CAMLlocal1(_var);

    if (!var.isValid()) {
        _dest = hash_variant("empty");
    } else {
        int ut = var.userType();
        if(ut == QMetaType::QString) {
            _dest = caml_alloc(2,0);
            Store_field(_dest,0,hash_variant("string"));
            Store_field(_dest,1,caml_copy_string(var.value<QString>().toLocal8Bit().data()));
        } else if (ut == QMetaType::Int) {
            _dest = caml_alloc(2,0);
            Store_field(_dest, 0, hash_variant("int"));
            Store_field(_dest, 1, Val_int(var.value<int>()));
        } else if ((ut==QMetaType::User) ||  (ut==QMetaType::QObjectStar)) {
            QObject *vvv = var.value<QObject*>();
            _var = caml_alloc_small(1,Abstract_tag);
            (*((QObject **) &Field(_var, 0))) = vvv;
            _dest = caml_alloc(2,0);
            Store_field(_dest, 0, hash_variant("qobject"));
            Store_field(_dest, 1, _var);
        } else {
            QString msg("Type is not supported:");
            msg += QString("userType() == %1").arg(ut);
            Q_ASSERT_X(false, __func__, msg.toLocal8Bit().data() );
        }
    }
    CAMLreturn(_dest);
}

extern "C" value caml_create_QQmlPropertyMap(value _func, value _unit) {
    CAMLparam2(_func, _unit);
    CAMLlocal1(_ans);
    //caml_enter_blocking_section();
    caml_register_global_root(&_func);

    QQmlPropertyMap *propMap = new QQmlPropertyMap();
    _ans = caml_alloc_small(1, Abstract_tag);
    (*((QQmlPropertyMap **) &Field(_ans, 0))) = propMap;

    QObject::connect(propMap, &QQmlPropertyMap::valueChanged,
                     [=](const QString& propName, const QVariant& var) {
                         CAMLparam0();
                         CAMLlocal2(_nameArg,_variantArg);
                         caml_leave_blocking_section();
                         _nameArg = caml_copy_string( propName.toLocal8Bit().data() );
                         caml_callback2(_func, _nameArg, Val_QVariant(_variantArg, var) );
                         caml_enter_blocking_section();
                         CAMLreturn0;
                     } );

    //caml_leave_blocking_section();
    CAMLreturn(_ans);
}

extern "C" value caml_QQmlPropertyMap_insert(value _map, value _propName, value _variant) {
    CAMLparam3(_map, _propName, _variant);

    QQmlPropertyMap *map = (QQmlPropertyMap*) (Field(_map,0));
    Q_ASSERT_X(map != NULL, __func__, "Trying to use QQmlPropertyMap object which is NULL");

    // copy and paste from the generated file for QAbstractModel subclass
    // TODO: move this conversion to the lablqml
    QVariant newval;
    if (Is_block(_variant)) {
        if (caml_hash_variant("string") == Field(_variant,0) )
            newval = QVariant::fromValue(QString(String_val(Field(_variant,1))));
        else if (caml_hash_variant("int") == Field(_variant,0) )
            newval = QVariant::fromValue(Int_val(Field(_variant,1)));
        else if (caml_hash_variant("qobject") == Field(_variant,0) )
            newval = QVariant::fromValue((QObject*) (Field(Field(_variant,1),0)));
        else Q_ASSERT_X(false,"While converting OCaml value to QVariant","Unknown variant tag");
    } else { // empty QVariant
        newval = QVariant();
    }

    map->insert( QString(String_val(_propName)), newval);

    CAMLreturn(Val_unit);
}