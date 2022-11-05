from collections import namedtuple
from random import choices, randrange, random, randint
from typing import List, Callable, Tuple

Genome = List[int]
Population = List[Genome]
Thing = namedtuple('Thing', ['name', 'value', 'weight'])
FitnessFunc = Callable[[Genome, List[Thing], int], int]    # calculate value of a genome
PopulationFunc = Callable[[], Population] 
SelectionFunc = Callable[[Population, FitnessFunc], Tuple[Genome, Genome]]
CrossoverFunc = Callable[[Genome, Genome], Tuple[Genome, Genome]]
MutationFunc = Callable[[Genome], Genome]

things = [
    Thing('Iphone', 600, 400),
    Thing('Headphones', 150, 160),
    Thing('Sweater', 399, 280),
    Thing('XBox', 200, 303),
    Thing('Snacks', 30, 192),
]

more_things = [
    Thing('Candy', 5, 5),
    Thing('Water', 10, 180),
    Thing('Tissues', 25, 80),
    Thing('Laptop', 700, 2400),
    Thing('Football', 60, 250),
] + things


def generate_genome(length: int) -> Genome:
    return choices([0, 1], k=length)

def generate_population(size: int, genome_length: int) -> Population:
    return [generate_genome(genome_length) for _ in range(size)]

def fitness(genome: Genome, things: List[Thing], weight_limit: int) -> int:
    if len(genome) != len(things):
        raise ValueError("genome and things must have the same length")
    
    weight, value = 0, 0

    for i, thing in enumerate(things):
        if genome[i] == 1:
            weight += thing.weight
            value += thing.value

            if weight > weight_limit:
                return 0
    return value


def selection_pair(population: Population, fitness_func: FitnessFunc) -> Population:
    return choices(
        population=population,
        weights=[fitness_func(genome) for genome in population],
        k=2
    )


def single_point_crossover(a: Genome, b: Genome) -> Tuple[Genome, Genome]:
    if len(a) != len(b):
        raise ValueError("Genome a and b must be of the same size")
    length = len(a)
    if length < 2:
        return a, b
    
    p = randint(1, length-1)   # partition

    return a[:p] + b[p:], b[:p] + a[p:]


def mutation(genome: Genome, num: int = 1, probability: float = 0.1) -> Genome:
    for _ in range(num):
        index = randrange(len(genome))
        genome[index] = genome[index] if random() > probability else abs(genome[index] - 1)
    return genome


def run_evolution(
    populate_func: PopulationFunc,
    fitness_func: FitnessFunc,
    fitness_limit: int,
    selection_func: SelectionFunc = selection_pair,
    crossover_func: CrossoverFunc = single_point_crossover,
    mutation_func: MutationFunc = mutation,
    generation_limit: int = 100
) -> Tuple[Population, int]:
    population = populate_func()

    for i in range(generation_limit):
        population = sorted(
            population,
            key=lambda genome: fitness_func(genome),
            reverse=True
        )
        if fitness_func(population[0] >= fitness_limit):
            break
            
        next_generation = population[:2]

        for _ in range(int(len(population)/2)-1):
            parents = selection_func(population, fitness_func)
            offspring_a, offspring_b = crossover_func(parents[0], parents[1])
            offspring_a = mutation_func(offspring_a)
            offspring_b = mutation_func(offspring_b)
            next_generation += [offspring_a, offspring_b]
        
        population = next_generation
    
    population = sorted(
        population,
        key=lambda genome: fitness_func(genome),
        reverse=True
    )

    return population, i

