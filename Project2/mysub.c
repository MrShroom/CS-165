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

typedef struct group_evener_t {
    int status;
    int majority;
    int minority;
} group_evener;

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

group_evener doleftovers(int majority_index, int n) {
    group g;
    group_evener evener;
    setgroup(&g, majority_index);
    printf("1=%d\t2=%d\t3=%d\t4=%d\n", g.indexes[0], g.indexes[1], g.indexes[2], g.indexes[3]);
    int i, next, remain = n % GROUP_SIZE, diff = 0;
    for (next = 0; next < remain; next++) {
        for (i = 0; i < GROUP_SIZE; i++) {
            int status, temp = g.indexes[i];
            g.indexes[i] = next + (n - (n % GROUP_SIZE)) + 1;
            status = QCOUNT(1, g.indexes); //Marking QCOUNT's 
            g.indexes[i] = temp;

            if (status == g.status) {
                if (status == ALL_SAME) {
                    // awesome. That was easy.
                    evener.majority = g.indexes[i];
                    evener.minority = -1;
                    diff += 1;
                    break;
                }
                else {
                    // we need a real difference.
                    continue;
                }
            }
            else {
                // we can count this item
                // towards whatever bin.
                if (status == ALL_SAME || (status == ONE_DIFFERENT && g.status == EVEN_DIVIDE)) {
                    diff += 1;
                    evener.majority = next + (n - (n % GROUP_SIZE)) + 1;
                    evener.minority = g.indexes[i];
                }
                else if (status == ONE_DIFFERENT || (status == EVEN_DIVIDE && g.status == ONE_DIFFERENT)) {
                    diff -= 1;
                    evener.minority = next + (n - (n % GROUP_SIZE)) + 1;
                    evener.majority = g.indexes[i];
                }
                break;
            }
        }
    }
    evener.status = diff;
    printf("status=%d\tmajority=%d\tminority=%d\n", diff, evener.majority, evener.minority);
    return evener;
}


int mysub(int n, int loop) {
    printf("\nloop %d :\n", loop);
    QCOUNT(-1);
    int majority = 0; // will eventually be >= n / 2 
    int minority = 0; //keep track in case we need to swap.
    int majority_index = 0;
    int minority_index = -1;
    int seen_a_all_4 = 0;
    int seen_a_3_to_1 = 0;

    // master group is the group that we compare others to check if they are in the majority
    // slave group is the group that we compare others to check if they are in the minority
    group *master = NULL, *slave = NULL;
    group *master_3_to_1 = NULL, *slave_3_to_1 = NULL;

    int local_majority_count = 0;
    int local_minority_count = 0;
    int local_majority_index = -2;//unknown yet
    int local_minority_index = -1;
    int index_into_master_group_marking_a_majority_element = -1;
    int master_majority_index = 0;
    int master_minority_index = 0;

    // loop over 0 to n - 4
    int i;
    for (i = 1; i <= n - (n % GROUP_SIZE); i += GROUP_SIZE)
    {
        group *current_group = (group*)malloc(sizeof(group));
        setgroup(current_group, i);// one hidden QCOUNT in set Group
        if (current_group->status == EVEN_DIVIDE)
        {
            free(current_group);
            majority += 2;
            minority += 2;

        }
        else if (current_group->status == ALL_SAME)
        {
            if (master == NULL) {
                // ASSUME master is majority, may swap later.
                master = current_group;
                majority_index = master->indexes[0];
                majority += 4;
                if (!seen_a_all_4 && seen_a_3_to_1)
                {
                    local_majority_index;
                    group group_to_split_3_to_1s = makegroup(master->indexes[0], master->indexes[1], master->indexes[2],
                        master_majority_index);
                    group_to_split_3_to_1s.status = QCOUNT(1, group_to_split_3_to_1s.indexes);
                    if (group_to_split_3_to_1s.status == ALL_SAME)
                    {
                        majority += local_majority_count;
                        minority += local_minority_count;
                    }
                    else
                    {
                        minority += local_majority_count;
                        majority += local_minority_count;
                    }
                }
            }
            else
            {
                group temp_group = makegroup(master->indexes[0], master->indexes[1], master->indexes[2],
                    current_group->indexes[0]);
                int status = QCOUNT(1, temp_group.indexes); //Marking QCOUNT's 
                if (status == ALL_SAME)
                {
                    majority += 4;
                }
                else
                {
                    minority += 4;
                    minority_index = current_group->indexes[0];
                    slave = current_group;
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
            seen_a_all_4 = 1;
        }
        else if (current_group->status == ONE_DIFFERENT) // This is the only place to reduce number of Qcounts
        {
            seen_a_3_to_1 = 1;
            if (!seen_a_all_4)
            {
                int offset = (current_group->indexes[3] + 1) % (n - (n % GROUP_SIZE));//next index in arrayafter end of bin
                group temp_group;
                int j;
                if (local_majority_index < 0)
                {
                    for (j = 0; j < GROUP_SIZE; j++) {
                        temp_group = makegroup(current_group->indexes[0], current_group->indexes[1],
                            current_group->indexes[2], current_group->indexes[3]);
                        temp_group.indexes[j] = offset;
                        int status = QCOUNT(1, temp_group.indexes);//Marking QCOUNT's 

                        if (status != ONE_DIFFERENT) {
                            master_3_to_1 = current_group;
                            if (status == EVEN_DIVIDE) {
                                master_majority_index = local_majority_index = current_group->indexes[j];
                                master_minority_index = local_minority_index = offset;
                                index_into_master_group_marking_a_majority_element = j;
                            }
                            else {
                                master_majority_index = local_majority_index = current_group->indexes[(j + 1) % GROUP_SIZE];
                                master_minority_index = local_minority_index = current_group->indexes[j];
                                index_into_master_group_marking_a_majority_element = (j + 1) % GROUP_SIZE;
                            }
                            local_majority_count += 3;
                            local_minority_count += 1;
                            break;
                        }
                    }
                }
                else {
                    for (j = 0; j < GROUP_SIZE; j++) {
                        temp_group = makegroup(current_group->indexes[0], current_group->indexes[1],
                            current_group->indexes[2], current_group->indexes[3]);
                        group temp_group2 = makegroup(master_3_to_1->indexes[0],
                            master_3_to_1->indexes[1], master_3_to_1->indexes[2],
                            master_3_to_1->indexes[3]);

                        // cell replacement
                        temp_group.indexes[j] = offset;
                        temp_group2.indexes[index_into_master_group_marking_a_majority_element] = current_group->indexes[j];

                        int status = QCOUNT(1, temp_group.indexes);//Marking QCOUNT's 
                        if (status == EVEN_DIVIDE)
                        {
                            status = QCOUNT(1, temp_group2.indexes); //Marking QCOUNT's 
                            local_majority_index = current_group->indexes[j]; // always true.
                            local_minority_index = offset;
                            if (status == EVEN_DIVIDE) {
                                local_majority_count += 1;
                                local_minority_count += 3;
                            }
                            else {
                                local_majority_count += 3;
                                local_minority_count += 1;
                            }

                            break;
                        }
                        else if (status == ALL_SAME)
                        {
                            local_minority_index = current_group->indexes[j];
                            local_majority_index = current_group->indexes[(j + 1) % GROUP_SIZE];
                            temp_group2.indexes[index_into_master_group_marking_a_majority_element] = local_majority_index;
                            // does it match the majority.
                            status = QCOUNT(1, temp_group2.indexes);//Marking QCOUNT's 
                            if (status == EVEN_DIVIDE) {
                                local_majority_count += 1;
                                local_minority_count += 3;
                            }
                            else {
                                local_majority_count += 3;
                                local_minority_count += 1;
                            }
                            break;
                        }

                    }
                }
                
                // early stopping, here because we have not seen a all same, majority come from even divides
                if (local_majority_count + majority > (n / 2))
                {
                    free(current_group);
                    return master_majority_index;
                }
            }
            else //we have a master ALL_SAME
            {
                group checker_group;
                int j;
                for (j = 0; j < GROUP_SIZE; j++) {
                    checker_group = makegroup(current_group->indexes[0], current_group->indexes[1],
                        current_group->indexes[2], current_group->indexes[3]);
                    checker_group.indexes[j] = majority_index;
                    checker_group.status = QCOUNT(1, checker_group.indexes);
                    if (checker_group.status == EVEN_DIVIDE)
                    {
                        minority += 3;
                        majority += 1;
                        minority_index = current_group->indexes[j];
                        break;
                    }
                    else if (checker_group.status == ALL_SAME)
                    {
                        majority += 3;
                        minority += 1;
                        minority_index = current_group->indexes[j];
                        break;
                    }
                    
                }
            }
        }
       
        // early stopping, here because we have not seen a all same, majority come from even divides
        if (!seen_a_all_4 && local_majority_count + majority > (n / 2))
        {
            return master_majority_index;
        }
        // early stopping, here because we have not seen a all same, majority come from even divides
        if (!seen_a_all_4 && local_minority_count + minority > (n / 2))
        {
            return master_minority_index;
        }
        if (majority > (n / 2))// early stopping
        {
            return majority_index;
        }
        if (minority > (n / 2))// early stopping
        {
            return minority_index;
        }
    }

    //if (seen_a_3_to_1) {
    //    if (!seen_a_all_4)
    //    {
    //        if (local_majority_count > local_minority_count)
    //        {
    //            majority += local_majority_count;
    //            minority += local_minority_count;
    //            majority_index = master_majority_index;
    //        }
    //        else if (local_majority_count < local_minority_count)
    //        {
    //            majority += local_minority_count;
    //            minority += local_majority_count;
    //            majority_index = master_minority_index;
    //        }
    //        else
    //        {
    //            majority += local_minority_count;
    //            minority += local_majority_count;
    //            majority_index = 0;
    //        }
    //    }
    //    else if (local_majority_count != local_minority_count) {
    //        // make sure our local majority matches the all by 4 majority.
    //        group temp_group = makegroup(master->indexes[0], master->indexes[1], master->indexes[2],
    //            master_majority_index);
    //        int status = QCOUNT(1, temp_group.indexes);//Marking QCOUNT's 
    //        if (status == ONE_DIFFERENT) {
    //            // they don't match
    //            majority += local_minority_count;
    //            minority += local_majority_count;

    //            if (majority < minority) {
    //                // swap         
    //                majority_index = minority_index == -1 ? master_majority_index : minority_index;
    //            }
    //            else if (majority == minority) {
    //                // tie
    //                majority_index = 0;
    //            }
    //        }
    //        else if (majority + local_majority_count == minority + local_minority_count) {
    //            // tie
    //            majority += local_majority_count;
    //            minority += local_minority_count;
    //            majority_index = 0;
    //        }
    //        else if (local_majority_count < local_minority_count && majority == minority) {
    //            // the local minority wins.
    //            majority += local_majority_count;
    //            minority += local_minority_count;
    //            majority_index = master_minority_index;
    //        }
    //        else if (minority + local_minority_count > majority + local_majority_count) {
    //            // majorities do match, BUT the sums are not even, and the local majority 
    //            majority += local_majority_count;
    //            minority += local_minority_count;
    //            majority_index = master_minority_index;
    //        }
    //    }
    //    else if (majority == minority) {
    //        // tie
    //        majority += local_majority_count;
    //        minority += local_minority_count;
    //        majority_index = 0;
    //    }
    //}
    //else if (majority == minority) {
    //    majority_index = 0;
    //}
    if (seen_a_all_4 && majority == minority )
    {
            majority_index = 0;
    }
    if (!seen_a_all_4 && seen_a_3_to_1 && local_majority_count == local_minority_count)
    {
        majority_index = 0;
    }
    if (!seen_a_all_4 && !seen_a_3_to_1)
    {
        majority_index = 0;
    }

    // handle when n is not divisible by 4
    if (n % GROUP_SIZE) {
        int index = majority_index;
        group_evener status;
        if (majority_index == 0) {
            index = (n - (n % GROUP_SIZE) - GROUP_SIZE) + 1;
        }
        printf("Index(before mutation)=%d\t", index);
        // bring index to power of four
        index -= (index - 1) % GROUP_SIZE;
        status = doleftovers(index, n);
        if (status.status < 0) {
            if (majority_index == 0) {
                majority_index = status.minority;
            }
        }
        else if (status.status > 0) {
            if (majority_index == 0) {
                majority_index = status.majority;
            }
        }
    }
    return majority_index;
}
