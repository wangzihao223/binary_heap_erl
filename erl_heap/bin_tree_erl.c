#include <erl_nif.h>
#include <binary_tree.h>


static ERL_NIF_TERM new_tree_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifResourceTypeInit erl_resource
    struct tree* tree = new_tree();
    enif_alloc_resource()

}

static ERL_NIF_TERM bar_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    int y, ret;
    if (!enif_get_int(env, argv[0], &y)) {
        return enif_make_badarg(env);
    }
    ret = bar(y);
    return enif_make_int(env, ret);
}

static ErlNifFunc nif_funcs[] = {
    {"foo", 1, foo_nif},
    {"bar", 1, bar_nif}
};

ERL_NIF_INIT(complex6, nif_funcs, NULL, NULL, NULL, NULL)