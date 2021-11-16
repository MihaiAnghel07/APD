#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "genetic_algorithm.h"


int read_input(sack_object **objects, int *object_count, int *sack_capacity, int *generations_count, int argc, char *argv[])
{
	FILE *fp;

	if (argc < 4) {
		fprintf(stderr, "Usage:\n\t./tema1_par in_file generations_count P\n");
		return 0;
	}

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		return 0;
	}

	if (fscanf(fp, "%d %d", object_count, sack_capacity) < 2) {
		fclose(fp);
		return 0;
	}

	if (*object_count % 10) {
		fclose(fp);
		return 0;
	}

	sack_object *tmp_objects = (sack_object *) calloc(*object_count, sizeof(sack_object));

	for (int i = 0; i < *object_count; ++i) {
		if (fscanf(fp, "%d %d", &tmp_objects[i].profit, &tmp_objects[i].weight) < 2) {
			free(objects);
			fclose(fp);
			return 0;
		}
	}

	fclose(fp);

	*generations_count = (int) strtol(argv[2], NULL, 10);
	
	if (*generations_count == 0) {
		free(tmp_objects);

		return 0;
	}

	*objects = tmp_objects;

	return 1;
}

void initialize_data(individual *current_generation, individual *next_generation, int object_count, int i)
{
	current_generation[i].fitness = 0;
	current_generation[i].chromosomes = (int*) calloc(object_count, sizeof(int));
	current_generation[i].chromosomes[i] = 1;
	current_generation[i].index = i;
	current_generation[i].chromosome_length = object_count;

	next_generation[i].fitness = 0;
	next_generation[i].chromosomes = (int*) calloc(object_count, sizeof(int));
	next_generation[i].index = i;
	next_generation[i].chromosome_length = object_count;

	current_generation[i + 1].fitness = 0;
	current_generation[i + 1].chromosomes = (int*) calloc(object_count, sizeof(int));
	current_generation[i + 1].chromosomes[i] = 1;
	current_generation[i + 1].index = i + 1;
	current_generation[i + 1].chromosome_length = object_count;

	next_generation[i + 1].fitness = 0;
	next_generation[i + 1].chromosomes = (int*) calloc(object_count, sizeof(int));
	next_generation[i + 1].index = i + 1;
	next_generation[i + 1].chromosome_length = object_count;

	current_generation[i + 2].fitness = 0;
	current_generation[i + 2].chromosomes = (int*) calloc(object_count, sizeof(int));
	current_generation[i + 2].chromosomes[i] = 1;
	current_generation[i + 2].index = i + 2;
	current_generation[i + 2].chromosome_length = object_count;

	next_generation[i + 2].fitness = 0;
	next_generation[i + 2].chromosomes = (int*) calloc(object_count, sizeof(int));
	next_generation[i + 2].index = i + 2;
	next_generation[i + 2].chromosome_length = object_count;

	current_generation[i + 3].fitness = 0;
	current_generation[i + 3].chromosomes = (int*) calloc(object_count, sizeof(int));
	current_generation[i + 3].chromosomes[i] = 1;
	current_generation[i + 3].index = i + 3;
	current_generation[i + 3].chromosome_length = object_count;

	next_generation[i + 3].fitness = 0;
	next_generation[i + 3].chromosomes = (int*) calloc(object_count, sizeof(int));
	next_generation[i + 3].index = i + 3;
	next_generation[i + 3].chromosome_length = object_count;

	current_generation[i + 4].fitness = 0;
	current_generation[i + 4].chromosomes = (int*) calloc(object_count, sizeof(int));
	current_generation[i + 4].chromosomes[i] = 1;
	current_generation[i + 4].index = i + 4;
	current_generation[i + 4].chromosome_length = object_count;

	next_generation[i + 4].fitness = 0;
	next_generation[i + 4].chromosomes = (int*) calloc(object_count, sizeof(int));
	next_generation[i + 4].index = i + 4;
	next_generation[i + 4].chromosome_length = object_count;

	current_generation[i + 5].fitness = 0;
	current_generation[i + 5].chromosomes = (int*) calloc(object_count, sizeof(int));
	current_generation[i + 5].chromosomes[i] = 1;
	current_generation[i + 5].index = i + 5;
	current_generation[i + 5].chromosome_length = object_count;

	next_generation[i + 5].fitness = 0;
	next_generation[i + 5].chromosomes = (int*) calloc(object_count, sizeof(int));
	next_generation[i + 5].index = i + 5;
	next_generation[i + 5].chromosome_length = object_count;

	current_generation[i + 6].fitness = 0;
	current_generation[i + 6].chromosomes = (int*) calloc(object_count, sizeof(int));
	current_generation[i + 6].chromosomes[i] = 1;
	current_generation[i + 6].index = i + 6;
	current_generation[i + 6].chromosome_length = object_count;

	next_generation[i + 6].fitness = 0;
	next_generation[i + 6].chromosomes = (int*) calloc(object_count, sizeof(int));
	next_generation[i + 6].index = i + 6;
	next_generation[i + 6].chromosome_length = object_count;

	current_generation[i + 7].fitness = 0;
	current_generation[i + 7].chromosomes = (int*) calloc(object_count, sizeof(int));
	current_generation[i + 7].chromosomes[i] = 1;
	current_generation[i + 7].index = i + 7;
	current_generation[i + 7].chromosome_length = object_count;

	next_generation[i + 7].fitness = 0;
	next_generation[i + 7].chromosomes = (int*) calloc(object_count, sizeof(int));
	next_generation[i + 7].index = i + 7;
	next_generation[i + 7].chromosome_length = object_count;

	current_generation[i + 8].fitness = 0;
	current_generation[i + 8].chromosomes = (int*) calloc(object_count, sizeof(int));
	current_generation[i + 8].chromosomes[i] = 1;
	current_generation[i + 8].index = i + 8;
	current_generation[i + 8].chromosome_length = object_count;

	next_generation[i + 8].fitness = 0;
	next_generation[i + 8].chromosomes = (int*) calloc(object_count, sizeof(int));
	next_generation[i + 8].index = i + 8;
	next_generation[i + 8].chromosome_length = object_count;

	current_generation[i + 9].fitness = 0;
	current_generation[i + 9].chromosomes = (int*) calloc(object_count, sizeof(int));
	current_generation[i + 9].chromosomes[i] = 1;
	current_generation[i + 9].index = i + 9;
	current_generation[i + 9].chromosome_length = object_count;

	next_generation[i + 9].fitness = 0;
	next_generation[i + 9].chromosomes = (int*) calloc(object_count, sizeof(int));
	next_generation[i + 9].index = i + 9;
	next_generation[i + 9].chromosome_length = object_count;
}

void compute_wight_and_profit(int *weight, int *profit,  individual generation, sack_object *objects, int j)
{
	if (generation.chromosomes[j]) {
		weight[0] += objects[j].weight;
		profit[0] += objects[j].profit;
	}
	if (generation.chromosomes[j + 1]) {
		weight[0] += objects[j + 1].weight;
		profit[0] += objects[j + 1].profit;
	}
	if (generation.chromosomes[j + 2]) {
		weight[0] += objects[j + 2].weight;
		profit[0] += objects[j + 2].profit;
	}
	if (generation.chromosomes[j + 3]) {
		weight[0] += objects[j + 3].weight;
		profit[0] += objects[j + 3].profit;
	}
	if (generation.chromosomes[j + 4]) {
		weight[0] += objects[j + 4].weight;
		profit[0] += objects[j + 4].profit;
	}
	if (generation.chromosomes[j + 5]) {
		weight[0] += objects[j + 5].weight;
		profit[0] += objects[j + 5].profit;
	}
	if (generation.chromosomes[j + 6]) {
		weight[0] += objects[j + 6].weight;
		profit[0] += objects[j + 6].profit;
	}
	if (generation.chromosomes[j + 7]) {
		weight[0] += objects[j + 7].weight;
		profit[0] += objects[j + 7].profit;
	}
	if (generation.chromosomes[j + 8]) {
		weight[0] += objects[j + 8].weight;
		profit[0] += objects[j + 8].profit;
	}
	if (generation.chromosomes[j + 9]) {
		weight[0] += objects[j + 9].weight;
		profit[0] += objects[j + 9].profit;
	}
}

int min(int a, int b) {
    return a < b ? a : b;
}

int compute_start(input in, int limit)
{
	return in.thread_id * ceil(limit / in.P);
}

int compute_end(input in, int limit)
{
	return min((in.thread_id + 1) * ceil(limit / in.P), limit);
}

int check(int end, input in, int limit)
{
	 if (end == 0 || (in.thread_id == in.P - 1 && end != limit))
	 	return 1;

	 return 0;
}

void print_objects(const sack_object *objects, int object_count)
{
	for (int i = 0; i < object_count; ++i) {
		printf("%d %d\n", objects[i].weight, objects[i].profit);
	}
}

void print_generation(const individual *generation, int limit)
{
	for (int i = 0; i < limit; ++i) {
		for (int j = 0; j < generation[i].chromosome_length; ++j) {
			printf("%d ", generation[i].chromosomes[j]);
		}

		printf("\n%d - %d\n", i, generation[i].fitness);
	}
}

void print_best_fitness(const individual *generation)
{
	printf("%d\n", generation[0].fitness);
}

void compute_fitness_function(input in, sack_object *objects, individual *generation, int object_count, int sack_capacity)
{
	int *weight = (int*)calloc(1, sizeof(int));
	int *profit = (int*)calloc(1, sizeof(int));
	
	int start = compute_start(in, object_count);
    int end = compute_end(in, object_count);
    if (check(end, in, object_count)) {
    	end = object_count;
    }
    // printf("start = %d , end = %d,   thread_id = %d\n", start, end, in.thread_id);
	for (int i = start; i < end; ++i) {
		weight[0] = 0;
		profit[0] = 0;
		
		for (int j = 0; j < generation[i].chromosome_length; j = j + 10) {
			//if (generation[i].chromosomes[j]) {
			//	weight[0] += objects[j].weight;
			//	profit[0] += objects[j].profit;
				compute_wight_and_profit(weight, profit, generation[i], objects, j);
			//}
		}

		generation[i].fitness = (weight[0] <= sack_capacity) ? profit[0] : 0;
	}

	free(weight);
	free(profit);
}

int cmpfunc(const void *a, const void *b)
{
	int i;
	individual *first = (individual *) a;
	individual *second = (individual *) b;

	int res = second->fitness - first->fitness; // decreasing by fitness
	if (res == 0) {
		int first_count = 0, second_count = 0;

		for (i = 0; i < first->chromosome_length && i < second->chromosome_length; ++i) {
			first_count += first->chromosomes[i];
			second_count += second->chromosomes[i];
		}

		res = first_count - second_count; // increasing by number of objects in the sack
		if (res == 0) {
			return second->index - first->index;
		}
	}

	return res;
}

void mutate_bit_string_1(input in, const individual *ind, int generation_index)
{
	int i, mutation_size;
	int step = 1 + generation_index % (ind->chromosome_length - 2);

	if (ind->index % 2 == 0) {
		// for even-indexed individuals, mutate the first 40% chromosomes by a given step
		mutation_size = ind->chromosome_length * 4 / 10;
		for (i = 0; i < mutation_size; i += step) {
			ind->chromosomes[i] = 1 - ind->chromosomes[i];
		}
	} else {
		// for even-indexed individuals, mutate the last 80% chromosomes by a given step
		mutation_size = ind->chromosome_length * 8 / 10;
		for (i = ind->chromosome_length - mutation_size; i < ind->chromosome_length; i += step) {
			ind->chromosomes[i] = 1 - ind->chromosomes[i];
		}
	}
}

void mutate_bit_string_2(input in, const individual *ind, int generation_index)
{
	int step = 1 + generation_index % (ind->chromosome_length - 2);

	// mutate all chromosomes by a given step
	for (int i = 0; i < ind->chromosome_length; i += step) {
		ind->chromosomes[i] = 1 - ind->chromosomes[i];
	}
}

void crossover(individual *parent1, individual *child1, int generation_index)
{
	individual *parent2 = parent1 + 1;
	individual *child2 = child1 + 1;
	int count = 1 + generation_index % parent1->chromosome_length;

	memcpy(child1->chromosomes, parent1->chromosomes, count * sizeof(int));
	memcpy(child1->chromosomes + count, parent2->chromosomes + count, (parent1->chromosome_length - count) * sizeof(int));

	memcpy(child2->chromosomes, parent2->chromosomes, count * sizeof(int));
	memcpy(child2->chromosomes + count, parent1->chromosomes + count, (parent1->chromosome_length - count) * sizeof(int));
}

void copy_individual(const individual *from, const individual *to)
{
	memcpy(to->chromosomes, from->chromosomes, from->chromosome_length * sizeof(int));
}

void free_generation(input in, individual *generation, individual *generation2)
{
	int i;
	int start = compute_start(in, generation->chromosome_length);
    int end = compute_end(in, generation->chromosome_length);

	for (i = start; i < end; ++i) {
		free(generation[i].chromosomes);
		free(generation2[i].chromosomes);
		generation[i].chromosomes = NULL;
		generation[i].fitness = 0;
		generation2[i].chromosomes = NULL;
		generation2[i].fitness = 0;
	}
}

void *run_genetic_algorithm(void *arg)
{

	input in = *(input*)arg;
	sack_object *objects = in.objects;
	int object_count = in.object_count;
	int generations_count = in.generations_count;
	int sack_capacity = in.sack_capacity;
	int thread_id = in.thread_id;
	static int flag = 0;

	int count, cursor;
	individual *current_generation = in.current_generation;
	individual *next_generation = in.next_generation;
	individual *tmp = NULL;

	int start = compute_start(in, object_count);
    int end = compute_end(in, object_count);
    if (check(end, in, object_count)) {
    	end = object_count;
    }
	//printf("start = %d,  end = %d\n",start, end);
	// set initial generation (composed of object_count individuals with a single item in the sack)
	for (int i = start; i < end; i++) {
		current_generation[i].fitness = 0;
		current_generation[i].chromosomes = (int*) calloc(object_count, sizeof(int));
		current_generation[i].chromosomes[i] = 1;
		current_generation[i].index = i;
		current_generation[i].chromosome_length = object_count;

		next_generation[i].fitness = 0;
		next_generation[i].chromosomes = (int*) calloc(object_count, sizeof(int));
		next_generation[i].index = i;
		next_generation[i].chromosome_length = object_count;
		// initialize_data(current_generation, next_generation, object_count, i);
	}
	
	// iterate for each generation
	for (int k = 0; k < generations_count; ++k) {
		cursor = 0;
		//pthread_barrier_wait(in.barrier);
		compute_fitness_function(in, objects, current_generation, object_count, sack_capacity);
		
		// sortarea trebuie sa se faca doar o data, abia dupa ce s-a terminat
		// calcularea fittnes-ului fiecarui individ din generatie!!
		pthread_barrier_wait(in.barrier);
		
		// se face sortarea doar de catre un thread
		pthread_mutex_lock(in.mutex);
		if (flag == k) {
			//printf("Sortarea este facuta de catre: thread_id = %d\n", thread_id);
			qsort(current_generation, object_count, sizeof(individual), cmpfunc);
			flag++;
		}
		pthread_mutex_unlock(in.mutex);

		// keep first 30% children (elite children selection)
		count = object_count * 3 / 10;
		int start2 = compute_start(in, count);
		int end2 = compute_end(in, count);
		if (check(end2, in, count)) {
    		end2 = count;
    	}
    	//pentru situatia cand sunt mai multe thread-uri
    	// decat task-uri de executat
    	if (start2 == 0 && end2 == count && in.P > 1) {
    		start2 = thread_id;
			end2 = start2 + 1;
    	}
    	if (end2 <= count) {
    		//printf("start2 = %d   end2 = %d  thread_id = %d\n", start2, end2, thread_id);
			for (int i = start2; i < end2; ++i) {
				copy_individual(current_generation + i, next_generation + i);
			}
		}
		cursor = count;
		
		// mutate first 20% children with the first version of bit string mutation
		count = object_count * 2 / 10;
		int start3 = compute_start(in, count);
		int end3 = compute_end(in, count);
		if (check(end3, in, count)) {
	    	end3 = count;
	    }
	    // pentru situatia cand sunt mai multe thread-uri
    	// decat task-uri de executat
    	if (start3 == 0 && end3 == count && in.P > 1) {
    		start3 = thread_id;
			end3 = start3 + 1;
    	}
		// pthread_barrier_wait(in.barrier);
		if (end3 <= count) {
			//printf("start3 = %d   end3 = %d\n", start3, end3);
			for (int i = start3; i < end3; ++i) {
				copy_individual(current_generation + i, next_generation + cursor + i);
				mutate_bit_string_1(in, next_generation + cursor + i, k);
			}
		}
		cursor += count;

		// mutate next 20% children with the second version of bit string mutation
		count = object_count * 2 / 10;
		int start4 = compute_start(in, count);
		int end4 = compute_end(in, count);
		if (check(end4, in, count)) {
	    	end4 = count;
	    }
	    if (start4 == 0 && end4 == count && in.P > 1) {
    		start4 = thread_id;
			end4 = start4 + 1;
    	}
		//pthread_barrier_wait(in.barrier);
		if (end4 <= count) {
			//printf("start4 = %d   end4 = %d\n", start4, end4);
			for (int i = start4; i < end4; ++i) {
				copy_individual(current_generation + i + count, next_generation + cursor + i);
				mutate_bit_string_2(in, next_generation + cursor + i, k);
			}
		}
		cursor += count;

		// crossover first 30% parents with one-point crossover
		// (if there is an odd number of parents, the last one is kept as such)
		count = object_count * 3 / 10;
		//printf("cursor = %d   count = %d\n", cursor, count);
		if (count % 2 == 1) {
			copy_individual(current_generation + object_count - 1, next_generation + cursor + count - 1);
			count--;
		}

		int start5 = compute_start(in, count);
		int end5 = compute_end(in, count);
		if (check(end5, in, count)) {
	    	end5 = count;
	    }
	    if (start5 == 0 && end5 == count && in.P > 1) {
    		start5 = thread_id;
			end5 = start5 + 1;
    	}
    	if (start5 % 2 != 0) {
    		start5++;
    	}
    	if (end5 <= count) {
			//pthread_mutex_lock(in.mutex);
			for (int i = start5; i < end5; i += 2) {
				//printf("i = %d,  start5 = %d,   end5 = %d  thread_id = %d\n",i, start5, end5, thread_id);
				crossover(current_generation + i, next_generation + cursor + i, k);
			}
			//pthread_mutex_unlock(in.mutex);
		}
		
		pthread_barrier_wait(in.barrier);
		// switch to new generation
		//pthread_mutex_lock(in.mutex);
		if (flag == k + 1) {
			tmp = current_generation;
			current_generation = next_generation;
			next_generation = tmp;
		}
		//pthread_mutex_unlock(in.mutex);
		//pthread_barrier_wait(in.barrier);
		int start6 = compute_start(in, object_count);
   		int end6 = compute_end(in, object_count);
   		if (check(end6, in, object_count)) {
	    	end6 = object_count;
	    }	    	
		for (int i = start6; i < end6; ++i) {
			current_generation[i].index = i;
		}

		if (k % 5 == 0 && thread_id == 0) {
			print_best_fitness(current_generation);
		}
	}

	//pthread_barrier_wait(in.barrier);
	compute_fitness_function(in, objects, current_generation, object_count, sack_capacity);
	if (thread_id == 0) {
		qsort(current_generation, object_count, sizeof(individual), cmpfunc);
		print_best_fitness(current_generation);
	}
		
	pthread_barrier_wait(in.barrier);
	free_generation(in, current_generation, next_generation);
	pthread_exit(NULL);
}