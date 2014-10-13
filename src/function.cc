
#include "function.h"
#include "value.h"

using namespace v8;

namespace GINode {

static Handle<Value> FunctionInvoker(const Arguments &args) {
    HandleScope scope;
    GIBaseInfo *info = (GIBaseInfo *) External::Unwrap (args.Data ());
    GIFunctionInfo *function_info = (GIFunctionInfo *) info;
    GError *error = NULL;

    /* XXX: For now, only work on functions without any OUT args at all.
     * Just assume everything is an in arg. */
    int n_in_args = g_callable_info_get_n_args ((GICallableInfo *) info);
    GIArgument in_args[n_in_args];

    if (args.Length() < n_in_args) {
        ThrowException (Exception::TypeError (String::New ("Not enough arguments.")));
        return scope.Close (Undefined ());
    }

    for (int i = 0; i < n_in_args; i++) {
        GIArgInfo *arg_info = g_callable_info_get_arg ((GICallableInfo *) info, i);
        GITypeInfo type_info;
        g_arg_info_load_type (arg_info, &type_info);
        V8ToGIArgument (&type_info, &in_args[i], args[i]);
        g_base_info_unref ((GIBaseInfo *) arg_info);
    }

    GIArgument return_value;
    g_function_info_invoke (function_info,
                            in_args, n_in_args,
                            NULL, 0,
                            &return_value,
                            &error);

    for (int i = 0; i < n_in_args; i++) {
        GIArgInfo *arg_info = g_callable_info_get_arg ((GICallableInfo *) info, i);
        GITypeInfo type_info;
        g_arg_info_load_type (arg_info, &type_info);
        FreeGIArgument (&type_info, &in_args[i]);
        g_base_info_unref ((GIBaseInfo *) arg_info);
    }

    if (error) {
        ThrowException (Exception::TypeError (String::New (error->message)));
        return scope.Close (Undefined ());
    }

    GITypeInfo return_value_type;
    g_callable_info_load_return_type ((GICallableInfo *) info, &return_value_type);
    return scope.Close (GIArgumentToV8 (&return_value_type, &return_value));
}

static void FunctionDestroyed (Persistent<Value> object, void *data) {
    GIBaseInfo *info = (GIBaseInfo *) data;
    g_base_info_unref (info);
}

Handle<Function> MakeFunction(GIBaseInfo *info) {
    HandleScope scope;

    Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New (FunctionTemplate::New (FunctionInvoker));
    tpl.MakeWeak (g_base_info_ref (info), FunctionDestroyed);

    tpl->SetCallHandler (FunctionInvoker, External::New (info));
    Local<Function> fn = tpl->GetFunction ();

    const char *function_name = g_base_info_get_name (info);
    fn->SetName (String::NewSymbol (function_name));

    return scope.Close (fn);
}

};