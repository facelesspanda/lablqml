type t
val get_view_exn : name:string -> t
val get_view : name:string -> t option

type cppobj
val set_context_property: ctx:t -> name:string -> cppobj -> unit

module QVariant : sig
  type t = [ `empty | `string of string | `qobject of cppobj | `int of int | `bool of bool
           | `float of float ]
  val empty: t
  val of_string: string -> t
  val of_object: cppobj -> t
  val of_int   : int -> t
  val of_bool  : bool -> t
  val of_float : float -> t
end

class test_object : t -> object
  method handler : t
  method property: string -> QVariant.t
end

module QModelIndex : sig
  type t
  val empty: t
  val row: t -> int
  val column: t -> int
  val make: row:int -> column:int -> t
  val to_string: t -> string
end

module QGuiApplication : sig
  type t
  val exec : t -> unit
end
module QQmlEngine : sig
  type t
  val register_context: name:string -> t -> unit
  val add_import_path: string -> t -> unit
end

module QQuickWindow : sig
  type t
  val showMaximized : t -> unit
  val as_test_object: t -> test_object
end

(** Creates QGuiApplication. No platform-dependent styling applied. *)
val create_qapplication : string array -> QGuiApplication.t * QQmlEngine.t

(** Creates QQuickWindow using file and QQmlEngine *)
val loadQml : string -> QQmlEngine.t -> QQuickWindow.t option

(** Initializates and open QQuickWindow. Uses platform dependent styling *)
val run_with_QQmlApplicationEngine : string array -> (unit -> unit) -> string -> unit

type qvariantable
type non_qvariantable
class virtual ['valtyp] prop :
  string ->
  object
    method virtual get : 'valtyp
    method name : string
    method virtual set : 'valtyp -> unit
  end
class virtual ['valtyp] qvariant_prop :
  string ->
  object
    method virtual get : 'valtyp
    method name : string
    method virtual set : 'valtyp -> unit
    method virtual wrap_in_qvariant : 'valtyp -> QVariant.t
  end

module PropMap: sig
  type t
  val handler: t -> cppobj

  val create: ?callback:(string -> QVariant.t -> unit) -> unit -> t
  val insert: t -> name:string -> QVariant.t -> unit
  val value_: t -> string -> QVariant.t
end

module SingleFunc: sig
  type t
  val handler: t -> cppobj
  val create: (unit -> unit) -> t
end
