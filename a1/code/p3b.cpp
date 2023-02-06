#include <stdio.h>
#include <string.h>
#include <x86intrin.h>
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

#define PROB 1/2
#define ITERATIONS 50

class BiNode {
	public:
		int key;
		BiNode** prev;
		BiNode** next;


		BiNode(int k, int level) {
			key = k;
			prev = new BiNode*[level + 1];
			next = new BiNode*[level + 1];
			memset(prev, 0, sizeof(BiNode*) * (level + 1));
			memset(next, 0, sizeof(BiNode*) * (level + 1));
		}
};

class BiSkipList {
	public:
		BiSkipList(int max) {
			max_level = max;
			current_level = 0;
			sentinels = new BiNode(0, max_level);
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
			BiNode* current_node = sentinels;
			BiNode** new_next_level = new BiNode*[max_level + 1];
			memset(new_next_level, 0, sizeof(BiNode*) * (max_level + 1));

			for (int i = max_level; i >= 0; i--) {
				while(current_node->next[i] != NULL && current_node->next[i]->key < key) {
					current_node = current_node->next[i];
				}
				new_next_level[i] = current_node;
			}

			current_node = current_node->next[0];

			if (current_node == NULL || current_node->key != key) {
				int random_level = random();
				if (random_level > current_level) {
					for (int i = current_level + 1; i < random_level + 1; i++){
						new_next_level[i] = sentinels;
					};
					current_level = random_level;
				}

				BiNode* new_node = new BiNode(key, random_level);

				for (int i = 0; i < random_level; i++) {
					new_node->next[i] = new_next_level[i]->next[i];
					new_node->prev[i] = new_next_level[i]->prev[i];
					new_next_level[i]->next[i] = new_node;
					new_node->prev[i] = new_next_level[i];
				}
			}
		}

		BiNode* traverse(BiNode* node, int key) {
			BiNode* smallest_node = NULL;
			BiNode* current_node = node;

			for (int i = current_level - 1; i >= 0; i--) {
				if (current_node->next[i] != NULL && current_node->next[i]->key < key) {
					if (smallest_node == NULL || smallest_node->key > current_node->next[i]->key) {
						smallest_node = current_node->next[i];
					}
				}
			}
			return smallest_node;
		}

		BiNode* search(int key) {
			BiNode* current_node = sentinels;
			BiNode* node = sentinels;
			int level = 0;

			for (int i = 0; i <= current_level; i++) {
				if (current_node->next[i] != NULL && current_node->next[i]->key <= key) {
					node = current_node->next[i];
					level = i;
				}
			}

			for (int i = level; i >= 0; i--) {
				while (node->next[i] != NULL && node->next[i]->key <= key) {
					node = node->next[i];
				}
			}

			if (node && node->key == key) {
				return node;
			}
			return NULL;
		}
		
	private:
		BiNode* sentinels;
		int max_level;
		int current_level;
};

int main() {
	int n = 1000;
	BiSkipList bi_skip_list(log(n));

	u_int64_t total = 0, min = -1, max = 0;
  ofstream insert_file;
	cout << "opening file\n";
  insert_file.open ("p3b_insert.csv");
	insert_file << "Bi Skip List\n";
	for(int i = 0; i < ITERATIONS; i++) {
		u_int64_t hs = __rdtsc();
		bi_skip_list.insert(i);
		u_int64_t he = __rdtsc() - hs;
		total += he;
		if (he > max) {
			max = he;
		}
		if (he < min) {
			min = he;
		}
		insert_file << he << "\n";
	}
	cout << "closing file\n";
	insert_file.close();

	printf("Insert Latency: %lu\n", total / ITERATIONS);
	printf("min: %lu\n", min);
	printf("max: %lu\n", max);

	total = 0, min = -1, max = 0;
  ofstream search_file;
	cout << "opening file\n";
  search_file.open ("p3b_search.csv");
	search_file << "Bi Skip List\n";
	for(int i = 0; i < ITERATIONS; i++) {
		u_int64_t hs = __rdtsc();
		bi_skip_list.search(i);
		u_int64_t he = __rdtsc() - hs;
		total += he;
		if (he > max) {
			max = he;
		}
		if (he < min) {
			min = he;
		}
		search_file << he << "\n";
	}
	cout << "closing file\n";
	search_file.close();

	printf("Search Latency: %lu\n", total / ITERATIONS);
	printf("min: %lu\n", min);
	printf("max: %lu\n", max);
}