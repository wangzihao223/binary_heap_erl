#include <stdlib.h>
#include <stdio.h>
#include <binary_tree.h>

/*internal*/
struct binary_tree_node* create_tree_node(int key, void* value);
void shift_up(struct tree* tree);
void sink(struct tree* tree);
void delete_head_node(struct tree* tree);
void insert_old_brother_left(struct binary_tree_node* tail, struct binary_tree_node* now);
void exchange(struct binary_tree_node* parent, struct binary_tree_node* child);
void check_parent_brother(struct binary_tree_node* tail, struct binary_tree_node* now);

#define now_greater_left  \
	if (index_node->key > index_node->left->key)\
	{\
		exchange(index_node, index_node->left);\
		index_node = index_node->left;\
	}\
	else\
	{\
		break;\
	}

#define now_greater_right \
	if (index_node->key > index_node->right->key)\
	{\
		exchange(index_node, index_node->right);\
		index_node = index_node->right;\
	}



struct tree* new_tree()
{
	struct tree* tree = (struct tree*)malloc(sizeof(struct tree));
	if (tree != NULL)
	{
		tree->num = 0;
		tree->head = NULL;
		tree->tail = NULL;
	}
	return tree;

}

void insert(int key, void* value, struct tree* tree)
{
	tree->num++;
	struct binary_tree_node* node = create_tree_node(key, value);
	if (tree->tail == NULL)
	{
		node->old_brother = node;
		tree->head = node;
		tree->tail = node;
		node->parent = NULL;
	}
	else
	{
		struct binary_tree_node* tail = tree->tail;
		if (tail->parent != NULL)
		{
			if (tail->parent->right == NULL)
			{
				tail->r_brother = node;
				tail->parent->right = node;
				node->parent = tail->parent;
				node->old_brother = tail->old_brother;
				node->l_brother = tail;
			}
			else
			{
				check_parent_brother(tail, node);
			}
		}
		else
		{
			tail->next_line = node;
			node->last_line = tail;
			tail->left = node;
			node->parent = tail;
			node->old_brother = node;
		}
		tree->tail = node;
		// 上浮
		shift_up(tree);
	}
}

void delete_min_node(struct tree* tree, int* k, void** v)
{
	if (tree != NULL)
	{
		if (tree->head != NULL)
		{
			exchange(tree->head, tree->tail);
			*k = tree->tail->key;
			*v = tree->tail->value;
			struct binary_tree_node* drop = tree->tail;
			if (tree->head == tree->tail)
			{
				tree->head = NULL;
				tree->tail = NULL;
			}
			else
			{
				struct binary_tree_node* p = tree->tail->parent;
				if (p->left == drop)
				{
					p->left = NULL;
					if (drop->l_brother != NULL)
					{
						tree->tail = drop->l_brother;
						drop->l_brother->r_brother = NULL;
					}
					else
					{
						// 上一层
						tree->tail = drop->last_line;
						drop->last_line->next_line = NULL;
					}
				}
				else
				{
					p->right = NULL;
					tree->tail = p->left;
					p->left->r_brother = NULL;
				}
			}
			//下沉
			sink(tree);
			tree->num--;
			free(drop);

		}
		else
		{
			*k = NULL;
			*v = NULL;
		}
	}
	else
	{
		*k = NULL;
		*v = NULL;
	}
}

void top(struct tree* tree, int* k, void** v)
{
	if (tree->head != NULL)
	{
		*k = tree->head->key;
		*v = tree->head->value;
	}
	else
	{
		*k = NULL;
		*v = NULL;
	}
}

void clear_tree_nodes(struct tree* tree)
{
	while (tree->num != 0)
	{
		delete_head_node(tree);
	}

}

void delete_tree(struct tree* tree)
{
	clear_tree_nodes(tree);
	free(tree);
}

void delete_head_node(struct tree* tree)
{
	struct binary_tree_node* head = tree->head;
	if (head->r_brother != NULL)
	{
		tree->head = head->r_brother;
		
	}
	else
	{
		tree->head = head->next_line;
	}
	if (head == tree->tail)
	{
		tree->tail = NULL;
	}
	free(head);
	tree->num--;
	
}


void sink(struct tree* tree)
{
	struct binary_tree_node* index_node = tree->head;
	while (index_node != NULL)
	{
		if (index_node->left != NULL && index_node->right != NULL)
		{
			if (index_node->left->key < index_node->right->key)
			{
				now_greater_left
			}
			else
			{
				now_greater_right
				else
				{
					break;
				}
			}
		}
		else if (index_node->left != NULL)
		{
			now_greater_left
		}
		else if (index_node->right != NULL)
		{
			now_greater_right
		}
		else
		{
			break;
		}
	}
}


void check_parent_brother(struct binary_tree_node* tail, struct binary_tree_node* now)
{
	struct binary_tree_node* parent = tail->parent;
	if (parent->r_brother == NULL)
	{
		tail->next_line = now;
		now->last_line = tail;
		//去下一层插入
		insert_old_brother_left(tail, now);
	}
	else
	{
		parent->r_brother->left = now;
		now->parent = parent->r_brother;
		now->old_brother = tail->old_brother;
		tail->r_brother = now;
		now->l_brother = tail;
	}
}

void insert_old_brother_left(struct binary_tree_node* tail, struct binary_tree_node* now)
{
	struct binary_tree_node* old_brother = tail->old_brother;
	old_brother->left = now;
	now->parent = old_brother;
	now->old_brother = now;
}

struct binary_tree_node* create_tree_node(int key, void* value)
{
	struct binary_tree_node* node = (struct binary_tree_node*)malloc(sizeof(struct binary_tree_node));
	if (node != NULL)
	{
		node->key = key;
		node->value = value;
		node->left = NULL;
		node->right = NULL;
		node->parent = NULL;
		node->r_brother = NULL;
		node->old_brother = NULL;
		node->l_brother = NULL;
		node->next_line = NULL;
		node->last_line = NULL;
	}
	return node;

}

void shift_up(struct tree* tree)
{
	struct binary_tree_node* now_node = tree->tail;
	while (now_node->parent != NULL)
	{
		struct binary_tree_node* parent = now_node->parent;
		if (parent->key > now_node->key)
		{
			// 交换节点
			exchange(parent, now_node);
		}
		else
		{
			break;
		}
		now_node = now_node->parent;
	}
}

void exchange(struct binary_tree_node* parent, struct binary_tree_node* child)
{
	int tmp_key = parent->key;
	parent->key = child->key;
	child->key = tmp_key;
	void* tmp_v = parent->value;
	parent->value = child->value;
	child->value = tmp_v;
}


int main()
{
	struct tree* tree = new_tree();
	if (tree != NULL)
	{
		for (int i = 100000; i >0; i--)
		{
			insert(i, NULL, tree);
		}
		printf("ok");
		//insert(1, NULL, tree);
		//insert(10, NULL, tree);
		//insert(-1, NULL, tree);
		//insert(-9, NULL, tree);
		//insert(2, NULL, tree);
		//insert(1, NULL, tree);
		//insert(11, NULL, tree);
		//insert(12, NULL, tree);
		//insert(-9, NULL, tree);
		//insert(20, NULL, tree);

		/*printf("min key is v %d \n", v);*/
		//int k;
		//void* v = NULL;
		//delete_min_node(tree, &k, &v);
		//printf("min key is v %d \n", k);
		//delete_min_node(tree, &k, &v);
		//printf("min key is v %d \n", k);
		//delete_min_node(tree, &k, &v);
		//printf("min key is v %d \n", k);
		//delete_min_node(tree, &k, &v);
		//printf("min key is v %d \n", k);
		//delete_min_node(tree, &k, &v);
		//printf("min key is v %d \n", k);
		//delete_min_node(tree, &k, &v);
		//printf("min key is v %d \n", k);
		//delete_min_node(tree, &k, &v);
		//printf("min key is v %d \n", k);
		//delete_min_node(tree, &k, &v);
		//printf("min key is v %d \n", k);
		//delete_min_node(tree, &k, &v);
		//printf("min key is v %d \n", k);
		//delete_min_node(tree, &k, &v);
		//printf("min key is v %d \n", k);

		//insert(1, NULL, tree);
		//delete_min_node(tree, &k, &v);
		//printf("min key is v %d \n", k);
		//insert(10, NULL, tree);
		//insert(-1, NULL, tree);
		//insert(-9, NULL, tree);
		//insert(2, NULL, tree);
		//delete_min_node(tree, &k, &v);
		//printf("min key is v %d \n", k);
		//insert(1, NULL, tree);
		//insert(11, NULL, tree);
		//delete_min_node(tree, &k, &v);
		//printf("min key is v %d \n", k);
		//insert(12, NULL, tree);
		//insert(-9, NULL, tree);
		//insert(20, NULL, tree);
		///*clear all nodes */
		//clear_tree_nodes(tree);
		//printf("clear all tree_nodes ! \n");

		//delete_tree(tree);
		//tree = NULL;
		//printf("delete the tree");
	}
}
