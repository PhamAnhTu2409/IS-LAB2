#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

//�������� ���������: 
#define MAX_ITERATION 1000	  //������������ ������������ �������� (���������� ���������)
#define POPULATION_SIZE 2000  //������ ���������;
#define CROSSOVER_PROB  0.7f  //����������� ������������� (��� ����, ��� ����� ������ ������������)
#define MUTATIONRATE 0.15     //����������� �������
#define GENERATION_GAP 0.8    //�������� ������� ��������� 
#define L 0.2                 //�������� �������� 

// ���������� �������� ����� ��� ��������� ������� ��������� � ������������� �������
const string GENES = "�����Ũ�������������������������� ��������������������������������";

// ������, ������� ������������ �������� ������ �������� � ���� ����� ������
const string TARGET = "��� ������� ���";

// ��������������� ������ ��� �����������
string sec_child = "";


// ��������� ��������� ����� � �������� ���������
int random_num(int start, int end)
{
    int range = (end - start) + 1;
    int random_int = start + (rand() % range);
    return random_int;
}

// ����� ������ ����������� ������� ��� ������������� ��� ������ ����
char mutated_genes()
{
    int len = GENES.size();
    int r = random_num(0, len - 1);
    return GENES[r];
}

// ��������� ��������� ��� ������������ �����
string create_gnome()
{
    int len = TARGET.size();
    string gnome = "";
    for (int i = 0; i < len; i++)
        gnome += mutated_genes();
    return gnome;
}

// ����� � ���������
class Individual
{
public:
    string chromosome;
    int fitness;
    Individual(string chromosome);
    Individual mate(Individual parent2);
    string mutate(string child);
    int cal_fitness();
};

Individual::Individual(string chromosome)  //�������� �����
{
    this->chromosome = chromosome;
    fitness = cal_fitness();
};


string Individual::mutate(string child)    //�������
{
    string child_chromosome = "";
    int len = child.size();

    for (int i = 0; i < len; i++) {
        float m = random_num(0, 100) / 100;

        if (m > MUTATIONRATE)
            child_chromosome += mutated_genes();
        else child_chromosome += child[i];
    }

    return child_chromosome;
}


Individual Individual::mate(Individual par2) //�����������
{
    string child_chromosome = "";
    string child_chromosome2 = "";
    string sec_child2 = "";

    int len = chromosome.size();
    float p0 = 0.5;

    for (int i = 0; i < len; i++)
    {
        float p = random_num(0, 100) / 100;

        if (p < p0)
        {
            child_chromosome += chromosome[i];
            sec_child2 += par2.chromosome[i];
        }

        else
        {
            child_chromosome += par2.chromosome[i];
            sec_child2 += chromosome[i];
        }
    }

    child_chromosome2 = mutate(child_chromosome);
    sec_child = mutate(sec_child2);

    return Individual(child_chromosome2);
};

// ������� ���-�� ��� ��������, ������� �� ��������� � ������� �������
int Individual::cal_fitness()
{
    int len = TARGET.size();
    int fitness = 0;
    for (int i = 0; i < len; i++)
    {
        if (chromosome[i] != TARGET[i])
            fitness++;
    }
    return fitness;
};

// ���������� ��������� < 
bool operator<(const Individual& ind1, const Individual& ind2)
{
    return ind1.fitness < ind2.fitness;
}

// ������������ ��������
int main()
{
    setlocale(LC_ALL, "Russian");
    system("chcp 1251");
    srand((unsigned)(time(0)));

    // ������� ���������
    int generation = 0;

    vector<Individual> population;
    bool found = false;

    // ������������ ��������� ���������
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        string gnome = create_gnome();
        population.push_back(Individual(gnome));
    }

    while (!found && generation < MAX_ITERATION)
    {
        // ���������� ��������� � ������� ����������� ���������� ������������ � ������� ������� (�� ���������������� ���� ������ ���������)
        sort(population.begin(), population.end());

        // �������� ���������� ������� ������ � ���������
        if (population[0].fitness <= 0)
        {
            found = true;
            break;
        }

        // �������� ������ ���������
        vector<Individual> new_generation;

        // ������� ����� ������������� ��������� � ��������� ���������, ����� �����������    
        int s = (1 - GENERATION_GAP) * POPULATION_SIZE;
        for (int i = 0; i < s; i++)
            new_generation.push_back(population[i]);

        //���� �������� (���������), ����������� (���������� �������� �������������) � ������� (�������) ������
        int k = 0;
        while (k < POPULATION_SIZE)
        {
            int i = random_num(s, s - 1 + (POPULATION_SIZE - s) * L);
            int j = random_num(s, s - 1 + (POPULATION_SIZE - s) * L);

            Individual parent1 = population[i];
            Individual parent2 = population[j];
            Individual offspring1 = population[i], offspring2 = population[j];
            if (CROSSOVER_PROB > (random_num(0, 100) / 100)) {

                offspring1 = parent1.mate(parent2);
                offspring2 = Individual(sec_child);
                sec_child = "";
            }
            k += 2;
            new_generation.push_back(offspring1);
            new_generation.push_back(offspring2);

        }
        population = new_generation;
        cout << "���������: " << generation << "\t";
        cout << "������: " << population[0].chromosome << "\t";
        cout << "������������: " << population[0].fitness << "\n";

        generation++;
    }
    cout << "���������: " << generation << "\t";
    cout << "������: " << population[0].chromosome << "\t";
    cout << "������������: " << population[0].fitness << "\n";
}
