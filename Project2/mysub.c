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

void memFreer(group* master_all_same, group* master_3_to_1)
{
    if (master_all_same != NULL)
        free(master_all_same);

    if (master_3_to_1 != NULL)
        free(master_3_to_1);
}

int mysub(int n) {
    int complete_A_count = 0; // will eventually be >= n / 2 
    int complete_B_count = 0; 
    int group_A_index = 0;
    int group_B_index = -1; //negitve one means not seen yet
    int seen_a_all_4 = 0;
    int seen_a_3_to_1 = 0;

    // master groups are groups that we compare others to. 
    group *master_all_same = NULL;
    group *master_3_to_1 = NULL;

    int group_A_3_to_1_count = 0; //count of only 3 to 1 used until we find a all same group
    int group_B_3_to_1_count = 0;
    int group_A_3_to_1_index = -1; //negitive numbers imply unissaigned indexes
    int group_B_3_to_1_index = -1;
    int index_marker = -1; //index into 3 to 1 master gropup marking a majority element
    int master_group_A_3_to_1_index = 0;
    int master_group_B_3_to_1_index = 0;

    // loop over 0 to n - 4
    int i;
    for (i = 1; i <= n - (n % GROUP_SIZE); i += GROUP_SIZE)
    {
        group *current_group = (group*)malloc(sizeof(group));
        setgroup(current_group, i);// one hidden QCOUNT in set Group -- WC happens O(n/4) times
        if (current_group->status == EVEN_DIVIDE)
        {
            complete_A_count += 2;
            complete_B_count += 2;
        }
        else if (current_group->status == ALL_SAME)
        {
            if (master_all_same == NULL) {
                master_all_same = current_group;
                group_A_index = master_all_same->indexes[0];
                complete_A_count += 4;

                //we must add in the 3 to 1 counts up to this point.
                if (!seen_a_all_4 && seen_a_3_to_1)
                {
                    group group_to_split_3_to_1s = 
                        makegroup(master_all_same->indexes[0], master_all_same->indexes[1], master_all_same->indexes[2], master_group_A_3_to_1_index);
                    group_to_split_3_to_1s.status = QCOUNT(1, group_to_split_3_to_1s.indexes); // QCOUNT -- WC happens 1 time O(k)

                    if (group_to_split_3_to_1s.status == ALL_SAME)
                    {
                        complete_A_count += group_A_3_to_1_count;
                        complete_B_count += group_B_3_to_1_count;
						group_B_index = master_group_B_3_to_1_index;
                    }
                    else
                    {
                        complete_B_count += group_A_3_to_1_count;
                        complete_A_count += group_B_3_to_1_count;
						group_B_index = master_group_A_3_to_1_index;
                    }
                }
            }
            else //we have a master. Use it to find where this group counts to
            {
                group check = makegroup(master_all_same->indexes[0], master_all_same->indexes[1], master_all_same->indexes[2], current_group->indexes[0]);
                check.status = QCOUNT(1, check.indexes); //Marking QCOUNT's --- WC happens (n/4 -1) times - O(n/4)
                
                if (check.status == ALL_SAME)
                {
                    complete_A_count += 4;
                }
                else
                {
                    complete_B_count += 4;
                    group_B_index = current_group->indexes[0];
                }
                free(current_group); //done with group 
                current_group = NULL;
            }
            seen_a_all_4 = 1;//mark that we've seem a all same group
        }
        else if (current_group->status == ONE_DIFFERENT) 
        {
            if (!seen_a_all_4) //if we don't have master all same group we have to use a 3 to 1 group
            {
                int offset = (current_group->indexes[3] + 1) % (n - (n % GROUP_SIZE));//next index in arrayafter end of bin
                group check;
                int j;
                if (group_A_3_to_1_index < 0) //set up our 3 to 1 master
                {
                    for (j = 0; j < GROUP_SIZE; j++) {
                        check = makegroup(current_group->indexes[0], current_group->indexes[1],
                            current_group->indexes[2], current_group->indexes[3]);
                        check.indexes[j] = offset;
                        check.status = QCOUNT(1, check.indexes);//Marking QCOUNT's  -- WC 3 times O(k)

                        if (check.status != ONE_DIFFERENT) {
                            master_3_to_1 = current_group;
                            if (check.status == EVEN_DIVIDE) {
                                master_group_A_3_to_1_index = group_A_3_to_1_index = current_group->indexes[j];
                                master_group_B_3_to_1_index = group_B_3_to_1_index = offset;
                                index_marker = j;
                            }
                            else {
                                master_group_A_3_to_1_index = group_A_3_to_1_index = current_group->indexes[(j + 1) % GROUP_SIZE];
                                master_group_B_3_to_1_index = group_B_3_to_1_index = current_group->indexes[j];
                                index_marker = (j + 1) % GROUP_SIZE;
                            }
                            group_A_3_to_1_count += 3;
                            group_B_3_to_1_count += 1;
                            break;
                        }
                    }
                }
                else //use our 3 to 1 master to check other counts 
                {
                    for (j = 0; j < GROUP_SIZE; j++) {
                        check = makegroup(current_group->indexes[0], current_group->indexes[1],
                            current_group->indexes[2], current_group->indexes[3]);
                        group check_against_master = makegroup(master_3_to_1->indexes[0],
                            master_3_to_1->indexes[1], master_3_to_1->indexes[2],
                            master_3_to_1->indexes[3]);

                        // cell replacement
                        check.indexes[j] = offset;
                        check_against_master.indexes[index_marker] = current_group->indexes[j];

                        check.status = QCOUNT(1, check.indexes);//Marking QCOUNT's -- WC happens at most n/4 - 1 times --- O(n/4)
                        if (check.status == EVEN_DIVIDE)
                        {
                            check_against_master.status = QCOUNT(1, check_against_master.indexes); //Marking QCOUNT's -- WC happens at most 4 * (n/4 -1)  times --- O(n)
                            group_A_3_to_1_index = current_group->indexes[j]; // always true.
                            group_B_3_to_1_index = offset;
                            if (check_against_master.status == EVEN_DIVIDE) {
                                group_A_3_to_1_count += 1;
                                group_B_3_to_1_count += 3;
                            }
                            else {
                                group_A_3_to_1_count += 3;
                                group_B_3_to_1_count += 1;
                            }

                            break;
                        }
                        else if (check.status == ALL_SAME)
                        {
                            group_B_3_to_1_index = current_group->indexes[j];
                            group_A_3_to_1_index = current_group->indexes[(j + 1) % GROUP_SIZE];
                            check_against_master.indexes[index_marker] = group_A_3_to_1_index;
                            // does it match the majority.
                            check_against_master.status = QCOUNT(1, check_against_master.indexes);//Marking QCOUNT's -- WC happens at most 4 * (n/4 -1)  times --- O(n)
                            if (check_against_master.status == EVEN_DIVIDE) {
                                group_A_3_to_1_count += 1;
                                group_B_3_to_1_count += 3;
                            }
                            else {
                                group_A_3_to_1_count += 3;
                                group_B_3_to_1_count += 1;
                            }
                            break;
                        }

                    }
                    free(current_group);
                    current_group = NULL;
                }
            }
            else //we have a master ALL_SAME
            {
                group checker;
                int j;
                for (j = 0; j < GROUP_SIZE; j++) {
                    checker = makegroup(current_group->indexes[0], current_group->indexes[1], current_group->indexes[2], current_group->indexes[3]);
                    checker.indexes[j] = group_A_index;
                    checker.status = QCOUNT(1, checker.indexes); //QCOUNT --WC happens at most (n / 4 - 1)  times-- - O(n/4)
                    if (checker.status == EVEN_DIVIDE)
                    {
                        complete_B_count += 3;
                        complete_A_count += 1;
                        group_B_index = current_group->indexes[j];
                        break;
                    }
                    else if (checker.status == ALL_SAME)
                    {
                        complete_A_count += 3;
                        complete_B_count += 1;
                        group_B_index = current_group->indexes[j];
                        break;
                    }

                }
            }
            seen_a_3_to_1 = 1;
        }
       
        // early stopping, here because we have not seen a all same, majority come from even divides
        if (!seen_a_all_4 && group_A_3_to_1_count + complete_A_count > (n / 2))
        {
            memFreer(master_all_same, master_3_to_1);
            return master_group_A_3_to_1_index;
        }
        // early stopping, here because we have not seen a all same, majority come from even divides
        if (!seen_a_all_4 && group_B_3_to_1_count + complete_B_count > (n / 2))
        {
            memFreer(master_all_same, master_3_to_1);
            return master_group_B_3_to_1_index;
        }
        if (complete_A_count > (n / 2))// early stopping
        {
            memFreer(master_all_same, master_3_to_1);
            return group_A_index;
        }
        if (complete_B_count > (n / 2))// early stopping
        {
            memFreer(master_all_same, master_3_to_1);
            return group_B_index;
        }
        
    }
    
    if (n % GROUP_SIZE) {
        if (seen_a_all_4) {
            for (; i <= n; i++)  
            {
                group check = makegroup(master_all_same->indexes[0], master_all_same->indexes[1], master_all_same->indexes[2], i);
                check.status = QCOUNT(1, check.indexes); //Marking QCOUNT's -- WC happens 3 times --O(k)

                if (check.status == ALL_SAME)
                {
                    complete_A_count += 1;
                }
                else
                {
                    complete_B_count += 1;
                }
           }
        }
        else if (seen_a_3_to_1) {
            for (; i <= n; i++) {
                int j;
                for (j = 0; j < GROUP_SIZE; j++) {
                    group checker = makegroup(master_3_to_1->indexes[0],
                        master_3_to_1->indexes[1], master_3_to_1->indexes[2],
                        master_3_to_1->indexes[3]);

                    // cell replacement
                    checker.indexes[j] = i;
                    checker.status = QCOUNT(1, checker.indexes);//Marking QCOUNT's -- WC happens 12 times --O(k)
                    if (checker.status == EVEN_DIVIDE)
                    {
                        group_B_3_to_1_count++;
                        break;
                    }
                    else if (checker.status == ALL_SAME)
                    {
                        group_A_3_to_1_count++;
                        break;
                    }

                }
            }
        } 
        else {
            int check_group_start_index = n - (n % GROUP_SIZE) - GROUP_SIZE;
            group checker = makegroup(check_group_start_index + 1, check_group_start_index + 2, check_group_start_index + 3, check_group_start_index + 4);
            for (; i <= n; i++) {
                checker.indexes[GROUP_SIZE - 1] = i;
                checker.status = QCOUNT(1, checker.indexes); //Marking QCOUNT's -- WC happens 12 times --O(k)
                if (checker.status != EVEN_DIVIDE) {
                    complete_A_count++;
                    group_A_index = i;
                }
                else {
                    complete_B_count++;
                    group_B_index = i;
                }
            }
        }
    }

    if (!seen_a_all_4 && seen_a_3_to_1) {
        complete_A_count += group_A_3_to_1_count;
        complete_B_count += group_B_3_to_1_count;
        group_A_index = master_group_A_3_to_1_index;
        group_B_index = master_group_B_3_to_1_index;
    }

    memFreer(master_all_same, master_3_to_1, NULL);
    if (complete_A_count == complete_B_count )
    {
         return 0;
    }
       
    return complete_A_count > complete_B_count ? group_A_index : group_B_index;
}
