//Andrew Logue 12/8/19
//Final project

//import
#include <iostream>
#include <ctime>
#include <math.h>
#include <string.h>
#include <stdio.h> 
#include <bits/stdc++.h>
using namespace std;
//create changable popSize variable
#define popSize 4000
#define exponentialFactor 1

//initialize functions
string *buildPopulation (int targetSize);
double *calculateFitness (string organisms[], string target, int targetSize);
int *buildMatingPool (double fitScores[]);
string getRandParent (string organisms[], int matingPool[]);
string breed (string parent1, string parent2, string target);
string causeMutation (string child, int mutationRate);

int main() {
    //seed time
    srand(time(NULL));
    //initialize varibales
    int targetSize;
    string *populationTemp;
    string population[popSize] = {""};
    string target = "";
    string child;
    double fitness[popSize] = {};
    double *fitnessTemp;
    int *matingPoolTemp;
    bool targetFound = false;
    int generationCounter = 0;
    int mostFitIndex = 0;
    int matingPoolLength;
    string mostFit;
    //create results.csv file
    ofstream myfile;
    myfile.open("results.csv");
    myfile << "Fitness,Generation\n";

    //define target
    cout << "Define target: " << endl;
    getline(cin, target);

    //generate initial population
    targetSize = target.length();
    populationTemp = buildPopulation(targetSize);
    copy(populationTemp, populationTemp + popSize, population);
    
    //repeat 'breeding' process until target is matched
    while (targetFound == false) {
        fitnessTemp = calculateFitness(population, target, targetSize);
        copy(fitnessTemp, fitnessTemp + popSize, fitness);
        generationCounter++;
        
        //build new mating pool
        matingPoolTemp = buildMatingPool(fitness);
        int matingPool[matingPoolTemp[0]] = {};
        matingPoolLength = matingPoolTemp[0];
        copy(matingPoolTemp, matingPoolTemp + matingPoolLength, matingPool);

        //calculate the maximum fitness member from the current generation, as well as generation number
        for (int i = 0; i < popSize; i++) {
            //cout << population[i] << fitness[i] << endl;
            if (population[i] == target) {
                targetFound = true;
            }
            if (fitness[i] >= fitness[mostFitIndex]) {
                mostFit = population[i];
                mostFitIndex = i;
            }
        }
        
        //breed a new population
        for (int j = 0; j < popSize; j++) {
            //randomly select two unique parents from mating pool
            //breed child from parents
            child = breed(getRandParent(population, matingPool), getRandParent(population, matingPool), target);

            //mutate child
            child = causeMutation(child, 0);
            //replace old population members with children until entirely new generation has been bred
            population[j] = child;
        }
        //print the maximum fitness member from the current generation, as well as generation number
        //to the terminal.
        cout << "Maximum fitnes member: " << mostFit << "  Fitness: " << fitness[mostFitIndex] << "  Generation number: " << generationCounter << endl;
        
        //Task 1.6: Bring it all together in a .cpp file!
        //store (write) the values of the fitness scores in each generation into a .csv file. (CSV stands
        //for comma separated values). Ideally the csv should be formed in such a way that it is easily
        //readable by MATLAB.
        for (int p = 0; p < popSize; p++) {
            myfile << fitness[p] << ",";
        }
        myfile << generationCounter << "\n";
    }
    myfile.close();
    return 0;
}

//Task 1.1: Randomly generate initial population of strings
string *buildPopulation(int targetSize) {
    //initialize variables
    static string DNAarray[popSize] = {""};
    char charArray[targetSize] = {};
    charArray[targetSize] = '\0';
    bool correct = false;
    int asciiDNA;

    //creates 200 DNA samples
    for (int i = 0; i < popSize; i++) {
        //specifies the length of each DNA sample

        for (int j = 0; j < targetSize; j++) {
            correct = false;
            do {
                //gets a random number between 64 and 122 (A-z in ascii decimal)
                asciiDNA = rand() % 58 + 64;
            //throws out ascii decimal values 91 - 96 ([ - `)
            if ((asciiDNA >= 64 && asciiDNA <= 90) || (asciiDNA >= 97 && asciiDNA <= 122)) {
                correct = true;
            }
            } while (!correct);
        //in order to save processing time, the decimal value 64 (@ in ascii) will be
        //converted to " ", or decimal value 32
        if (asciiDNA == 64) {
                    asciiDNA = 32;
                }
        charArray[j] = (char)asciiDNA;
        }
        //fill DNAarray[] with strings to the specified length (popSize)
        DNAarray[i] = charArray;
    }

    //return DNAarray
    return DNAarray;
}

//Task 1.2: Calculate the fitness of each member in the population
double *calculateFitness(string organisms[], string target, int targetSize) {
    //initialize variables
    static double fitScores[popSize] = {0};
    double DNAcounter;
    
    //gets the number of mathcing chars from each string
    for (int i = 0; i < popSize; i++) {
        DNAcounter = 0;

        //checks each individual character of the string for match in same
        //index of target string
        for (int j = 0; j < targetSize; j++) {
            if (target.at(j) == organisms[i].at(j)) {
            DNAcounter++;
            }
        }
        //add the percentage of matching characters to the target string
        //in decimal form to the cooresponding index in fitScores[]
        fitScores[i] = pow((DNAcounter / targetSize), exponentialFactor);
    }
    //return the array full of the fitness score of each 'organism'
    return fitScores;
}

//Task 1.3: Build a mating pool
int *buildMatingPool (double fitScores[]){
    //initialize varibales
    static int matingPoolTemp[(popSize*20)] = {0};
    double newFitScores[popSize] = {};
    double ticketCounter;
    double maxFit = 0;
    double temp;
    int mostFitIndex = 0;
    int poolCounter = 1;
    int matingPoolSize = 11;
    bool correct = false;
    
    //get maxFit
    for (int k = 0; k < popSize; k++) {
            if (fitScores[k] >= fitScores[mostFitIndex]) {
                maxFit = fitScores[k];
                mostFitIndex = k;
            }
    }

    //sort fitScores so that the string with the most characters in common
    //with the target string has value 1, and the one with the least has 0
    //sort(fitScores, fitScores + popSize);
    for (int i = 0; i < popSize; i++) {
        //double temp = round(((i+1.0) / 200.0) * 10.0);
        if (fitScores[i] != 0) {
            temp = (fitScores[i] + (1.0 - maxFit)) * 10.0;
        }
        else {
            temp = 0.0;
        }
        ticketCounter = temp;
        for (int j = 0; j < ticketCounter; j++) {
            matingPoolTemp[poolCounter] = i;
            poolCounter++;
        }
    }
    do {
        matingPoolSize++;
        if (matingPoolTemp[matingPoolSize] == 0) {
            correct = true;
        }
    } while (!correct);
    matingPoolTemp[0] = matingPoolSize;

    //return mating pool array
    return matingPoolTemp;
}

//Task 1.4: Breed a child from two parents
string getRandParent (string organisms[], int matingPool[]) {
    //initialize varibales
    string parent;
    int matingPoolSize = matingPool[0];
    bool correct = false;
    int randNum;
    //chooses a random ticket from mating pool and assigns that
    //index in organisms[] to a parent
    randNum = rand() % (matingPoolSize - 1) + 1;
    parent = organisms[matingPool[randNum]];

    //return the random parent
    return parent;
}

//Task 1.5: Cause DNA mutation
string breed (string parent1, string parent2, string target) {
    //initialize varibales
    string child;
    char charArray[parent1.length()];
    int asciiVal;
    bool correct = false;
    /*
    //method 1
    int midpoint = (rand() % (target.length() - 1) + 1);
    child = parent1.substr(0, midpoint + 1) + parent2.substr(midpoint + 1);
    */
    //method 2
    for (int i = 0; i < parent1.length(); i++) {
        if (rand()%2 == 0) {
            charArray[i] = parent1.at(i);
        }
        else {
            charArray[i] = parent2.at(i);
        }
    }
    
    for (int j = 0; j < parent1.length(); j++) {
        child = child + charArray[j];
    }
    
    /*
    //Task 1.7: Experiment!
    //MOST EFFECIENT METHOD, CREATED BY ME
    //method 3
    //loops for each character in the parent
    for (int i = 0; i < parent1.length(); i++) {
        //checks to see if any of the characters match those in the target array
        //if so then they get to stay
        //if not, a new random character is chosen to replace it 
        if (target.at(i) == parent1.at(i)) {
            charArray[i] = parent1.at(i);
        }
        else if (target.at(i) == parent2.at(i)) {
            charArray[i] = parent2.at(i);
        }
        else {
            correct = false;
            do {
                //gets a random number between 64 and 122 (A-z in ascii decimal)
                asciiVal = rand() % 58 + 64;
                //throws out ascii decimal values 91 - 96 ([ - `)
                if ((asciiVal >= 64 && asciiVal <= 90) || (asciiVal >= 97 && asciiVal <= 122)) {
                    correct = true;
                }
            } while (!correct);
            //in order to save processing time, the decimal value 64 (@ in ascii) will be
            //converted to " ", or decimal value 32
            if (asciiVal == 64) {
                    asciiVal = 32;
                }
            charArray[i] = (char)asciiVal;
            }
        }
    //populatate child
    for (int j = 0; j < parent1.length(); j++) {
        child = child + charArray[j];
    }
    */

    //return the complete child
    return child;
}

string causeMutation (string child, int mutationRate) {
    //initialize varibales
    int asciiVal;
    bool correct = false;
    //if the chance to mutate is matched, then a new  random character will replace
    //an index in child
    for (int i = 0; i < child.length(); i++) {
        if (rand() % 101 < mutationRate) {
            correct = false;
            do {
                //gets a random number between 64 and 122 (A-z in ascii decimal)
                asciiVal = rand() % 58 + 64;
                //throws out ascii decimal values 91 - 96 ([ - `)
                if ((asciiVal >= 64 && asciiVal <= 90) || (asciiVal >= 97 && asciiVal <= 122)) {
                    correct = true;
                }
            } while (!correct);
            //in order to save processing time, the decimal value 64 (@ in ascii) will be
            //converted to " ", or decimal value 32
            if (asciiVal == 64) {
                    asciiVal = 32;
                }
            child.at(i) = (char)asciiVal;
            }
    }

    //return the mutated child
    return child;
}