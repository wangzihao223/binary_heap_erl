#include <erl_nif.h>
#include <stdio.h>
#include <binary_tree.h>


struct meta
{
    ErlNifResourceType* resource_type;
    struct tree* tree;
};

typedef struct meta meta_data;

ErlNifResourceType* get_resource_type(ErlNifEnv* env)
{
    meta_data* meta = enif_priv_data(env);
    ErlNifResourceType* type = meta->resource_type;
    return type;
}

void free_binary_heap(ErlNifEnv* caller_env, void* obj)
{
    clear_tree_nodes((struct tree*)obj);
}

int load(ErlNifEnv* caller_env, void** priv_data, ERL_NIF_TERM load_info)
{
    ErlNifResourceType* resource_type = enif_open_resource_type(caller_env, NULL, "binary_heap_tree", free_binary_heap, 1, NULL);
    meta_data* meta = (meta_data*)malloc(sizeof(meta_data));
    *priv_data = meta;
    if (meta != NULL)
    {
        meta->resource_type = resource_type;
        return 0;
    }
    else
    {
        return 1;
    }
}

static ERL_NIF_TERM new_tree_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    int flag = 0;
    ErlNifResourceType* type = get_resource_type(env);
    struct tree* tree = enif_alloc_resource(type, sizeof(struct tree));
    if (tree != NULL)
    {
        tree->num = 0;
        tree->head = NULL;
        tree->tail = NULL;
    }
    ERL_NIF_TERM nif_resource = enif_make_resource(env, tree);
    return nif_resource;
}

static ERL_NIF_TERM insert_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    int key, arity;
    ERL_NIF_TERM* value = NULL;
    struct tree* tree = NULL;
    ErlNifResourceType* type = get_resource_type(env);
    if (!enif_get_int(env, argv[0], &key) || !enif_get_tuple(env, argv[1], &arity, &value) || !enif_get_resource(env, argv[2], type, &tree))
    {
        return enif_make_badarg(env);
    }
    if (tree != NULL)
    {
        insert(key, value[1], tree);
        ERL_NIF_TERM arr[2] = { enif_make_atom(env, "ok"), argv[2] };
        ERL_NIF_TERM res = enif_make_tuple_from_array(env, arr, 2);
        return res;
    }
    return enif_make_badarg(env);
}

static ERL_NIF_TERM delete_min_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    struct tree* tree = NULL;
    ErlNifResourceType* type = get_resource_type(env);
    if(!enif_get_resource(env, argv[0], type, &tree))
    {
        return enif_make_badarg(env);
    }
    int k = 0;
    void* value = NULL;
    if (tree != NULL)
    {
        if (tree->num == 0)
        {
            ERL_NIF_TERM arr[2] = { enif_make_atom(env, "empty_heap"), argv[0] };
            return enif_make_tuple_from_array(env, arr, 2);
        }
        delete_min_node(tree, &k, &value);;
        ERL_NIF_TERM k_v[2] = { enif_make_int(env, k), value };
        ERL_NIF_TERM k_v1 = enif_make_tuple_from_array(env, k_v, 2);
        ERL_NIF_TERM arr1[2] = { k_v1, argv[0]};
        ERL_NIF_TERM res = enif_make_tuple_from_array(env, arr1, 2);
        return res;
    }
    return enif_make_badarg(env);
}

static ERL_NIF_TERM delete_tree_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    struct tree* tree = NULL;
    ErlNifResourceType* type = get_resource_type(env);
    if (!enif_get_resource(env, argv[0], type, &tree))
    {
        return enif_make_badarg(env);
    }
    if (tree != NULL)
    {
        enif_release_resource(tree);
        return enif_make_atom(env, "ok");
    }
    return enif_make_badarg(env);
}

static ERL_NIF_TERM item_count_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    struct tree* tree = NULL;
    ErlNifResourceType* type = get_resource_type(env);
    if (!enif_get_resource(env, argv[0], type, &tree))
    {
        return enif_make_badarg(env);
    }
    if (tree != NULL)
    {
        int count = tree->num;
        return enif_make_int(env, count);
    }
    return enif_make_badarg(env);
}

static ERL_NIF_TERM top_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    struct tree* tree = NULL;
    ErlNifResourceType* type = get_resource_type(env);
    if (!enif_get_resource(env, argv[0], type, &tree))
    {
        return enif_make_badarg(env);
    }
    if (tree != NULL)
    {
        int k = 0;
        void* value = NULL;
        top(tree, &k, &value);
        if (tree->num == 0)
        {
            return enif_make_atom(env, "null");
        }
        else
        {
            ERL_NIF_TERM k_v[2] = { enif_make_int(env, k), value };
            ERL_NIF_TERM k_v1 = enif_make_tuple_from_array(env, k_v, 2);
            return k_v1;
        }

    }
    return enif_make_badarg(env);
}

static ErlNifFunc nif_funcs[] = {
    {"new_tree", 0, new_tree_nif},
    {"insert", 3, insert_nif},
    {"delete_min", 1, delete_min_nif},
    {"delete_tree", 1, delete_tree_nif},
    {"top", 1, top_nif},
    {"item_count", 1, item_count_nif}
};

ERL_NIF_INIT(binary_heap, nif_funcs, load, NULL, NULL, NULL)