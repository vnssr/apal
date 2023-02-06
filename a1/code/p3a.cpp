#include <stdio.h>
#include <string.h>
#include <x86intrin.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <list>
using namespace std;

#define PROB 1/2
#define ITERATIONS 50

class Node {
	public:
		int key;
		Node** pointers;

		Node(int key, int level) {
			key = key;
			pointers = new Node*[level + 1];
			memset(pointers, 0, sizeof(Node*) * (level + 1));
		}
};

class SkipList {
	public:
		SkipList(int max) {
			max_level = max;
			current_level = 0;
			sentinels = new Node(0, max_level);
		}

		int random() {
			int level = 1;
			float random_num = (float) rand()/RAND_MAX;
			while (random_num < PROB && level < max_level) {
				random_num = (float) rand()/RAND_MAX;
				level++;
			}
			return level;
		}
		
		void insert(int key) {
			Node* current_node = sentinels;
			Node** new_level = new Node*[max_level + 1];
			memset(new_level, 0, sizeof(Node*) * (max_level + 1));

			for (int i = max_level; i >= 0; i--) {
				while(current_node->pointers[i] != NULL && current_node->pointers[i]->key < key) {
					current_node = current_node->pointers[i];
				}
				new_level[i] = current_node;
			}

			current_node = current_node->pointers[0];

			if (current_node == NULL || current_node->key != key) {
				int random_level = random();
				if (random_level > current_level) {
					for (int i = current_level + 1; i < random_level + 1; i++){
						new_level[i] = sentinels;
					};
					current_level = random_level;
				}

				Node* new_node = new Node(key, random_level);

				for (int i = 0; i < random_level; i++) {
					new_node->pointers[i] = new_level[i]->pointers[i];
					new_level[i]->pointers[i] = new_node;
				}
			}
		}

		Node* search(int key) {
			Node* current_node = sentinels;
			for (int i = max_level; i >= 0; i--) {
				while(current_node->pointers[i] != NULL && current_node->pointers[i]->key < key) {
					current_node = current_node->pointers[i];
				}
			}

			current_node = current_node->pointers[0];
			if (current_node && current_node->key == key) {
				return current_node;
			}
			return NULL;
		}
		
	private:
		Node* sentinels;
		int max_level;
		int current_level;
};

int get(list<int> linked_list, int key) {
    list<int>::iterator it;
    for (it = linked_list.begin(); it != linked_list.end(); it++) {
			if (*it == key) {
				return *it;
			}
		}
}

int main() {
	int n = 1000;
	SkipList skip_list(log(n));
	list<int> linked_list;

	u_int64_t total = 0, min = -1, max = 0;
  ofstream insert_file;
	cout << "opening file\n";
  insert_file.open ("insert.csv");
	insert_file << "Skip List,Linked List\n";
	for(int i = 0; i < ITERATIONS; i++) {
		u_int64_t hs = __rdtsc();
		skip_list.insert(i);
		u_int64_t he = __rdtsc() - hs;
		total += he;
		if (he > max) {
			max = he;
		}
		if (he < min) {
			min = he;
		}
		u_int64_t vs = __rdtsc();
		linked_list.push_back(i);
		u_int64_t ve = __rdtsc() - hs;
		insert_file << he << "," << ve << "\n";
	}
	cout << "closing file\n";
	insert_file.close();

	printf("Insert Latency: %lu\n", total / ITERATIONS);
	printf("min: %lu\n", min);
	printf("max: %lu\n", max);

	total = 0, min = -1, max = 0;
  ofstream search_file;
	cout << "opening file\n";
  search_file.open ("search.csv");
	search_file << "Skip List,Linked List\n";
	for(int i = 0; i < ITERATIONS; i++) {
		u_int64_t hs = __rdtsc();
		skip_list.search(i);
		u_int64_t he = __rdtsc() - hs;
		total += he;
		if (he > max) {
			max = he;
		}
		if (he < min) {
			min = he;
		}
		u_int64_t vs = __rdtsc();
		get(linked_list, i);
		u_int64_t ve = __rdtsc() - hs;
		search_file << he << "," << ve << "\n";
	}
	cout << "closing file\n";
	search_file.close();

	printf("Search Latency: %lu\n", total / ITERATIONS);
	printf("min: %lu\n", min);
	printf("max: %lu\n", max);
}