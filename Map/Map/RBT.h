#pragma once
#include<iostream>
#include<utility>
#include<vector>  //used for constructing the tree from an initialization vector

template <typename KeyType, typename ValueType, typename Compare = std::less<KeyType>>
class RBT
{
public:
	struct TreeNode
	{
		std::pair<KeyType, ValueType> KeyValuePair;
		bool isRed;
		TreeNode* leftNode;
		TreeNode* rightNode;
		TreeNode* parentNode;

		TreeNode(const std::pair<KeyType, ValueType>& KeyValue = std::make_pair(KeyType(), ValueType()), bool red = true, TreeNode* left = nullptr, TreeNode* right = nullptr, TreeNode* parent = nullptr)
			: KeyValuePair(KeyValue), isRed(red), leftNode(left), rightNode(right), parentNode(parent) {}
	};

private:

	TreeNode* root;
	TreeNode* sentinelNode;

	Compare comparator;  // comparator(a,b) returns true if a is "before" b based on the specified criteria
	size_t count;

	void RotateLeft(TreeNode* node);

	void RotateRight(TreeNode* node);

	void InsertRepair(TreeNode* node);

	void EraseRepair(TreeNode* node);

	void TransplantRBT(TreeNode* node1, TreeNode* node2);

	void clear(TreeNode* node);

	void displayPreOrder(TreeNode* node);

	void displayInOrder(TreeNode* node);

	void displayPostOrder(TreeNode* node);

	void displayKeyValue(TreeNode* node);

	void erase(TreeNode* nodeDeleted);

	TreeNode* CopySubtree(TreeNode* node, TreeNode* otherSentinelNode, TreeNode* parent = nullptr, bool isRoot = false);

public:

	RBT(const Compare& comparator = Compare());

	RBT(const RBT& other, const Compare& comparator = Compare());

	RBT(const std::vector<std::pair<KeyType, ValueType>>& vector, const Compare& comparator = Compare());

	~RBT();

	void insert(const std::pair<KeyType, ValueType>& KeyValue);

	TreeNode* max(TreeNode* node);

	TreeNode* min(TreeNode* node);

	TreeNode* successor(TreeNode* node);

	TreeNode* predecessor(TreeNode* node);

	TreeNode* find(const KeyType& keySearch);

	void deleteKey(const KeyType& keyDeleted);

	ValueType& operator[](const KeyType& key);

	RBT& operator=(const RBT& other);

	int size();

	void clear();

	bool empty();

	void displayPreOrder();

	void displayInOrder();

	void displayPostOrder();

	void displayKeyValue();

	TreeNode* getSentinelNode();

	TreeNode* getRoot();
};

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::RotateLeft(TreeNode* node)
{
	TreeNode* rightChild = node->rightNode;

	node->rightNode = rightChild->leftNode;

	if (rightChild->leftNode != sentinelNode)
		rightChild->leftNode->parentNode = node;

	rightChild->parentNode = node->parentNode;

	if (node->parentNode == sentinelNode)
	{
		root = rightChild;
		root->parentNode = sentinelNode;
	}
	else
	{
		if (node == node->parentNode->leftNode)  //if the node is on the left of the parent
			node->parentNode->leftNode = rightChild;  //the right child of the node takes the node's place
		else                                    //if the node is on the right of the parent
			node->parentNode->rightNode = rightChild; //the right child of the node takes the node's place
	}
	rightChild->leftNode = node;
	node->parentNode = rightChild;
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::RotateRight(TreeNode* node)
{
	TreeNode* leftChild = node->leftNode;

	node->leftNode = leftChild->rightNode;

	if (leftChild->rightNode != sentinelNode)
		leftChild->rightNode->parentNode = node;

	leftChild->parentNode = node->parentNode;

	if (node->parentNode == sentinelNode)
	{
		root = leftChild;
		root->parentNode = sentinelNode;
	}
	else
	{
		if (node == node->parentNode->rightNode)
			node->parentNode->rightNode = leftChild;
		else
			node->parentNode->leftNode = leftChild;
	}
	leftChild->rightNode = node;
	node->parentNode = leftChild;
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::InsertRepair(TreeNode* node) //the function takes the node that was inserted as a parameter and if it is needed rebalances the tree
{
	if (node == root)
	{
		node->isRed = false;
		return;
	}
	if (node->parentNode == sentinelNode || node->parentNode->parentNode == sentinelNode)
	{
		node->parentNode->isRed = false; // the parent becomes black if the grandparent is sentinelNode because it means the parent is the root
		return;
	}

	if (node->parentNode->isRed != true) // if the parent is black,the tree is still balanced
		return;

	TreeNode* uncle;
	TreeNode* parent = node->parentNode;
	TreeNode* grandparent = parent->parentNode;
	if (grandparent->leftNode == parent) //if the parent is on the left of the grandparent,the uncle is on the right of the grandparent
		uncle = grandparent->rightNode;
	else  //if the parent is on the right of the grandparent,the uncle is on the left of the grandparent
		uncle = grandparent->leftNode;

	if (node->isRed && parent->isRed && uncle->isRed)  //if the parent and the uncle are red,then the parent,the grandparent and the uncle are recolored and the process starts again at the grandparent
	{
		parent->isRed = false; //make the parent black
		uncle->isRed = false;  //make the uncle black
		grandparent->isRed = true; //make the grandparent red and recheck if the tree is balanced
		InsertRepair(grandparent);
	}
	else if (node->isRed && parent->isRed && !uncle->isRed)  //if the parent is red and the uncle is black
	{
		if (grandparent->rightNode == parent && parent->leftNode == node)
		{
			RotateRight(parent);
			InsertRepair(parent);
		}
		else if (grandparent->leftNode == parent && parent->rightNode == node)
		{
			RotateLeft(parent);
			InsertRepair(parent);
		}

		if (grandparent->rightNode == parent && parent->rightNode == node)
		{
			parent->isRed = false;
			grandparent->isRed = true;
			RotateLeft(grandparent);
			InsertRepair(parent);
		}
		else if (grandparent->leftNode == parent && parent->leftNode == node)
		{
			parent->isRed = false;
			grandparent->isRed = true;
			RotateRight(grandparent);
			InsertRepair(parent);
		}
	}
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::EraseRepair(TreeNode* node)  //fixes the problems that can appear after erasing a node
{
	while (node != root && !node->isRed)
	{
		if (node == node->parentNode->leftNode)  //if the node is on the left of the parent
		{
			TreeNode* sibling = node->parentNode->rightNode;
			if (sibling->isRed)  //case 1: the sibling is red
			{
				sibling->isRed = false;
				node->parentNode->isRed = true;
				RotateLeft(node->parentNode);
				sibling = node->parentNode->rightNode;
			}
			else if (!sibling->leftNode->isRed && !sibling->rightNode->isRed) //case 2: if the sibling and the sibling's children are all black
			{
				sibling->isRed = true;
				node = node->parentNode;
			}
			else
			{
				if (!sibling->rightNode->isRed)//case 3: if the sibling is black and the left child of the sibling is red and the right child of the sibling is black
				{
					sibling->leftNode->isRed = false;
					sibling->isRed = true;
					RotateRight(sibling);
				}
				else  //case 4: if the sibling is black and the right child of the sibling is red
				{
					sibling->isRed = node->parentNode->isRed;
					node->parentNode->isRed = false;
					sibling->rightNode->isRed = false;
					RotateLeft(node->parentNode);
					node = root;
				}
			}
		}
		else  //if the node is on the right of the parent,it is the same algorithm but simetrically,so right becomes left and left becomes right
		{
			TreeNode* sibling = node->parentNode->leftNode;
			if (sibling->isRed)  //case 1: the sibling is red
			{
				sibling->isRed = false;
				node->parentNode->isRed = true;
				RotateRight(node->parentNode);
				sibling = node->parentNode->leftNode;
			}
			else if (!sibling->rightNode->isRed && !sibling->leftNode->isRed) //case 2
			{
				sibling->isRed = true;
				node = node->parentNode;
			}
			else
			{
				if (!sibling->leftNode->isRed)//case 3
				{
					sibling->rightNode->isRed = false;
					sibling->isRed = true;
					RotateLeft(sibling);
				}
				else  //case 4
				{
					sibling->isRed = node->parentNode->isRed;
					node->parentNode->isRed = false;
					sibling->leftNode->isRed = false;
					RotateRight(node->parentNode);
					node = root;
				}
			}
		}
	}
	node->isRed = false; //case 0
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::TransplantRBT(TreeNode* node1, TreeNode* node2)  //replaces node1 with node2
{
	if (node1->parentNode == sentinelNode)
		root = node2;
	else
	{
		if (node1 == node1->parentNode->leftNode)
			node1->parentNode->leftNode = node2;
		else
			node1->parentNode->rightNode = node2;
	}
	node2->parentNode = node1->parentNode;
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::clear(TreeNode* node)  //clears the tree using postOrder traversal,which is the most efficient way
{
	if (node == nullptr || node == sentinelNode)
		return;

	clear(node->leftNode);
	clear(node->rightNode);

	delete node;
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayPreOrder(TreeNode* node)
{
	if (node != sentinelNode)
	{
		std::cout << "(" << node->KeyValuePair.first << "," << node->KeyValuePair.second << ")->";
		if (node->isRed)
			std::cout << "red ";
		else
			std::cout << "black ";
		displayPreOrder(node->leftNode);
		displayPreOrder(node->rightNode);
	}
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayInOrder(TreeNode* node)
{
	if (node != sentinelNode)
	{
		displayInOrder(node->leftNode);
		std::cout << "(" << node->KeyValuePair.first << "," << node->KeyValuePair.second << ")->";
		if (node->isRed)
			std::cout << "red ";
		else
			std::cout << "black ";
		displayInOrder(node->rightNode);
	}
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayPostOrder(TreeNode* node)
{
	if (node != sentinelNode)
	{
		displayPostOrder(node->leftNode);
		displayPostOrder(node->rightNode);
		std::cout << "(" << node->KeyValuePair.first << "," << node->KeyValuePair.second << ")->";
		if (node->isRed)
			std::cout << "red ";
		else
			std::cout << "black ";
	}
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayKeyValue(TreeNode* node)  //in order traversal where only the KeyValuePair is displayed
{
	if (node != sentinelNode)
	{
		displayKeyValue(node->leftNode);
		std::cout << "(" << node->KeyValuePair.first << "," << node->KeyValuePair.second << ")  ";
		displayKeyValue(node->rightNode);
	}
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::erase(TreeNode* nodeDeleted)
{
	if (nodeDeleted == sentinelNode)
		return;

	bool isRed = nodeDeleted->isRed;
	TreeNode* node;

	if (nodeDeleted->leftNode == sentinelNode)
	{
		node = nodeDeleted->rightNode;
		TransplantRBT(nodeDeleted, node);
	}
	else
	{
		if (nodeDeleted->rightNode == sentinelNode)
		{
			node = nodeDeleted->leftNode;
			TransplantRBT(nodeDeleted, node);
		}
		else
		{
			TreeNode* successorNode = successor(nodeDeleted);
			isRed = successorNode->isRed;
			node = successorNode->rightNode;
			if (successorNode->parentNode == nodeDeleted)
				node->parentNode = successorNode;
			else
			{
				TransplantRBT(successorNode, node);
				successorNode->rightNode = nodeDeleted->rightNode;
				nodeDeleted->rightNode->parentNode = successorNode;
			}
			TransplantRBT(nodeDeleted, successorNode);
			successorNode->leftNode = nodeDeleted->leftNode;
			nodeDeleted->leftNode->parentNode = successorNode;
			successorNode->isRed = nodeDeleted->isRed;
		}
	}
	if (!isRed)
		EraseRepair(node);
}

template <typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::TreeNode* RBT<KeyType, ValueType, Compare>::CopySubtree(TreeNode* node, TreeNode* otherSentinelNode, TreeNode* parent, bool isRoot)
{  //this is used for the operator=.Instead of inserting each element with the function insert,it copies each node and goes down the left and right subtrees recursively
	if (node == otherSentinelNode && !isRoot)
		return sentinelNode;

	TreeNode* newNode = new TreeNode(node->KeyValuePair, node->isRed);
	newNode->parentNode = parent;
	if (isRoot)
	{
		root = newNode;
		newNode->parentNode = sentinelNode;
	}
	newNode->leftNode = CopySubtree(node->leftNode, otherSentinelNode, newNode);
	newNode->rightNode = CopySubtree(node->rightNode, otherSentinelNode, newNode);

	return newNode;
}

template <typename KeyType, typename ValueType, typename Compare>
RBT<KeyType, ValueType, Compare>::RBT(const Compare& comparator)
{
	count = 0;
	sentinelNode = new TreeNode(std::make_pair(KeyType(), ValueType()), false, nullptr, nullptr, nullptr);
	root = sentinelNode;
	this->comparator = comparator;
}

template <typename KeyType, typename ValueType, typename Compare>
RBT<KeyType, ValueType, Compare>::RBT(const RBT& other, const Compare& comparator)
{
	sentinelNode = new TreeNode(std::make_pair(KeyType(), ValueType()), false, nullptr, nullptr, nullptr);
	root = sentinelNode;
	this->comparator = comparator;

	*this = other;
}

template <typename KeyType, typename ValueType, typename Compare>
RBT<KeyType, ValueType, Compare>::RBT(const std::vector<std::pair<KeyType, ValueType>>& vector, const Compare& comparator)  //constructor using a vector
{
	clear(root);
	delete sentinelNode;

	count = 0;
	sentinelNode = new TreeNode(std::make_pair(KeyType(), ValueType()), false, nullptr, nullptr, nullptr);
	root = sentinelNode;
	this->comparator = comparator;

	for (const std::pair<KeyType, ValueType>& pair : vector)
		insert(pair);
}

template <typename KeyType, typename ValueType, typename Compare>
RBT<KeyType, ValueType, Compare>::~RBT()
{
	count = 0;
	clear();
	delete sentinelNode;
}

template <typename KeyType, typename ValueType, typename Compare>
void  RBT<KeyType, ValueType, Compare>::insert(const std::pair<KeyType, ValueType>& KeyValue)
{
	if (root == sentinelNode)  //if the tree is empty,the keyvalue pair inserted becomes the root
	{
		TreeNode* node = new TreeNode();
		node->KeyValuePair = KeyValue;
		node->isRed = false;
		node->rightNode = sentinelNode;
		node->leftNode = sentinelNode;
		node->parentNode = sentinelNode;
		root = node;
		count++;
		return;
	}

	TreeNode* node = root;
	while (node != sentinelNode)  //searches the tree for the right spot to insert the KeyValue
	{
		if (KeyValue.first == node->KeyValuePair.first)  //if the key already exists,the value is updated
		{
			node->KeyValuePair.second = KeyValue.second;
			break;
		}

		if (comparator(node->KeyValuePair.first, KeyValue.first))  //same as if (KeyValue.first > node->KeyValuePair.first) but replaced with comparator    //if the key inserted is higher than the node key,it will be inserted in the right subtree
		{
			if (node->rightNode == sentinelNode)  //if the right child of the current node is the sentinelNode,then the new node will be inserted as the right child of the current node
			{
				TreeNode* newNode = new TreeNode;
				newNode->KeyValuePair = KeyValue;
				newNode->parentNode = node;
				newNode->leftNode = sentinelNode;
				newNode->rightNode = sentinelNode;
				node->rightNode = newNode;
				InsertRepair(newNode); //the function takes the node that was inserted as a parameter and if it is needed rebalances the tree
				count++;
				break;
			}
			else  //if the node doesn't reach the sentinelNode,then it keeps searching for the correct spot to insert the value in the right subtree
				node = node->rightNode;
		}
		else  //if the key inserted is lower than the node key,it will be inserted in the left subtree
		{
			if (node->leftNode == sentinelNode)  //if the left child of the current node is the sentinelNode,then the new node will be inserted as the left child of the current node
			{
				TreeNode* newNode = new TreeNode;
				newNode->KeyValuePair = KeyValue;
				newNode->parentNode = node;
				newNode->leftNode = sentinelNode;
				newNode->rightNode = sentinelNode;
				node->leftNode = newNode;
				InsertRepair(newNode); //the function takes the node that was inserted as a parameter and if it is needed rebalances the tree
				count++;
				break;
			}
			else  //if the node doesn't reach the sentinelNode,then it keeps searching for the correct spot to insert the value in the left subtree
				node = node->leftNode;
		}
	}
}

template <typename KeyType, typename ValueType, typename Compare>
typename  RBT<KeyType, ValueType, Compare>::TreeNode* RBT<KeyType, ValueType, Compare>::max(TreeNode* node)  //returns a pointer to the node with the maximum key from the subtree rooted in the parameter node
{
	if (node == sentinelNode)
		return nullptr;

	while (node->rightNode != sentinelNode)
		node = node->rightNode;

	return node;
}

template <typename KeyType, typename ValueType, typename Compare>
typename  RBT<KeyType, ValueType, Compare>::TreeNode* RBT<KeyType, ValueType, Compare>::min(TreeNode* node)
{
	if (node == sentinelNode)
		return nullptr;

	while (node->leftNode != sentinelNode)
		node = node->leftNode;

	return node;
}

template <typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::TreeNode* RBT<KeyType, ValueType, Compare>::successor(TreeNode* node)  //returns a pointer to the successor of the node
{
	if (node->rightNode != sentinelNode)  //if the node has a right child,the successor is the minimum from the right subtree
		return min(node->rightNode);
	else  //if the node doesn't have a right child,the successor is the first parent node that has contains the node in the left subtree
	{
		while (node != sentinelNode)
		{
			if (node->parentNode->leftNode == node)
				return node->parentNode;
			node = node->parentNode;
		}
		return nullptr;
	}
}

template <typename KeyType, typename ValueType, typename Compare>
typename  RBT<KeyType, ValueType, Compare>::TreeNode* RBT<KeyType, ValueType, Compare>::predecessor(TreeNode* node)  //returns a pointer to the predecessor of the node
{
	if (node->leftNode != sentinelNode)  //if the node has a left child,the predecessor is the maximum from the left subtree
		return max(node->leftNode);
	else   //if the node doesn't have a left child,the predecessor is the first parent node that has contains the node in the right subtree
	{
		while (node != sentinelNode)
		{
			if (node->parentNode->rightNode == node)
				return node->parentNode;
			node = node->parentNode;
		}
		return nullptr;
	}
}

template <typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::TreeNode* RBT<KeyType, ValueType, Compare>::find(const KeyType& keySearch)
{
	TreeNode* node = root;
	while (node != sentinelNode)
	{
		if (keySearch == node->KeyValuePair.first)
			return node;

		if (comparator(node->KeyValuePair.first, keySearch))  //if the value is higher than the node key,it will search in the right subtree
		{
			if (node == sentinelNode)
				return nullptr;
			else  //if the node is not the sentinelNode,then it keeps searching in the right subtree
				node = node->rightNode;
		}
		else  //if the value is lower than the node key,it will search in the left subtree
		{
			if (node == sentinelNode)
				return nullptr;
			else  //if the node is not a leaf node,then it keeps searching in the left subtree
				node = node->leftNode;
		}
	}
	return nullptr;
}

template <typename KeyType, typename ValueType, typename Compare>
void  RBT<KeyType, ValueType, Compare>::deleteKey(const KeyType& keyDeleted)
{
	TreeNode* nodeDeleted = find(keyDeleted);

	if (nodeDeleted)  //if the key is found,the node with that key is erased,otherwise nothing happens
	{
		erase(nodeDeleted);
		count--;
	}
}

template <typename KeyType, typename ValueType, typename Compare>
ValueType& RBT<KeyType, ValueType, Compare>::operator[](const KeyType& key)
{
	if (find(key) == nullptr)  //if the key is not found in the tree,it is inserted
	{
		insert({ key,ValueType() });
		count++;
	}

	return find(key)->KeyValuePair.second;  //it returns the value of the key that was either found or inserted
}

template <typename KeyType, typename ValueType, typename Compare>
typename  RBT<KeyType, ValueType, Compare>::RBT& RBT<KeyType, ValueType, Compare>::operator=(const RBT& other)  //makes a deep copy of a tree
{
	if (this == &other) // this prevents a=a
		return *this;

	clear();

	if (other.root == nullptr || other.root == other.sentinelNode)  //if the other tree is empty,just retun this empty tree
		return *this;

	CopySubtree(other.root, other.sentinelNode, sentinelNode, true);
	count = other.count;
	comparator = other.comparator;

	return *this;
}

template <typename KeyType, typename ValueType, typename Compare>
int RBT<KeyType, ValueType, Compare>::size()
{
	return count;
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::clear()
{
	count = 0;
	clear(root);
	root = sentinelNode;
}

template <typename KeyType, typename ValueType, typename Compare>
bool RBT<KeyType, ValueType, Compare>::empty()
{
	return root == sentinelNode;
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayPreOrder()
{
	displayPreOrder(root);
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayInOrder()
{
	displayInOrder(root);
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayPostOrder()
{
	displayPostOrder(root);
}

template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayKeyValue()
{
	displayKeyValue(root);
}

template <typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::TreeNode* RBT<KeyType, ValueType, Compare>::getSentinelNode()
{
	return sentinelNode;
}

template <typename KeyType, typename ValueType, typename Compare>
typename  RBT<KeyType, ValueType, Compare>::TreeNode* RBT<KeyType, ValueType, Compare>::getRoot()
{
	return root;
}