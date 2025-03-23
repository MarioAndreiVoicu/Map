export module RedBlackTree;

import <iostream>;
import <utility>;

/**
 * @brief A red–black tree implementation.
 *
 * This template implements a red–black tree supporting basic operations
 * such as insertion, deletion, and traversal.
 *
 * @tparam KeyType   Type of keys.
 * @tparam ValueType Type of values.
 * @tparam Compare   Comparison functor (default: std::less<KeyType>).
 */
export template <typename KeyType, typename ValueType, typename Compare = std::less<KeyType>>
class RBT
{
public:
    using KeyValue = std::pair<KeyType, ValueType>;

    /**
     * @brief Node structure representing an element in the tree.
     */
    struct Node
    {
        KeyValue KeyValuePair; ///< The key-value pair.
        bool isRed;            ///< Color flag: true if red, false if black.
        Node* left;            ///< Left child.
        Node* right;           ///< Right child.
        Node* parent;          ///< Parent node.

        /**
         * @brief Constructs a Node.
         *
         * @param KeyValue The key-value pair (default: default-constructed).
         * @param red      The color flag (default: true).
         * @param left     Pointer to the left child (default: nullptr).
         * @param right    Pointer to the right child (default: nullptr).
         * @param parent   Pointer to the parent node (default: nullptr).
         */
        Node(const KeyValue& KeyValue = std::make_pair(KeyType(), ValueType()),
            bool red = true,
            Node* left = nullptr,
            Node* right = nullptr,
            Node* parent = nullptr)
            : KeyValuePair(KeyValue), isRed(red), left(left), right(right), parent(parent) {}
    };

private:
    Node* root;         ///< Root of the tree.
    Node* sentinel;     ///< Sentinel node representing leaves.
    Compare comparator; ///< Comparator for key ordering.
    size_t count;       ///< Number of elements.

    // Helper functions
    void RotateLeft(Node* node);
    void RotateRight(Node* node);
    void InsertRepair(Node* node);
    void EraseRepair(Node* node);
    void TransplantRBT(Node* node1, Node* node2);
    void clear(Node* node);
    void displayPreOrder(Node* node) const;
    void displayInOrder(Node* node) const;
    void displayPostOrder(Node* node) const;
    void displayKeyValue(Node* node) const;
    void erase(Node* nodeDeleted);
    Node* CopySubtree(Node* node, Node* otherSentinel, Node* parent = nullptr, bool isRoot = false);

public:
    // Constructors and destructor
    RBT(const Compare& comparator = Compare());
    RBT(const RBT& other, const Compare& comparator = Compare());
    RBT& operator=(const RBT& other);
    RBT(RBT&& other) noexcept;
    RBT& operator=(RBT&& other) noexcept;
    ~RBT();

    // Public member functions
    void insert(const KeyValue& KeyValue);
    Node* max(Node* node) const;
    Node* min(Node* node) const;
    Node* successor(Node* node);
    const Node* successor(Node* node) const;
    Node* predecessor(Node* node);
    const Node* predecessor(Node* node) const;
    Node* find(const KeyType& keySearch);
    const Node* find(const KeyType& keySearch) const;
    void deleteKey(const KeyType& keyDeleted);
    ValueType& operator[](const KeyType& key);
    int size() const;
    void clear();
    bool empty() const;
    void displayPreOrder() const;
    void displayInOrder() const;
    void displayPostOrder() const;
    void displayKeyValue() const;
    Node* getSentinel() const;
    Node* getRoot() const;
};

/* ======================= Function Implementations ======================= */

/**
 * @brief Performs a left rotation around the given node.
 *
 * Adjusts pointers to maintain the red–black tree structure.
 *
 * @param node The node around which the rotation is performed.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::RotateLeft(Node* node)
{
    Node* rightChild = node->right;
    node->right = rightChild->left;
    if (rightChild->left != sentinel)
        rightChild->left->parent = node;
    rightChild->parent = node->parent;
    if (node->parent == sentinel)
    {
        root = rightChild;
        root->parent = sentinel;
    }
    else
    {
        if (node == node->parent->left)
            node->parent->left = rightChild;
        else
            node->parent->right = rightChild;
    }
    rightChild->left = node;
    node->parent = rightChild;
}

/**
 * @brief Performs a right rotation around the given node.
 *
 * @param node The node around which the rotation is performed.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::RotateRight(Node* node)
{
    Node* leftChild = node->left;
    node->left = leftChild->right;
    if (leftChild->right != sentinel)
        leftChild->right->parent = node;
    leftChild->parent = node->parent;
    if (node->parent == sentinel)
    {
        root = leftChild;
        root->parent = sentinel;
    }
    else
    {
        if (node == node->parent->right)
            node->parent->right = leftChild;
        else
            node->parent->left = leftChild;
    }
    leftChild->right = node;
    node->parent = leftChild;
}

/**
 * @brief Repairs the tree after insertion to restore red–black properties.
 *
 * @param node The newly inserted node.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::InsertRepair(Node* node)
{
    if (node == root)
    {
        node->isRed = false;
        return;
    }
    if (node->parent == sentinel || node->parent->parent == sentinel)
    {
        node->parent->isRed = false;
        return;
    }
    if (!node->parent->isRed)
        return;
    Node* parent = node->parent;
    Node* grandparent = parent->parent;
    Node* uncle = (grandparent->left == parent) ? grandparent->right : grandparent->left;
    if (node->isRed && parent->isRed && uncle->isRed)
    {
        parent->isRed = false;
        uncle->isRed = false;
        grandparent->isRed = true;
        InsertRepair(grandparent);
    }
    else if (node->isRed && parent->isRed && !uncle->isRed)
    {
        if (grandparent->right == parent && parent->left == node)
        {
            RotateRight(parent);
            InsertRepair(parent);
        }
        else if (grandparent->left == parent && parent->right == node)
        {
            RotateLeft(parent);
            InsertRepair(parent);
        }
        if (grandparent->right == parent && parent->right == node)
        {
            parent->isRed = false;
            grandparent->isRed = true;
            RotateLeft(grandparent);
            InsertRepair(parent);
        }
        else if (grandparent->left == parent && parent->left == node)
        {
            parent->isRed = false;
            grandparent->isRed = true;
            RotateRight(grandparent);
            InsertRepair(parent);
        }
    }
}

/**
 * @brief Repairs the tree after deletion to restore red–black properties.
 *
 * @param node The node that replaces the deleted node.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::EraseRepair(Node* node)
{
    while (node != root && !node->isRed)
    {
        if (node == node->parent->left)
        {
            Node* sibling = node->parent->right;
            if (sibling->isRed)
            {
                sibling->isRed = false;
                node->parent->isRed = true;
                RotateLeft(node->parent);
                sibling = node->parent->right;
            }
            else if (!sibling->left->isRed && !sibling->right->isRed)
            {
                sibling->isRed = true;
                node = node->parent;
            }
            else
            {
                if (!sibling->right->isRed)
                {
                    sibling->left->isRed = false;
                    sibling->isRed = true;
                    RotateRight(sibling);
                }
                else
                {
                    sibling->isRed = node->parent->isRed;
                    node->parent->isRed = false;
                    sibling->right->isRed = false;
                    RotateLeft(node->parent);
                    node = root;
                }
            }
        }
        else
        {
            Node* sibling = node->parent->left;
            if (sibling->isRed)
            {
                sibling->isRed = false;
                node->parent->isRed = true;
                RotateRight(node->parent);
                sibling = node->parent->left;
            }
            else if (!sibling->right->isRed && !sibling->left->isRed)
            {
                sibling->isRed = true;
                node = node->parent;
            }
            else
            {
                if (!sibling->left->isRed)
                {
                    sibling->right->isRed = false;
                    sibling->isRed = true;
                    RotateLeft(sibling);
                }
                else
                {
                    sibling->isRed = node->parent->isRed;
                    node->parent->isRed = false;
                    sibling->left->isRed = false;
                    RotateRight(node->parent);
                    node = root;
                }
            }
        }
    }
    node->isRed = false;
}

/**
 * @brief Replaces one subtree with another.
 *
 * @param node1 The subtree to be replaced.
 * @param node2 The subtree that replaces node1.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::TransplantRBT(Node* node1, Node* node2)
{
    if (node1->parent == sentinel)
        root = node2;
    else
    {
        if (node1 == node1->parent->left)
            node1->parent->left = node2;
        else
            node1->parent->right = node2;
    }
    node2->parent = node1->parent;
}

/**
 * @brief Recursively clears the subtree using postorder traversal.
 *
 * @param node The current node to clear.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::clear(Node* node)
{
    if (node == nullptr || node == sentinel)
        return;
    clear(node->left);
    clear(node->right);
    delete node;
}

/**
 * @brief Displays the tree in preorder.
 *
 * @param node The current node.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayPreOrder(Node* node) const
{
    if (node != sentinel)
    {
        std::cout << "(" << node->KeyValuePair.first << "," << node->KeyValuePair.second << ")->";
        std::cout << (node->isRed ? "red " : "black ");
        displayPreOrder(node->left);
        displayPreOrder(node->right);
    }
}

/**
 * @brief Displays the tree in inorder.
 *
 * @param node The current node.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayInOrder(Node* node) const
{
    if (node != sentinel)
    {
        displayInOrder(node->left);
        std::cout << "(" << node->KeyValuePair.first << "," << node->KeyValuePair.second << ")->";
        std::cout << (node->isRed ? "red " : "black ");
        displayInOrder(node->right);
    }
}

/**
 * @brief Displays the tree in postorder.
 *
 * @param node The current node.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayPostOrder(Node* node) const
{
    if (node != sentinel)
    {
        displayPostOrder(node->left);
        displayPostOrder(node->right);
        std::cout << "(" << node->KeyValuePair.first << "," << node->KeyValuePair.second << ")->";
        std::cout << (node->isRed ? "red " : "black ");
    }
}

/**
 * @brief Displays only the key-value pairs in inorder.
 *
 * @param node The current node.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayKeyValue(Node* node) const
{
    if (node != sentinel)
    {
        displayKeyValue(node->left);
        std::cout << "(" << node->KeyValuePair.first << "," << node->KeyValuePair.second << ")  ";
        displayKeyValue(node->right);
    }
}

/**
 * @brief Deletes a node from the tree and repairs the structure.
 *
 * @param nodeDeleted The node to be deleted.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::erase(Node* nodeDeleted)
{
    if (nodeDeleted == sentinel)
        return;
    bool isRed = nodeDeleted->isRed;
    Node* node;
    if (nodeDeleted->left == sentinel)
    {
        node = nodeDeleted->right;
        TransplantRBT(nodeDeleted, node);
    }
    else if (nodeDeleted->right == sentinel)
    {
        node = nodeDeleted->left;
        TransplantRBT(nodeDeleted, node);
    }
    else
    {
        Node* successorNode = successor(nodeDeleted);
        isRed = successorNode->isRed;
        node = successorNode->right;
        if (successorNode->parent == nodeDeleted)
            node->parent = successorNode;
        else
        {
            TransplantRBT(successorNode, node);
            successorNode->right = nodeDeleted->right;
            nodeDeleted->right->parent = successorNode;
        }
        TransplantRBT(nodeDeleted, successorNode);
        successorNode->left = nodeDeleted->left;
        nodeDeleted->left->parent = successorNode;
        successorNode->isRed = nodeDeleted->isRed;
    }
    if (!isRed)
        EraseRepair(node);
}

/**
 * @brief Recursively copies a subtree.
 *
 * Used for deep-copying the tree.
 *
 * @param node The current node in the source tree.
 * @param otherSentinel The sentinel of the source tree.
 * @param parent The parent for the new node.
 * @param isRoot True if this node is the root of the new tree.
 * @return Pointer to the newly copied subtree.
 */
template <typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::Node* RBT<KeyType, ValueType, Compare>::CopySubtree(Node* node, Node* otherSentinel, Node* parent, bool isRoot)
{
    if (node == otherSentinel && !isRoot)
        return sentinel;
    Node* newNode = new Node(node->KeyValuePair, node->isRed);
    newNode->parent = parent;
    if (isRoot)
    {
        root = newNode;
        newNode->parent = sentinel;
    }
    newNode->left = CopySubtree(node->left, otherSentinel, newNode);
    newNode->right = CopySubtree(node->right, otherSentinel, newNode);
    return newNode;
}

/**
 * @brief Constructs an empty red–black tree.
 *
 * @param comparator The comparator to use (default: std::less<KeyType>).
 */
template <typename KeyType, typename ValueType, typename Compare>
RBT<KeyType, ValueType, Compare>::RBT(const Compare& comparator)
    : comparator(comparator), count(0)
{
    sentinel = new Node(std::make_pair(KeyType(), ValueType()), false);
    root = sentinel;
}

/**
 * @brief Copy constructor.
 *
 * @param other The tree to copy.
 * @param comparator The comparator to use.
 */
template <typename KeyType, typename ValueType, typename Compare>
RBT<KeyType, ValueType, Compare>::RBT(const RBT& other, const Compare& comparator)
    : comparator(comparator), count(0)
{
    sentinel = new Node(std::make_pair(KeyType(), ValueType()), false);
    root = sentinel;
    *this = other;
}

/**
 * @brief Move constructor.
 *
 * @param other The tree to copy.
 */
template<typename KeyType, typename ValueType, typename Compare>
RBT<KeyType, ValueType, Compare>& RBT<KeyType, ValueType, Compare>::operator=(const RBT& other)
{
    if (this != &other)
    {
        clear();
        root = CopySubtree(other.root, other.sentinel, sentinel, true);
        count = other.count;
    }
    return *this;
}

/**
 * @brief Move constructor.
 *
 * @param other The tree to move.
 */
template<typename KeyType, typename ValueType, typename Compare>
RBT<KeyType, ValueType, Compare>::RBT(RBT&& other) noexcept
    : root(other.root), sentinel(other.sentinel), comparator(std::move(other.comparator)), count(other.count)
{
    other.root = other.sentinel = nullptr;
    other.count = 0;
}

/**
 * @brief Move assignment operator.
 *
 * @param other The tree to move.
 */
template<typename KeyType, typename ValueType, typename Compare>
RBT<KeyType, ValueType, Compare>& RBT<KeyType, ValueType, Compare>::operator=(RBT&& other) noexcept
{
    if (this == &other)
        return *this;
    clear();
    root = other.root;
    sentinel = other.sentinel;
    comparator = std::move(other.comparator);
    count = other.count;
    other.root = other.sentinel = nullptr;
    other.count = 0;
    return *this;
}

/**
 * @brief Destructor.
 *
 * Clears the tree and deallocates memory.
 */
template <typename KeyType, typename ValueType, typename Compare>
RBT<KeyType, ValueType, Compare>::~RBT()
{
    count = 0;
    clear();
    delete sentinel;
}

/**
 * @brief Inserts a key-value pair into the tree.
 *
 * If the key already exists, its value is updated.
 *
 * @param KeyValue The key-value pair to insert.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::insert(const KeyValue& KeyValue)
{
    if (root == sentinel)
    {
        Node* node = new Node();
        node->KeyValuePair = KeyValue;
        node->isRed = false;
        node->left = sentinel;
        node->right = sentinel;
        node->parent = sentinel;
        root = node;
        count++;
        return;
    }
    Node* node = root;
    while (node != sentinel)
    {
        if (KeyValue.first == node->KeyValuePair.first)
        {
            node->KeyValuePair.second = KeyValue.second;
            break;
        }
        if (comparator(node->KeyValuePair.first, KeyValue.first))
        {
            if (node->right == sentinel)
            {
                Node* newNode = new Node;
                newNode->KeyValuePair = KeyValue;
                newNode->parent = node;
                newNode->left = sentinel;
                newNode->right = sentinel;
                node->right = newNode;
                InsertRepair(newNode);
                count++;
                break;
            }
            else
                node = node->right;
        }
        else
        {
            if (node->left == sentinel)
            {
                Node* newNode = new Node;
                newNode->KeyValuePair = KeyValue;
                newNode->parent = node;
                newNode->left = sentinel;
                newNode->right = sentinel;
                node->left = newNode;
                InsertRepair(newNode);
                count++;
                break;
            }
            else
                node = node->left;
        }
    }
}

/**
 * @brief Returns the node with the maximum key in the subtree.
 *
 * @param node The root of the subtree.
 * @return Pointer to the node with the maximum key, or nullptr if none.
 */
template <typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::Node* RBT<KeyType, ValueType, Compare>::max(Node* node) const
{
    if (node == sentinel)
        return nullptr;
    while (node->right != sentinel)
        node = node->right;
    return node;
}

/**
 * @brief Returns the node with the minimum key in the subtree.
 *
 * @param node The root of the subtree.
 * @return Pointer to the node with the minimum key, or nullptr if none.
 */
template <typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::Node* RBT<KeyType, ValueType, Compare>::min(Node* node) const
{
    if (node == sentinel)
        return nullptr;
    while (node->left != sentinel)
        node = node->left;
    return node;
}

/**
 * @brief Finds the successor of a given node.
 *
 * @param node The node whose successor is to be found.
 * @return Pointer to the successor node, or nullptr if none exists.
 */
template <typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::Node* RBT<KeyType, ValueType, Compare>::successor(Node* node)
{
    if (node->right != sentinel)
        return min(node->right);
    while (node != sentinel)
    {
        if (node->parent->left == node)
            return node->parent;
        node = node->parent;
    }
    return nullptr;
}

/**
 * @brief Finds the successor of a given node.
 *
 * Uses const_cast to call the non-const version then casts the result back to const.
 * This is a workaround to avoid code duplication
 *
 * @param node The node whose successor is to be found.
 * @return Pointer to the successor node, or nullptr if none exists.
 */
template <typename KeyType, typename ValueType, typename Compare>
const typename RBT<KeyType, ValueType, Compare>::Node* RBT<KeyType, ValueType, Compare>::successor(Node* node) const
{
    return const_cast<RBT*>(this)->successor(node);
}

/**
 * @brief Finds the predecessor of a given node.
 *
 * @param node The node whose predecessor is to be found.
 * @return Pointer to the predecessor node, or nullptr if none exists.
 */
template <typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::Node* RBT<KeyType, ValueType, Compare>::predecessor(Node* node)
{
    if (node->left != sentinel)
        return max(node->left);
    while (node != sentinel)
    {
        if (node->parent->right == node)
            return node->parent;
        node = node->parent;
    }
    return nullptr;
}

/*
*  @brief Finds the predecessor of a given node.
 *
 * Uses const_cast to call the non-const version then casts the result back to const.
 * This is a workaround to avoid code duplication
 *
 * @param node The node whose predecessor is to be found.
 * @return Pointer to the predecessor node, or nullptr if none exists.
*/
template <typename KeyType, typename ValueType, typename Compare>
const typename RBT<KeyType, ValueType, Compare>::Node* RBT<KeyType, ValueType, Compare>::predecessor(Node* node) const
{
    return const_cast<RBT*>(this)->predecessor(node);
}

/**
 * @brief Searches for a node with the given key.
 *
 * @param keySearch The key to search for.
 * @return Pointer to the node containing the key, or nullptr if not found.
 */
template <typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::Node* RBT<KeyType, ValueType, Compare>::find(const KeyType& keySearch)
{
    Node* node = root;
    while (node != sentinel)
    {
        if (keySearch == node->KeyValuePair.first)
            return node;
        node = comparator(node->KeyValuePair.first, keySearch) ? node->right : node->left;
    }
    return sentinel;
}

/**
 * @brief Searches for a node with the given key.
 *
 * Uses const_cast to call the non-const version then casts the result back to const.
 * This is a workaround to avoid code duplication
 *
 * @param keySearch The key to search for.
 * @return Pointer to the node containing the key, or nullptr if not found.
 */
template <typename KeyType, typename ValueType, typename Compare>
const typename RBT<KeyType, ValueType, Compare>::Node* RBT<KeyType, ValueType, Compare>::find(const KeyType& keySearch) const
{
    return const_cast<RBT*>(this)->find(keySearch);
}

/**
 * @brief Deletes the node with the specified key.
 *
 * @param keyDeleted The key of the node to delete.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::deleteKey(const KeyType& keyDeleted)
{
    Node* nodeDeleted = find(keyDeleted);
    if (nodeDeleted != sentinel)
    {
        erase(nodeDeleted);
        count--;
    }
}

/**
 * @brief Access operator.
 *
 * Inserts the key if not found and returns the associated value.
 *
 * @param key The key to access.
 * @return Reference to the value associated with the key.
 */
template <typename KeyType, typename ValueType, typename Compare>
ValueType& RBT<KeyType, ValueType, Compare>::operator[](const KeyType& key)
{
    Node* node = find(key);
    if (node == sentinel)
    {
        insert({ key, ValueType() });
        node = find(key);
    }
    return node->KeyValuePair.second;
}

/**
 * @brief Returns the number of elements in the tree.
 *
 * @return The size of the tree.
 */
template <typename KeyType, typename ValueType, typename Compare>
int RBT<KeyType, ValueType, Compare>::size() const
{
    return count;
}

/**
 * @brief Clears the tree.
 *
 * Removes all nodes and resets the tree to an empty state.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::clear()
{
    count = 0;
    clear(root);
    root = sentinel;
}

/**
 * @brief Checks if the tree is empty.
 *
 * @return True if empty, false otherwise.
 */
template <typename KeyType, typename ValueType, typename Compare>
bool RBT<KeyType, ValueType, Compare>::empty() const
{
    return root == sentinel;
}

/**
 * @brief Displays the tree in preorder.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayPreOrder() const
{
    displayPreOrder(root);
}

/**
 * @brief Displays the tree in inorder.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayInOrder() const
{
    displayInOrder(root);
}

/**
 * @brief Displays the tree in postorder.
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayPostOrder() const
{
    displayPostOrder(root);
}

/**
 * @brief Displays only the key-value pairs in the tree (inorder).
 */
template <typename KeyType, typename ValueType, typename Compare>
void RBT<KeyType, ValueType, Compare>::displayKeyValue() const
{
    displayKeyValue(root);
}

/**
 * @brief Returns the sentinel node.
 *
 * @return Pointer to the sentinel node.
 */
template <typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::Node* RBT<KeyType, ValueType, Compare>::getSentinel() const
{
    return sentinel;
}

/**
 * @brief Returns the root node.
 *
 * @return Pointer to the root node.
 */
template <typename KeyType, typename ValueType, typename Compare>
typename RBT<KeyType, ValueType, Compare>::Node* RBT<KeyType, ValueType, Compare>::getRoot() const
{
    return root;
}