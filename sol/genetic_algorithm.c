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

int min(int a, int b) {
    return a < b ? a : b;
}

void compute_fitness_function(input in, const sack_object *objects, individual *generation, int object_count, int sack_capacity)
{
	int weight;
	int profit;
	// printf("id = %d\n", in.thread_id);
	int start = in.thread_id * ceil(object_count / in.P);
    int end = min((in.thread_id + 1) * ceil(object_count / in.P), object_count);
    if (end == 0 || (in.thread_id == in.P - 1 && end != object_count)) {
    	end = object_count;
    }
    // printf("start = %d , end = %d,   thread_id = %d\n", start, end, in.thread_id);
	for (int i = start; i < end; ++i) {
		weight = 0;
		profit = 0;
		// posibil este necesar un mutex
		for (int j = 0; j < generation[i].chromosome_length; ++j) {
			if (generation[i].chromosomes[j]) {
				weight += objects[j].weight;
				profit += objects[j].profit;
			}
		}

		generation[i].fitness = (weight <= sack_capacity) ? profit : 0;
	}
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
		// int start = in.thread_id * ceil(mutation_size / in.P);
  //   	int end = min((in.thread_id + 1) * ceil(mutation_size / in.P), mutation_size);
  //   	if (end == 0 || (in.thread_id == in.P - 1 && end != mutation_size)) {
	 //    	end = mutation_size;
	 //    }
		for (i = 0; i < mutation_size; i += step) {
			ind->chromosomes[i] = 1 - ind->chromosomes[i];
		}
	} else {
		// for even-indexed individuals, mutate the last 80% chromosomes by a given step
		mutation_size = ind->chromosome_length * 8 / 10;
		// int start = in.thread_id * ceil(ind->chromosome_length / in.P);
  //   	int end = min((in.thread_id + 1) * ceil(ind->chromosome_length / in.P), ind->chromosome_length);
  //   	if (end == 0 || (in.thread_id == in.P - 1 && end != ind->chromosome_length)) {
	 //    	end = ind->chromosome_length;
	 //    }
		for (i = ind->chromosome_length - mutation_size; i < ind->chromosome_length; i += step) {
			ind->chromosomes[i] = 1 - ind->chromosomes[i];
		}
	}
}

void mutate_bit_string_2(input in, const individual *ind, int generation_index)
{
	int step = 1 + generation_index % (ind->chromosome_length - 2);
printf("step = %d\n", step);
	// mutate all chromosomes by a given step
	// int start = in.thread_id * ceil(ind->chromosome_length / in.P);
 //    int end = min((in.thread_id + 1) * ceil(ind->chromosome_length / in.P), ind->chromosome_length);
	// if (end == 0 || (in.thread_id == in.P - 1 && end != ind->chromosome_length)) {
	//     	end = ind->chromosome_length;
	//     }
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
	int start = in.thread_id * ceil(generation->chromosome_length / in.P);
    int end = min((in.thread_id + 1) * ceil(generation->chromosome_length / in.P), generation->chromosome_length);

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
	const sack_object *objects = in.objects;
	int object_count = in.object_count;
	int generations_count = in.generations_count;
	int sack_capacity = in.sack_capacity;
	int thread_id = in.thread_id;
	static int flag = 0;

	int count, cursor;
	individual *current_generation = in.current_generation;
	individual *next_generation = in.next_generation;
	individual *tmp = NULL;

	// daca P > obj_count -> p = objCount
	int start = thread_id * ceil(object_count / in.P);
    int end = min((thread_id + 1) * ceil(object_count / in.P), object_count);
    if (end == 0 || (thread_id == in.P - 1 && end != object_count)) {
    	end = object_count;
    }
	//printf("start = %d,  end = %d\n",start, end);
	// set initial generation (composed of object_count individuals with a single item in the sack)
	for (int i = start; i < end; ++i) {
		current_generation[i].fitness = 0;
		current_generation[i].chromosomes = (int*) calloc(object_count, sizeof(int));
		current_generation[i].chromosomes[i] = 1;
		current_generation[i].index = i;
		current_generation[i].chromosome_length = object_count;

		next_generation[i].fitness = 0;
		next_generation[i].chromosomes = (int*) calloc(object_count, sizeof(int));
		next_generation[i].index = i;
		next_generation[i].chromosome_length = object_count;
	}
	
	
	// iterate for each generation
	for (int k = 0; k < generations_count; ++k) {
		cursor = 0;
		pthread_barrier_wait(in.barrier);
		// printf("BEFORE:\n");
		// print_generation(current_generation, object_count);
		// printf("\n");
		// compute fitness and sort by it
		compute_fitness_function(in, objects, current_generation, object_count, sack_capacity);
		// printf("AFTER:\n");
		// print_generation(current_generation, object_count);
		// printf("\n\n");
		// sortarea trebuie sa se faca doar o data, abia dupa ce s-a terminat
		// calcularea fittnes-ului fiecarui individ din generatie!!
		pthread_barrier_wait(in.barrier);
		// se face sortarea doar de catre un thread
		if (flag == k) {
			qsort(current_generation, object_count, sizeof(individual), cmpfunc);
			flag++;
		}
		
		// keep first 30% children (elite children selection)
		count = object_count * 3 / 10;
		//printf("count = %d\n", count);
		int start2 = thread_id * ceil(count / in.P);
		int end2 = min((thread_id + 1) * ceil(count / in.P), count);
		if (end2 == 0 || (thread_id == in.P - 1 && end2 != count)) {
    		end2 = count;
    	}
		//printf("start3 = %d   end3 = %d    thread_id = %d\n", start3, end3, thread_id);
		//pthread_barrier_wait(in.barrier);
		for (int i = start2; i < end2; ++i) {
			copy_individual(current_generation + i, next_generation + i);
		}
		cursor = count;
		
		// mutate first 20% children with the first version of bit string mutation
		count = object_count * 2 / 10;
		//printf("cursor = %d   count = %d\n", cursor, count);
		int start3 = thread_id * ceil(count / in.P);
		int end3 = min((thread_id + 1) * ceil(count / in.P), count);
		if (end3 == 0 || (thread_id == in.P - 1 && end3 != count)) {
	    	end3 = count;
	    }
		//printf("start4 = %d   end4 = %d\n", start4, end4);
		pthread_barrier_wait(in.barrier);
		for (int i = start3; i < end3; ++i) {
			copy_individual(current_generation + i, next_generation + cursor + i);
			mutate_bit_string_1(in, next_generation + cursor + i, k);
		}
		cursor += count;

		// mutate next 20% children with the second version of bit string mutation
		count = object_count * 2 / 10;
		int start4 = thread_id * ceil(count / in.P);
		int end4 = min((thread_id + 1) * ceil(count / in.P), count);
		if (end4 == 0 || (thread_id == in.P - 1 && end4 != count)) {
	    	end4 = count;
	    }
		//printf("start5 = %d   end5 = %d\n", start5, end5);
		//printf("cursor = %d   count = %d\n", cursor, count);
		pthread_barrier_wait(in.barrier);
		for (int i = start4; i < end4; ++i) {
			copy_individual(current_generation + i + count, next_generation + cursor + i);
			mutate_bit_string_2(in, next_generation + cursor + i, k);
		}
		cursor += count;

		// crossover first 30% parents with one-point crossover
		// (if there is an odd number of parents, the last one is kept as such)
		count = object_count * 3 / 10;
		//printf("cursor = %d   count = %d\n", cursor, count);
		//pthread_mutex_lock(in.mutex);
		if (count % 2 == 1) {
			copy_individual(current_generation + object_count - 1, next_generation + cursor + count - 1);
			count--;
		}

		//pthread_mutex_unlock(in.mutex);
		//pthread_barrier_wait(in.barrier);
		//pthread_mutex_lock(in.mutex);
		//printf("id = %d\n", thread_id);
		//printf("count = %d,  cursor = %d\n",count, cursor);
		int start5 = thread_id * ceil(count / in.P);
		int end5 = min((thread_id + 1) * ceil(count / in.P), count);
		if (end5 == 0 || (thread_id == in.P - 1 && end5 != count)) {
	    	end5 = count;
	    }
		for (int i = start5; i < end5; i += 2) {
		//	printf("i = %d,  start6 = %d,   end6 = %d    thread_id = %d\n",i, start6, end6, thread_id);
			crossover(current_generation + i, next_generation + cursor + i, k);
		}
		//pthread_mutex_unlock(in.mutex);
		// switch to new generation
		pthread_barrier_wait(in.barrier);
		if (flag == k + 1) {
			tmp = current_generation;
			current_generation = next_generation;
			next_generation = tmp;
		}
		//pthread_barrier_wait(in.barrier);
		int start6 = thread_id * ceil(object_count / in.P);
   		int end6 = min((thread_id + 1) * ceil(object_count / in.P), object_count);
   		if (end6 == 0 || (thread_id == in.P - 1 && end6 != object_count)) {
	    	end6 = object_count;
	    }	
		for (int i = start6; i < end6; ++i) {
			current_generation[i].index = i;
		}

		if (k % 5 == 0 && thread_id == 0) {
			print_best_fitness(current_generation);
		}
	}

	pthread_barrier_wait(in.barrier);
	
	compute_fitness_function(in, objects, current_generation, object_count, sack_capacity);
	if (thread_id == 0) {
		qsort(current_generation, object_count, sizeof(individual), cmpfunc);
		print_best_fitness(current_generation);
	}
		
	//pthread_barrier_wait(in.barrier);
	// free resources for old generation
	//free_generation(in, current_generation, next_generation);
	pthread_exit(NULL);
}