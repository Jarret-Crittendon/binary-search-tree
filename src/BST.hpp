// Binary Search Tree
// 8 July 2022
// Jarret Crittendon

#include <memory>
#include <iterator>

template <typename T>
class BST {
    struct Node;
    friend class Iterator;
public:
    class Iterator;
    

    using value_type = T;
    using reference = value_type&;
    using size_type = size_t;
    using iterator = Iterator;

    BST(): root(nullptr), end_node(nullptr), node_count(0) { }

    auto add(const T& value) -> void;
    auto find(const T& value)  -> iterator;
    auto minimum()  -> T;
    auto maximum()  -> T;
    auto erase(const T& value) -> iterator;
    auto inorder()  -> void;

    auto begin() noexcept -> iterator { return iterator(root); }
    auto end() noexcept -> iterator { return iterator(end_node); }

    ~BST();

private:
    struct Node {
        explicit Node(const T& value):
            _data(value), parent(nullptr), child_left(nullptr), child_right(nullptr) { }

        T _data;       
        Node* parent;

        // Data that is greater than the data in the parent node goes in the right child node
        Node* child_left;
        Node* child_right;

    };


    auto search(Node* , const T&)  -> Node*;
    auto iterative_search(Node*, const T&)  -> Node*;
    auto successor(Node* )  -> Node*;
    auto predecessor(Node* )  -> Node*;
    auto min_node( Node*)  -> Node*;
    auto max_node( Node*)  -> Node*;
    auto tree_walk(const Node* )  -> void;
    auto transplant(Node*, Node*) -> void;

    Node* root;
    Node* end_node;
    unsigned node_count;

    std::allocator<Node> alloc;
    std::allocator_traits<decltype(alloc)> alloc_t;

};

template <typename T>
auto BST<T>::add(const T& key) -> void {
    auto current = root;
    decltype(current) previous = nullptr;
    while (current != end_node) {
        previous = current;
        if (key < current->_data) {
            current = current->child_left;
        }
        else {
            current = current->child_right;
        }
    }

    auto new_node = alloc_t.allocate(alloc, 1);
    alloc_t.construct(alloc, new_node, key);

    new_node->parent = previous;
    if (previous == end_node) {
        root = new_node;
    } else if (new_node->_data < previous->_data) {
        previous->child_left = new_node;
    } else {
        previous->child_right = new_node;
    }

}

template <typename T>
auto BST<T>::min_node(Node* node)  -> Node* {
    while (node->child_left != end_node) {
        node = node->child_left;
    }

    return node;
}

template <typename T>
auto BST<T>::minimum() -> T {
    Node* node = min_node(root);

    return node->_data;
}

template <typename T>
auto BST<T>::max_node (Node* node) -> Node* {
    while (node->child_right != end_node) {
        node = node->child_right;
    }

    return node;
}

template <typename T>
auto BST<T>::maximum() -> T {
    Node* node = max_node(root);

    return node->_data;
}



template <typename T>
auto BST<T>::search(Node* node, const T& key)  -> Node* {
    if (node == end_node || key == node->_data) {
        return node;
    }

    if (key < node->_data) {
        return search(node->child_left, key);
    } else {
        return search(node->child_right, key);
    }
}

template <typename T>
auto BST<T>::iterative_search(Node* node, const T& key)  -> Node* {
    while (node != end_node && key != node->_data) {
        if (key < node->_data) {
            node = node->child_left;
        } else {
            node = node->child_right;
        }
    } 

    return node;
}

// Inorder Successor of an input node can also be defined as
// the node with the smallest key greater than
// the key of the input node.

/*
If right subtree of node is not NULL, then successor lies
in right subtree.  Do the following. 
Go to right subtree and return the node with minimum key value in
the right subtree.

If right subtree of node is NULL, then successor is
one of the ancestors.  Do the following. 
Travel up using the parent pointer until you see a node
which is left child of its parent. The parent of such a node
is the successor.
*/

template <typename T>
auto BST<T>::successor(Node* node)  -> Node* {
    // If the right subtree of node x is not empty,
    // then the successor of x is the leftmost node
    // in x's right subtree

    if (node->child_right != end_node) {
        return min_node(node->child_right);
    }
    auto other = node->parent;
    while (other != end_node && node == other->child_right) {
        node = other;
        other = other->parent;
    }

    return other;
}

/*
If left subtree of node is not NULL, then predecessor lies
in left subtree.  Do the following. 
Go to left subtree and return the node with maximum key value in
the left subtree.

If left subtree of node is NULL, then predecessor is
one of the ancestors.  Do the following. 
Travel up using the parent pointer until you see a node
which is right child of its parent. The parent of such a node
is the predecessor.
*/

template <typename T>
auto BST<T>::predecessor(Node* node)  -> Node* {
    // If the right subtree of node x is not empty,
    // then the successor of x is the leftmost node
    // in x's right subtree

    if (node->child_left != end_node) {
        return max_node(node->child_left);
    }
    auto other = node->parent;
    while (other != end_node && node == other->child_left) {
        node = other;
        other = other->parent;
    }

    return other;
}

template <typename T>
auto BST<T>::find(const T& value)  -> iterator {
    auto node = search(root, value);
    return iterator(node);
}


template <typename T>
auto BST<T>::transplant(Node* old_node, Node* new_node) -> void {
    if (old_node->parent == end_node) {
        root = new_node;
    } else if (old_node == old_node->parent->child_left) {
        old_node->parent->child_left = new_node;
    } else {
        old_node->parent->child_right = new_node;
    }

    if (new_node != end_node) {
        new_node->parent = old_node->parent;
    }
}

template <typename T>
auto BST<T>::erase(const T& value) -> iterator {
    // Using the Hibbard delete algorithm; there are three cases that need handling:
    // 1. Node to be removed has no children
    //          - set parent-to-child link to null, then delete the child node
    // 2. Node to be removed has one child
    //          - Node is unlinked from tree, and its child is linked to the previous
    //          - node's parent 
    // 3. Node to be removed has two children
    //          - Find minimum value in right subtree
    //          - replace value of the node to be removed with found minimum
    //          - remove the duplicate (minimum) node
    // https://www.algolist.net/Data_structures/Binary_search_tree/Removal

    auto node = iterative_search(root, value);
    decltype(node) other = end_node;


    if (node->child_left == end_node) {
        transplant(node, node->child_right);
    } else if (node->child_right == end_node) {
        transplant(node, node->child_left);
    } else {
        // if node has two children, get the successor
        other = min_node(node->child_right);

        if (other->parent != node) {
            transplant(other, other->child_right);
            other->child_right = node->child_right;
            other->child_right->parent = other;
        }

        transplant(node, other);
        other->child_left = node->child_left;
        other->child_left->parent = other;
    }

    alloc_t.destroy(alloc, node);
    alloc_t.deallocate(alloc, node, 1);
    node = nullptr;

    return iterator(other);
   
}

template <typename T>
auto BST<T>::inorder()  -> void {
    tree_walk(root);
}

template <typename T>
auto BST<T>::tree_walk(const Node* node)  -> void {
    if (node != end_node) {
        tree_walk(node->child_left);
        std::cout << node->_data << std::endl;
        tree_walk(node->child_right);
    }
}

template <typename T>
BST<T>::~BST() {
    while (root != end_node) {
        erase(root->_data); 
    }
}

template <typename T>
class BST<T>::Iterator : private BST {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;
    //BST& outer;

    explicit Iterator(Node* ptr): _ptr(ptr) { }

    auto operator*() const -> reference {
        return _ptr->_data;
    }

    auto operator->() const -> Node* {
        return _ptr;
    }

    // postfix increment
    auto operator++(int) -> Iterator {
        auto copy = *this;

        _ptr = successor(_ptr);
        return copy;
    }

    // prefix increment
    auto operator++() -> Iterator& {
        _ptr = successor(_ptr);
        return *this;
    }

    // postfix decrement
    auto operator--(int) -> Iterator {
        auto copy = *this;

        _ptr = predecessor(_ptr);
        return copy;
    }

    // prefix decrement
    auto operator--() -> Iterator& {
        _ptr = predecessor(_ptr);
        return *this;
    }

    friend auto operator==(const Iterator& lhs, const Iterator& rhs) -> bool {
        return lhs._ptr == rhs._ptr;
    }

    friend auto operator!=(const Iterator& lhs, const Iterator& rhs) -> bool {
        return !(lhs._ptr == rhs._ptr);
    }

private:
    Node* _ptr;
};