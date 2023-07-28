#pragma once



struct binary_tree_node
{
	int key;
	void* value;
	struct binary_tree_node* parent;
	struct binary_tree_node* left;
	struct binary_tree_node* right;
	struct binary_tree_node* r_brother;
	struct binary_tree_node* l_brother;
	struct binary_tree_node* old_brother;
	struct binary_tree_node* next_line;
	struct binary_tree_node* last_line;
};


struct tree
{
	int num;
	struct binary_tree_node* head;
	struct binary_tree_node* tail;
};

struct kv
{
	int k;
	void* value;
};

/*API*/
struct tree* new_tree();
void clear_tree_nodes(struct tree* tree);
void delete_tree(struct tree* tree);
void delete_min_node(struct tree* tree, int* k, void** v);