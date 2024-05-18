struct Node {
	char	data;
	Node*	next;
	// constructor instantiates a node with default arguments
	Node(char i = 0, Node * n = nullptr) {
	 	data = i, next = n;
	}
};

int stack_clear(Node * p);

bool stack_empty(Node * p);

int stack_size(Node * p);	

Node * stack_push(Node * p, char val);

Node * stack_pop(Node * p);

Node * stack_top(Node * p);