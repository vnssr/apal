#include <stdio.h>
#include <string.h>
#include <x86intrin.h>

#define ITERATIONS 100

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
					printf("%d ", blocks[block_index][pos]);
			}
			printf("\n");
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

	uint64_t total = 0;
	for(int i = 0; i < ITERATIONS; i++) {
		uint64_t start = __rdtsc();
		hat.append(i);
		total += __rdtsc() - start;
	}
	printf("Append Latency: %lu\n", total / ITERATIONS);

	total = 0;
	for(int i = 0; i < ITERATIONS; i++) {
		uint64_t start = __rdtsc();
		hat.access(i);
		total += __rdtsc() - start;
	}
	printf("Access Latency: %lu\n", total / ITERATIONS);

	total = 0;
	for(int i = 0; i < ITERATIONS; i++) {
		uint64_t start = __rdtsc();
		hat.scan();
		total += __rdtsc() - start;
	}
	printf("Scan Latency: %lu\n", total / ITERATIONS);

  return 0;
}
