#include "Tree.h"
class Tree
{
public:
	Tree();
	~Tree();

	struct node
	{
		int distanceFromStart;
		struct node* adjacentleft;
		struct node* adjacentright;
		struct node* adjacentup;
		struct node* adjacentdown;
		bool visited = false;
	};

	node* rootNode;
};

Tree::Tree()
{

}

Tree::~Tree()
{

}
