#include <stdlib.h>
#include <string.h>
#define ALL_SAME       4
#define ONE_DIFFERENT  2
#define EVEN_DIVIDE    0
#define GROUP_SIZE     4
#define DEBUG
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

int mysub (int n) {
#ifdef DEBUG
	QCOUNT(-1);
#endif
	int majority = 0; // will eventually be >= n / 2 
	int minority = 0;
	int majority_index = 0;
	group **all_4_bin = (group**)malloc(sizeof(group*) * ((n + GROUP_SIZE)/GROUP_SIZE));
	int all_4_bin_size =0;
	group **one_to_3_bin = (group**)malloc(sizeof(group*) * ((n + GROUP_SIZE)/GROUP_SIZE));
	int one_to_3_bin_size =0;

	// loop over 0 to n - 4
	int i;
	for (i = 1; i <= n; i += GROUP_SIZE) {
		group *temp = (group*)malloc(sizeof(group));
		setgroup(temp, i);
		if(temp->status == EVEN_DIVIDE)
		{
#ifdef DEBUG
			printf("Setting %i to EVEN_DIVIDE\n", i);
#endif
			free(temp);
		}
		else if(temp->status == ALL_SAME)
		{
#ifdef DEBUG
			printf("Setting %i to ALL_SAME\n", i);
#endif
			all_4_bin[all_4_bin_size++] = temp;
		}
		else if(temp->status == ONE_DIFFERENT)
		{
#ifdef DEBUG
			printf("Setting %i to ONE_DIFFERENT\n", i);
#endif
			one_to_3_bin[one_to_3_bin_size++] = temp;
		}

	}
	group *master;
	// pick one group of 4 to be master group.
	if (all_4_bin_size > 0) {
		// ASSUME master is majority, may swap later.
		master = all_4_bin[0];
		majority_index = master->indexes[0];
		int minority_index = 0;
		majority = 4;
		for (i = 1; i < all_4_bin_size; i++) {
			group temp_group;
			temp_group.indexes[0] = master->indexes[0];
			temp_group.indexes[1] = master->indexes[1];
			temp_group.indexes[2] = master->indexes[2];
			temp_group.indexes[3] = all_4_bin[i]->indexes[0]; // index doesn't matter here.
			int status = QCOUNT(1, temp_group.indexes);
			if (status == ALL_SAME) majority += 4;
			else { 
				minority += 4;
				minority_index = all_4_bin[i]->indexes[0];
			}
		}

		if (minority > majority) {
			// swap
			int temp = minority;
			minority = majority;
			majority = temp;
			majority_index = minority_index;
			minority_index = majority_index;
		}
	}

	if (majority <= n / 2 && one_to_3_bin_size > 0) {
		int local_majority_count=0;
		int local_minority_count=0;
		int local_majority_index =-1;//unknown yet
		int local_minority_index = -1;
		int master_group_index = -1;
		int master_group_index_of_index_array = -1;
		for (i = 0; i < one_to_3_bin_size; i++) 
		{
			int offset = one_to_3_bin[i]->indexes[3]+1 % n;
			group temp_group;
			int j;
			for (j = 0; j < GROUP_SIZE; j++) {
				temp_group.indexes[0] = one_to_3_bin[i]->indexes[0];
				temp_group.indexes[1] = one_to_3_bin[i]->indexes[1];
				temp_group.indexes[2] = one_to_3_bin[i]->indexes[2];
				temp_group.indexes[3] = one_to_3_bin[i]->indexes[3];
				temp_group.indexes[j] = offset;
				int status = QCOUNT(1, temp_group.indexes);

				if (local_majority_index == -1 && status != ONE_DIFFERENT) {
					local_majority_index = one_to_3_bin[i]->indexes[j];
					master_group_index = i;
					master_group_index_of_index_array = j;
					if (status == EVEN_DIVIDE) {
						local_majority_count += 3;
						local_minority_count += 1;
					} else {
						local_majority_count += 1;
						local_minority_count += 3;
					}
				} else {
					group temp_group2;
					temp_group2.indexes[0] = one_to_3_bin[master_group_index]->indexes[0];
					temp_group2.indexes[1] = one_to_3_bin[master_group_index]->indexes[1];
					temp_group2.indexes[2] = one_to_3_bin[master_group_index]->indexes[2];
					temp_group2.indexes[3] = one_to_3_bin[master_group_index]->indexes[3];
					temp_group2.indexes[master_group_index_of_index_array] = one_to_3_bin[i]->indexes[j];

					if(status == EVEN_DIVIDE )
					{
						// does it match the majority.
						if(QCOUNT(1, temp_group2.indexes) == EVEN_DIVIDE) {
							local_majority_count += 3;
							local_minority_count += 1;
						} else {
							local_majority_count += 1;
							local_minority_count += 3;
						}
						break;
					}
					else if(status == ALL_SAME)
					{
						local_minority_index = one_to_3_bin[i]->indexes[j];
						// does it match the majority.
						if(QCOUNT(1, temp_group2.indexes) == EVEN_DIVIDE) {
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

		if (local_minority_count > local_majority_index) {
			// swap
			int temp = local_minority_count;
			local_minority_count = local_majority_index;
			local_majority_index = temp;
			local_majority_index = local_minority_index;
		}

		if (all_4_bin_size == 0) majority_index = local_majority_index;
		else {
			// make sure our local majority matches the all by 4 majority.
			group temp_group;
			temp_group.indexes[0] = master->indexes[0];
			temp_group.indexes[1] = master->indexes[1];
			temp_group.indexes[2] = master->indexes[2];
			temp_group.indexes[3] = local_majority_index; 
			if (QCOUNT(1, temp_group.indexes) == ONE_DIFFERENT) {
				// they don't match
				majority += local_minority_count;
				minority += local_majority_count;

				if (majority < minority) {
					// swap
					
					majority_index = local_minority_index;
				}
				else if (majority == minority)
				{
					majority_index = 0;
				}
			}
		}
	} 

#ifdef DEBUG
	printf("Majority count = %d\nMinority count = %d\n", majority, minority);
#endif // DEBUG

	return majority_index;
}






