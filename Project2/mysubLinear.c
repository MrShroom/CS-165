#include <stdlib.h>
#include <string.h>
#define ALL_SAME       4
#define ONE_DIFFERENT  2
#define EVEN_DIVIDE    0
#define GROUP_SIZE     4
typedef struct group_t {
	int indexes[GROUP_SIZE];
	int status;
} group;

void setgroup(group* thegroup, int start) {
    thegroup->indexes[0] = start;
    thegroup->indexes[1] = start + 1;
    thegroup->indexes[2] = start + 2;
    thegroup->indexes[3] = start + 3;
    thegroup->status = QCOUNT(1, thegroup->indexes);
}

group makegroup(int a, int b, int c, int d) {
	group thegroup;
	thegroup.indexes[0] = a;
	thegroup.indexes[1] = b;
	thegroup.indexes[2] = c;
	thegroup.indexes[3] = d;
	return thegroup;
}

int doleftovers(int majority_index, int n) {
	group g;
	setgroup(&g, majority_index);
	int i, next, remain = n % GROUP_SIZE, diff=0;
	for (next = 0; next < remain; next++) {
		for (i = 0; i < GROUP_SIZE; i++) {
			int status, temp = g.indexes[i];
			g.indexes[i] = next + (n - (n % GROUP_SIZE)) + 1;
			status = QCOUNT(1, g.indexes);
			g.indexes[i] = temp;

			if (status == g.status) continue;
			if (status != EVEN_DIVIDE) {
				// we can count this item
				// towards whatever bin.
				if (status == ALL_SAME)
					diff += 1;
				else if (status == ONE_DIFFERENT)
					diff -= 1;
				break;
			}
		}
	}
	return diff;
}

int mysub (int n) {
	QCOUNT(-1);
	int majority = 0; // will eventually be >= n / 2 
	int minority = 0;
	int majority_index = 0;
	int minority_index = -1;
	group **all_4_bin = (group**)malloc(sizeof(group*) * ((n + GROUP_SIZE)/GROUP_SIZE));
	int all_4_bin_size =0;
	group **one_to_3_bin = (group**)malloc(sizeof(group*) * ((n + GROUP_SIZE)/GROUP_SIZE));
	int one_to_3_bin_size =0;

	// loop over 0 to n - 4
	int i;
	for (i = 1; i <= n - (n - GROUP_SIZE); i += GROUP_SIZE) {
		group *temp = (group*)malloc(sizeof(group));
		setgroup(temp, i);
		if(temp->status == EVEN_DIVIDE)
		{
			free(temp);
		}
		else if(temp->status == ALL_SAME)
		{
			all_4_bin[all_4_bin_size++] = temp;
		}
		else if(temp->status == ONE_DIFFERENT)
		{
			one_to_3_bin[one_to_3_bin_size++] = temp;
		}

	}
	group *master = NULL, *slave = NULL;
	// pick one group of 4 to be master group.
	if (all_4_bin_size > 0) {
		// ASSUME master is majority, may swap later.
		master = all_4_bin[0];
		majority_index = master->indexes[0];
		majority = 4;
		for (i = 1; i < all_4_bin_size; i++) {
			group temp_group = makegroup(master->indexes[0], master->indexes[1], master->indexes[2], 
				all_4_bin[i]->indexes[0]);
			int status = QCOUNT(1, temp_group.indexes);
			if (status == ALL_SAME) majority += 4;
			else { 
				minority += 4;
				minority_index = all_4_bin[i]->indexes[0];
				slave = all_4_bin[i];
			}
		}

		if (minority > majority) {
			// swap
			int temp = minority;
			minority = majority;
			majority = temp;
			temp = majority_index;
			majority_index = minority_index;
			minority_index = temp;

			// swap the master groups.
			group *temp_group = master;
			master = slave;
			slave = temp_group;
		} 
	}
	if (majority <= n / 2 && one_to_3_bin_size > 0) {
		int local_majority_count=0;
		int local_minority_count=0;
		int local_majority_index =-2;//unknown yet
		int local_minority_index = -1;
		int master_group_index = -1;
		int master_group_index_of_index_array = -1;
		int master_majority_index = 0;
		int master_minority_index = 0;
		for (i = 0; i < one_to_3_bin_size; i++) 
		{
			int offset = (one_to_3_bin[i]->indexes[3]+1) % n;
			group temp_group;
			int j;
			if (local_majority_index < 0) {
				for (j = 0; j < GROUP_SIZE; j++) {
					temp_group = makegroup(one_to_3_bin[i]->indexes[0], one_to_3_bin[i]->indexes[1],
						one_to_3_bin[i]->indexes[2], one_to_3_bin[i]->indexes[3]);
					temp_group.indexes[j] = offset;
					int status = QCOUNT(1, temp_group.indexes);

					if (status != ONE_DIFFERENT) {
						master_group_index = i;
						if (status == EVEN_DIVIDE) {
							master_majority_index = local_majority_index = one_to_3_bin[i]->indexes[j];
							master_minority_index = local_minority_index = offset;
							master_group_index_of_index_array = j;
						} else {
							master_majority_index = local_majority_index = one_to_3_bin[i]->indexes[(j+1) % GROUP_SIZE];
							master_minority_index = local_minority_index = one_to_3_bin[i]->indexes[j];
							master_group_index_of_index_array = (j+1) % GROUP_SIZE;
						}
						local_majority_count += 3;
						local_minority_count += 1;
						break;
					}
				}
			} else {
				int a = 1;
				for (j = 0; j < GROUP_SIZE; j++) {
					temp_group = makegroup(one_to_3_bin[i]->indexes[0], one_to_3_bin[i]->indexes[1],
						one_to_3_bin[i]->indexes[2], one_to_3_bin[i]->indexes[3]);
					group temp_group2 = makegroup(one_to_3_bin[master_group_index]->indexes[0], 
						one_to_3_bin[master_group_index]->indexes[1], one_to_3_bin[master_group_index]->indexes[2], 
						one_to_3_bin[master_group_index]->indexes[3]);

					// cell replacement
					temp_group.indexes[j] = offset;
					temp_group2.indexes[master_group_index_of_index_array] = one_to_3_bin[i]->indexes[j];

					int status = QCOUNT(1, temp_group.indexes);
					if(status == EVEN_DIVIDE )
					{
						status = QCOUNT(1, temp_group2.indexes);
						local_majority_index = one_to_3_bin[i]->indexes[j]; // always true.
						local_minority_index = offset;
						if(status == EVEN_DIVIDE) {
							local_majority_count += 1;
							local_minority_count += 3;
						} else {
							local_majority_count += 3;
							local_minority_count += 1;
						}
						break;
					}
					else if(status == ALL_SAME)
					{
						local_minority_index = one_to_3_bin[i]->indexes[j];
						local_majority_index = one_to_3_bin[i]->indexes[(j + 1) % GROUP_SIZE];
						temp_group2.indexes[master_group_index_of_index_array] = local_majority_index;
						// does it match the majority.
						status = QCOUNT(1, temp_group2.indexes);
						if(status == EVEN_DIVIDE) {
							local_majority_count += 1;
							local_minority_count += 3;
						} else {
							local_majority_count += 3;
							local_minority_count += 1;
						}
						break;
					}
				}
			}
		}

		if (all_4_bin_size == 0) 
		{
			if (local_majority_count > local_minority_count)
				majority_index = master_majority_index;
			else if (local_majority_count < local_minority_count)
				majority_index = master_minority_index;
			else
				majority_index = 0;
		}
		else if (local_majority_count != local_minority_count) {
			// make sure our local majority matches the all by 4 majority.
			group temp_group = makegroup(master->indexes[0], master->indexes[1], master->indexes[2],
				master_majority_index);
			int status = QCOUNT(1, temp_group.indexes);
			if (status == ONE_DIFFERENT) {
				// they don't match
				majority += local_minority_count;
				minority += local_majority_count;

				if (majority < minority) {
					// swap
					majority_index = minority_index == -1 ? master_majority_index : minority_index;
				} else if (majority == minority) {
					// tie
					majority_index = 0;
				} 
			} else if (majority + local_majority_count == minority + local_minority_count) {
				// tie
				majority_index = 0;
			} else if (local_majority_count < local_minority_count && majority == minority) {
				// the local minority wins.
				majority_index = master_minority_index;
			} else if (minority + local_minority_count > majority + local_majority_count) {
				// majorities do match, BUT the sums are not even, and the local majority 
				majority_index = master_minority_index;
			}
		} else if (majority == minority) {
			// tie
			majority_index = 0;
		}
	} else if (one_to_3_bin_size == 0 && majority == minority) {
		majority_index = 0;
	}

	// handle when n is not divisible by 4
	if (n % GROUP_SIZE) {
		int index = majority_index;
		if (majority_index == 0) {
			index = (n - (n % GROUP_SIZE) - GROUP_SIZE) + 1;
		}
		status = doleftovers(index, n);
		if (status < 0) {

		} else if (status > 0) {
			
		}
	}
	return majority_index;
}
