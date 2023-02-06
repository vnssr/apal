class Node {
	public:
		int key;
		Node** pointers;

		Node(int k, int level);
};

class SkipList {
	public:
		SkipList(int max);
		int random();
		void insert(int key);
		Node* search(int key);
		
	private:
		Node* sentinels;
		int max_level;
		int current_level;
};