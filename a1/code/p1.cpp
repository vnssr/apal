#include <stdio.h>
#include <string.h>
#include <x86intrin.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#define ITERATIONS 50

class HashedArrayTree {
 	public:
		HashedArrayTree() : num_blocks(1), size(0), blocks((int**) malloc(1 * sizeof(int*))) {
			for (int i = 0; i < num_blocks; i++) {
				blocks[i] = (int*) malloc(num_blocks * sizeof(int));
				memset(blocks[i], 0, num_blocks * sizeof(int));
			}
		}

		~HashedArrayTree() {
			for (int i = 0; i < num_blocks; i++) {
				free(blocks[i]);
			}
			free(blocks);
		}

		void append(int value) {
			if (size == num_blocks) {
				expand();
			}
			int block_index = size / num_blocks;
			int offset = size % num_blocks;
			blocks[block_index][offset] = value;
			size++;
		}

		int access(int index) const {
			if (index >= size) {
				return -1;
			}
			int block_index = index / num_blocks;
			int offset = index % num_blocks;
			return blocks[block_index][offset];
		}

		void scan() const {
			for (int i = 0; i < size; ++i) {
					int block_index = i / num_blocks;
					int pos = i % num_blocks;
					// printf("%d ", blocks[block_index][pos]);
			}
			// printf("\n");
		}

 	private:
		int num_blocks;
		int size;
		int** blocks;

		void expand() {
			int old_num_blocks = num_blocks;
			num_blocks *= 2;
			int** new_blocks = (int**) malloc(num_blocks * sizeof(int*));
			for (int i = 0; i < old_num_blocks; i++) {
				new_blocks[i] = blocks[i];
			}
			for (int i = old_num_blocks; i < num_blocks; i++) {
				new_blocks[i] = (int*) malloc(num_blocks * sizeof(int));
				memset(new_blocks[i], 0, num_blocks * sizeof(int));
			}
			blocks = new_blocks;
		}
};

int main() {
  HashedArrayTree hat;
	vector<int> arr;

	u_int64_t total = 0, min = -1, max = 0;
  ofstream append_file;
	cout << "opening file\n";
  append_file.open ("append.csv");
	append_file << "HAT, Vector\n";
	for(int i = 0; i < ITERATIONS; i++) {
		u_int64_t hs = __rdtsc();
		hat.append(i);
		u_int64_t he = __rdtsc() - hs;
		total += he;
		if (he > max) {
			max = he;
		}
		if (he < min) {
			min = he;
		}
		u_int64_t vs = __rdtsc();
		arr.push_back(i);
		u_int64_t ve = __rdtsc() - hs;
		append_file << he << "," << ve << "\n";
	}
	cout << "closing file\n";
	append_file.close();

	printf("append Latency: %lu\n", total / ITERATIONS);
	printf("min: %lu\n", min);
	printf("max: %lu\n", max);

	total = 0, min = -1, max = 0;
	ofstream access_file;
	cout << "opening file\n";
  access_file.open ("access.csv");
	access_file << "HAT, Vector\n";
	for(int i = 0; i < ITERATIONS; i++) {
		u_int64_t hs = __rdtsc();
		hat.access(i);
		u_int64_t he = __rdtsc() - hs;
		total += he;
		if (he > max) {
			max = he;
		}
		if (he < min) {
			min = he;
		}	
		u_int64_t vs = __rdtsc();
		arr.at(i);
		u_int64_t ve = __rdtsc() - hs;
		access_file << he << "," << ve << "\n";
	}
	cout << "closing file\n";
	access_file.close();

	printf("Access Latency: %lu\n", total / ITERATIONS);
	printf("min: %lu\n", min);
	printf("max: %lu\n", max);

	total = 0, min = -1, max = 0;
	ofstream scan_file;
	cout << "opening file\n";
  scan_file.open ("scan.csv");
	scan_file << "HAT, Vector\n";
	for(int i = 0; i < ITERATIONS; i++) {
		u_int64_t hs = __rdtsc();
		hat.scan();
		u_int64_t he = __rdtsc() - hs;
		total += he;
		if (he > max) {
			max = he;
		}
		if (he < min) {
			min = he;
		}
		u_int64_t vs = __rdtsc();
		arr.back();
		u_int64_t ve = __rdtsc() - hs;
		scan_file << he << "," << ve << "\n";	}
	cout << "closing file\n";
	scan_file.close();

	printf("Scan Latency: %lu\n", total / ITERATIONS);
	printf("min: %lu\n", min);
	printf("max: %lu\n", max);

  return 0;
}
