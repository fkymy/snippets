// Ranked Pairs https://en.wikipedia.org/wiki/Ranked_pairs
// Given candidate names as arg and voting info as stdin in the format of 'test',
// decide a winner of election based on Tideman algorithm, without graph node cycles (source of the graph is the winner)
// e.g.
// ./a.out aki nori yu < test
// nori

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX 9

typedef struct
{
	int winner;
	int loser;
}
pair;

int preferences[MAX][MAX];
bool locked[MAX][MAX];
char *candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];
int pair_count;
int candidate_count;

bool vote(int rank, char *name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool check_cycle(int loser, int p);
void print_winner(void);

int main(int argc, char *argv[])
{
	int voter_count;
	pair_count = 0;
	candidate_count = 0;
	if (argc < 2)
	{
		printf("Usage: tideman [candidate ...]\n");
		return 1;
	}
	candidate_count = argc - 1;
	if (candidate_count > MAX)
	{
		printf("Maximum number of candidates is %i\n", MAX);
		return 2;
	}
	for (int i = 0; i < candidate_count; i++)
	{
		candidates[i] = argv[i + 1];
		for (int j = 0; j < candidate_count; j++)
			locked[i][j] = false;
	} 
	scanf("%d\n", &voter_count);
	for (int i = 0; i < voter_count; i++)
	{
		int ranks[candidate_count];
		for (int j = 0; j < candidate_count; j++)
		{
			char name[20];
			if (j + 1 == candidate_count && i + 1 != voter_count)
				scanf("%s\n\n", name);
			else
				scanf("%s\n", name);
			if (!vote(j, name, ranks))
			{
				printf("Invalid vote.\n");
				return 3;
			}
		}
		record_preferences(ranks);
	}
	add_pairs();
	sort_pairs();
	lock_pairs();
	print_winner();
	return 0;
}

bool vote(int rank, char *name, int ranks[])
{
	for (int i = 0; i < candidate_count; i++)
	{
		if (strcmp(candidates[i], name) == 0 && strlen(candidates[i]) == strlen(name))
		{
			ranks[rank] = i;
			return true;
		}
	}
	return false;
}

void record_preferences(int ranks[])
{
	for (int i = 0; i < candidate_count; i++)
		for (int j = i + 1; j < candidate_count; j++)
			preferences[ranks[i]][ranks[j]] += 1;
	return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
	for (int i = 0; i < candidate_count; i++)
	{
		for (int j = 0; j < candidate_count; j++)
		{
			if (preferences[i][j] > preferences[j][i])
			{
				pairs[pair_count].winner = i;
				pairs[pair_count].loser = j;
				pair_count++;
			}
		}
	}
	return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
	int max_idx;
	int i;
	int j;

	for (i = 0; i < pair_count - 1; i++)
	{
		max_idx = i;
		for (j = i + 1; j < pair_count; j++)
		{
			if (preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[max_idx].winner][pairs[max_idx].loser])
				max_idx = j;
		}
		pair tmp;
		tmp = pairs[i];
		pairs[i] = pairs[max_idx];
		pairs[max_idx] = tmp;
	}
	return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
	for (int i = 0; i < pair_count; i++)
		if (!check_cycle(pairs[i].loser, i))
			locked[pairs[i].winner][pairs[i].loser] = true;
}

bool check_cycle(int loser, int p)
{
	if (loser == pairs[p].winner)
		return true;
	for (int i = 0; i < candidate_count; i++)
		if (locked[loser][i])
			return check_cycle(i, p);
	return false;
}

void iterative_lock_pairs(void)
{
	int is_loop;

	if (pair_count > 0)
		locked[pairs[0].winner][pairs[0].loser] = true;

	for (int i = 0; i < pair_count; i++)
	{
		int losers[candidate_count];
		int loser_count = 0;

		is_loop = 0;
		for (int j = 0; j < candidate_count; j++)
		{
			if (locked[pairs[i].loser][j] == true)
				losers[loser_count++] = j;
		}

		if (loser_count > 0)
		{
			for (int k = 0; k < loser_count; k++)
			{
				int loser;
				int l = 0;
				loser = losers[k];
				while (l < candidate_count)
				{
					// this is insufficient, what if it has multiple losers...
					if (locked[loser][l] == true)
					{
						loser = l;
						l = 0;
						continue ;
					}
					if (loser == pairs[i].winner)
						is_loop = 1;
					l++;
				}
			}
		}

		if (is_loop == 0)
			locked[pairs[i].winner][pairs[i].loser] = true;
	}
	return;
}

void print_winner(void)
{
	int is_winner;

	for (int i = 0; i < candidate_count - 1; i++)
	{
		is_winner = 1;
		for (int j = 0; j < candidate_count; j++)
		{
			if (locked[j][i] == true)
					is_winner = 0;
		}
		if (is_winner)
			printf("%s\n", candidates[i]);
	}
	return;
}
