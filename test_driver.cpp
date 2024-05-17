#include "solution.cpp"

int main()
{
    BST<int> bst;
    int a;
    int num;
	TreeNode<int> * result;
    int count = 0;
    while(count < 20)
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
        count++;
    }
    
}
