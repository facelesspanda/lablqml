#include "QSingleFunc.h"
#include <caml/memory.h>
#include <caml/threads.h>
#include <caml/alloc.h>

extern "C"
value caml_create_qsinglefunc(value _cb)
{
  CAMLparam1(_cb);
  CAMLlocal1(_ans);
  caml_enter_blocking_section();

  _ans = caml_alloc_small(1, Abstract_tag);
  (*((QSingleFunc **) &Field(_ans, 0))) = new QSingleFunc(_cb);

  caml_leave_blocking_section();
  CAMLreturn(_ans);
}
//-----------------------------------------------------------
QSingleFunc::QSingleFunc(value v) : _caml_callback(v)
{
   caml_register_global_root(&_caml_callback);
}
void QSingleFunc::run()
{
      // call callback there
}
