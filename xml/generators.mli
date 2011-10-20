open Parser

val skipClass : prefix:string list -> clas -> bool
val skipArgument : index:SuperIndex.index_t -> func_arg -> bool
exception DoSkip
exception DontSkip
exception BreakSilent
exception BreakS of string
val breaks : string -> 'a

type t1 = string
and t2 = string
and castResult =
    Success of t1
  | CastError of t2
  | CastValueType of t2
  | CastTemplate of t2
exception BreakOk of t1
exception BreakFail of t2
exception BreakResult of castResult
type pattern =
  | InvalidPattern
  | PrimitivePattern
  | ObjectPattern
  | EnumPattern  of enum * SuperIndex.NameKey.t
  | ObjectDefaultPattern

val cpp_stub_name: classname:string -> ?res_n_name:cpptype*string -> ?is_byte:bool -> func_arg list 
  -> string
val is_good_meth : classname:string -> index:SuperIndex.index_t -> meth -> bool

val enum_conv_func_names : (string list * string) -> string*string
val pattern : SuperIndex.index_t -> Parser.func_arg -> pattern
val is_abstract_class : prefix:string list -> SuperIndex.index_t -> string -> bool

class virtual abstractGenerator :
  SuperIndex.index_t ->
  object
    method private toCamlCast :
      ?forcePattern: pattern option -> Parser.func_arg -> string -> string -> castResult
    method private fromCamlCast :
      SuperIndex.index_t ->
      Parser.cpptype -> default:string option -> ?cpp_argname:string option  -> string -> castResult
    method private virtual gen_class : prefix:string list -> dir:string -> Parser.clas -> string option
    method private virtual gen_enum_in_ns :
      key:SuperIndex.NameKey.t -> dir:string -> Parser.enum -> string option
    method private index : SuperIndex.index_t
    method private virtual prefix : string
  end

