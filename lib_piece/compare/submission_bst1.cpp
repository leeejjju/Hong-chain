#include <iostream>

template<typename T>
class TreeNode {
public:
    T data;
    TreeNode<T> *left;
    TreeNode<T> *right;

    TreeNode(T data) : data(data), left(nullptr), right(nullptr) {}
};

template<typename T>
class BST {
private:
    TreeNode<T> *root;

    TreeNode<T>* insertRecursive(TreeNode<T>* root, T data) {
        if (root == nullptr) {
            return new TreeNode<T>(data);
        }

        if (data < root->data) {
            root->left = insertRecursive(root->left, data);
        } 
        else if (data > root->data) {
            root->right = insertRecursive(root->right, data);
        }
        else {
            // 같은 값을 가진 노드가 이미 존재하는 경우, 여기서 처리
            std::cout << "Node with value " << data << " already exists in the tree." << std::endl;
        }

        return root;
    }

    TreeNode<T>* findMin(TreeNode<T>* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    TreeNode<T>* deleteRecursive(TreeNode<T>* root, T data) {
        if (root == nullptr) {
            return nullptr;
        }

        if (data < root->data) {
            root->left = deleteRecursive(root->left, data);
        } else if (data > root->data) {
            root->right = deleteRecursive(root->right, data);
        } else {
            // Case 1: No child or only one child
            //here make error change left and right
            if (root->left == nullptr) {
                TreeNode<T>* temp = root->left;
                delete root;
                return temp;
            } 
            else if (root->right == nullptr) {
                TreeNode<T>* temp = root->right;
                delete root;
                return temp;
            }

            // Case 2: Two children
            TreeNode<T>* temp = findMin(root->right);
            root->data = temp->data;
            root->right = deleteRecursive(root->right, temp->data);
        }

        return root;
    }

    TreeNode<T>* searchRecursive(TreeNode<T>* root, T data) {
        if (root == nullptr || root->data == data) {
            return root;
        }

        if (data < root->data) {
            return searchRecursive(root->left, data);
        } else {
            return searchRecursive(root->right, data);
        }
    }

    void inorderRecursive(TreeNode<T>* root) {
        if (root != nullptr) {
            inorderRecursive(root->left);
            std::cout << root->data << " ";
            inorderRecursive(root->right);
        }
    }

public:
    BST() : root(nullptr) {}

    void insert(T data) {
        root = insertRecursive(root, data);
    }

    void deleteNode(T data) {
        root = deleteRecursive(root, data);
    }

    TreeNode<T>* search(T data) {
        return searchRecursive(root, data);
    }

    void inorderTraversal() {
        inorderRecursive(root);
    }
};

#ifdef TEST
int main() {
    BST<int> bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);

    std::cout << "Inorder traversal of BST before deletion: ";
    bst.inorderTraversal();
    std::cout << std::endl;

    bst.deleteNode(30);
    std::cout << "Inorder traversal of BST after deletion of 30: ";
    bst.inorderTraversal();
    std::cout << std::endl;

    return 0;
}
#endif

#ifndef TEST
int main()
{
    BST<int> bst;
    int a;
    int num;
	TreeNode<int> * result;
    while(1)
    {
        std::cin >> a;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        switch (a)
        {
        case 1:
			std::cin >> num;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
			bst.insert(num);
			bst.inorderTraversal();
            std::cout << "\n";
			break;
        case 2:
			std::cin >> num;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
			bst.deleteNode(num);
			bst.inorderTraversal();
            std::cout << "\n";
			break;
        case 3:
			std::cin >> num;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            result = bst.search(num);
			if (result != nullptr) {
				std::cout << "Found: " << result->data << std::endl;
			} else {
				std::cout << "Not found" << std::endl;
			}
			break;
        case 1234:
            return 0;
        default:
          	break;
        }
    }
    
}
#endif
