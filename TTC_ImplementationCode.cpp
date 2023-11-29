// Gale's TTC algorithm implementation.
// Miren Lur Barquin Torre. 17 February 2020.

#include<iostream>
#include<vector>
#include<numeric>
#include<cmath>
#include<string>
#include<ctime>
#include<random>
#include<time.h>
#include<iomanip>
#include<fstream>

using namespace std;

void PreferenceMat();	// Initial preference matrix using vectors & number of couples.
vector<vector<int>> inCycle(int vertex, vector<int> AdjacVect);		// Returns a vector with 1 (true) if vertex is in a cycle (or loop) and 0 (false) if vertex is not in a cycle (or loop) as first element, and the cycle to which vertex belongs in second position (empty cycle if vertex does not belong to any).  

clock_t timeUsed;
double cpu_time_used;
float nCouples;
int nStages = 0, nLoops = 0, nCycles = 0, lengthMaxCycles = 0, lengthMinCycles, 
	nMinCycles, nMaxCycles = 0;
vector<int> V, auxNumCyclesInStage, auxLengthCycles;	// Vector V will represent at the same time agents, objects and vertices.
vector<vector<int>> PrefMat;

struct Results {
	int dim, num_Stages, num_Transplants, num_Cycles, numMin_CyclesInStage, numMax_CyclesInStage, lengthMin_Cycles, lenghtMax_Cycles;
	float time, numAverage_CyclesInStage, lengthAverage_Cycles, percent_Transpl;
};

int main() {
	PreferenceMat();	// Obtains the preference matrix & number of pairs taking part in the exchange.
	nMinCycles = nCouples;	 // nCouples would be the max number of cycles that could be in the problem (considering all were loops).
	lengthMinCycles = nCouples;	 // The longest possible cycle is the one in which every couple participate.
	timeUsed = clock();
	vector<int> FinalAllocAdjacVector(nCouples);	// Creates a zero vector in which the final allocation will be saved.
	cout << "\nWHAT IS HAPPENING DURING THE RESOLUTION: ";
	int noncerosV = V.size();
	while (noncerosV != 0) {
		nStages++;
		cout << "\nSTAGE " << nStages;
		int nCyclesInThisStage = 0;
		vector<int> AdjacVect(nCouples);   //Creates a zero vector in which the adjacency vector at the current stage will be saved. Creating the adjacency vector for stage nStage, will be as drawing the graph.
		for (int j : V) {
			if (j != 0) {
				for (int i = 0; i < nCouples; i++) {
					if (PrefMat[i][j - 1] != 0) {
						AdjacVect[j - 1] = PrefMat[i][j - 1];
						break;
					}}}}

		//Once we have the graph with the arcs, we can search for the cycles and the loops.
		for (int k : V) {
			if (k != 0) {	   // If k is still in the problem.
				if (inCycle(k, AdjacVect)[0][0] == true) {
					cout << "\n cycle(s): ";
					vector<int> cycle = inCycle(k, AdjacVect)[1];
					for (int i : cycle) {
						cout << i << " ";
					}
					// See if it is a loop or a cycle.
					int lengthCycle = cycle.size();
					if (lengthCycle == 1) {
						nLoops++;
					}
					else {
						auxLengthCycles.push_back(lengthCycle);
						if (lengthCycle > lengthMaxCycles) { lengthMaxCycles = lengthCycle; }
						if (lengthCycle < lengthMinCycles) { lengthMinCycles = lengthCycle; }
						nCycles++;
						nCyclesInThisStage++;
					}
					// Allocate to each agent taking part in the cycle the corresponding object. 
					for (int v : cycle) {
						FinalAllocAdjacVector[v - 1] = AdjacVect[v - 1];
						for (int i = 0; i < nCouples; i++) {
							//Delete the column corresponding to agent v from the Preference matrix.
							PrefMat[i][v - 1] = 0;
						}
						for (int j : V) {
							if (j != 0) {
								if (j != v) {
									// Delete the object v from the preferences of the agents still in the problem
									for (int i = 0; i < nCouples; i++) {
										if (PrefMat[i][j - 1] == v) {
											PrefMat[i][j - 1] = 0;
											break;
										}}}}}

						// Remove the vertices in the cycle from V because they have already been assigned.
						for (int i = 0; i < V.size(); i++) {
							if (V[i] == v) {
								V[i] = 0;
								noncerosV--;
								break;
							}}}}}}

		auxNumCyclesInStage.push_back(nCyclesInThisStage);
		if (nCyclesInThisStage > nMaxCycles) { nMaxCycles = nCyclesInThisStage; }
		if (nCyclesInThisStage < nMinCycles) { nMinCycles = nCyclesInThisStage; }
	}
	cout << "\n \n \n The final allocation vector will be:\n";
	cout << "( ";
	for (int i : FinalAllocAdjacVector) {
		cout << i << " ";
	}
	cout << ")";
	cout << "\n \n";
	cout << "FINAL ALLOCATION: " << endl;
	for (int i = 0; i < nCouples; i++) {
		cout << "Agent: " << i + 1 << " obtains object: " << FinalAllocAdjacVector[i] << "." << endl;
	}
	cout << "* Keep in mind that in those cases where the agent obtains its initial object, the transplant will not be carried out. ";

	if(auxLengthCycles.size() == 0) {auxLengthCycles.push_back(0); lengthMinCycles = 0; } // In the case that there are no cycles (just loops)
	if(auxNumCyclesInStage.size() == 0) {auxNumCyclesInStage.push_back(0); nMinCycles = 0; }
	
	float numAverageCyclesInStage = accumulate(auxNumCyclesInStage.begin(), auxNumCyclesInStage.end(), 0.0) / auxNumCyclesInStage.size();
	float AverageLength = accumulate(auxLengthCycles.begin(), auxLengthCycles.end(), 0.0) / auxLengthCycles.size();

	timeUsed = clock() - timeUsed;
	cpu_time_used = ((double)(timeUsed)) / CLOCKS_PER_SEC;

	Results FinalResults;
	FinalResults.dim = nCouples;
	FinalResults.num_Stages = nStages;
	FinalResults.time = cpu_time_used;
	FinalResults.num_Transplants = nCouples - nLoops;
	FinalResults.percent_Transpl = ((nCouples - nLoops) * 100) / nCouples;
	FinalResults.num_Cycles = nCycles;
	FinalResults.numMax_CyclesInStage = nMaxCycles;
	FinalResults.numAverage_CyclesInStage = numAverageCyclesInStage;
	FinalResults.numMin_CyclesInStage = nMinCycles;
	FinalResults.lenghtMax_Cycles = lengthMaxCycles;
	FinalResults.lengthAverage_Cycles = AverageLength;
	FinalResults.lengthMin_Cycles = lengthMinCycles;

	cout << "\n \n";
	cout << "RESULTS: " << endl;
	cout << "Dimension: " << FinalResults.dim << endl;
	cout << "Number of stages: " << FinalResults.num_Stages << endl;
	cout << "time: " << FinalResults.time << endl;
	cout << "Number of transplants: " << FinalResults.num_Transplants << endl;
	cout << "Percentage of transplanted kidneys: " << FinalResults.percent_Transpl << endl;
	cout << "Number of cycles: " << FinalResults.num_Cycles << endl;
	cout << "Minimum number of cycles in a stage: " << FinalResults.numMin_CyclesInStage << endl;
	cout << "Average number of cycles in a stage: " << FinalResults.numAverage_CyclesInStage << endl;
	cout << "Maximum number of cycles in a stage: " << FinalResults.numMax_CyclesInStage << endl;
	cout << "Length of the shortest cycle: " << FinalResults.lengthMin_Cycles << endl;
	cout << "Average length of cycles: " << FinalResults.lengthAverage_Cycles << endl;
	cout << "Length of the longest cycle : " << FinalResults.lenghtMax_Cycles << endl;

	ofstream out("ResultadosTTCG.txt", fstream::app);
	out <<FinalResults.dim << " & " << FinalResults.num_Stages << " & " << FinalResults.time << " & " << FinalResults.num_Transplants<< " & " << FinalResults.percent_Transpl << " & " << FinalResults.num_Cycles << " & " << FinalResults.numMin_CyclesInStage << " & " << FinalResults.numAverage_CyclesInStage << " & " << FinalResults.numMax_CyclesInStage << " & " << FinalResults.lengthMin_Cycles << " & " << FinalResults.lengthAverage_Cycles << " & " << FinalResults.lenghtMax_Cycles <<" \\\\ "<< endl;
	out << "\\hline" << endl;
	out.close();
	return 0;
}

// DEFINITION OF FUNCTIONS:

void PreferenceMat() {
	// See how many couples are taking part in the problem.
	int pairs = 0;
	ifstream nPairs("RPrefMat.dat", fstream::binary | fstream::out);
	string unused;
	while (getline(nPairs, unused))
		++pairs;
	nPairs.close();

	// Obtain the preference Matrix.
	nCouples = pairs;
	for (int i = 0; i < nCouples; i++) {
		V.push_back(i + 1);}

	vector<vector<int>> AuxPrefMat(nCouples, vector<int>(nCouples));
	ifstream mat("RPrefMat.dat", fstream::binary | fstream::out);
	for (int i = 0; i < nCouples; i++) {
		for (int j = 0; j < nCouples; j++) {
			mat >> AuxPrefMat[i][j];}}

	mat.close();
	PrefMat = AuxPrefMat;

	//SHOW THE MATRIX:
	cout << "\n \n The preference matrix is: \n";
	cout << fixed << setfill(' ');
	for (int i = 0; i < nCouples; i++) {
		for (int j = 0; j < nCouples; j++) {
			cout << setw(5) << PrefMat[i][j];}
		cout << "\n";}
}

vector<vector<int>> inCycle(int vertex, vector<int> AdjacVect) {
	int aux = AdjacVect[vertex - 1];
	bool IsInAuxVect = false;
	vector<int> auxVect, cycle;
	if (vertex != aux) { auxVect.push_back(vertex); }
	auxVect.push_back(aux);
	while (IsInAuxVect == false) {
		if (AdjacVect[aux - 1] == vertex) {
			cycle = auxVect;
			vector<vector<int>> ToReturn = { {1}, cycle };
			return ToReturn;}
		else {
			aux = AdjacVect[aux - 1];
			for (int i : auxVect) {
				if (i == aux) {
					IsInAuxVect = true;
					break;
				}}
			if (IsInAuxVect != true) {
				auxVect.push_back(aux);
			}}}

	vector<vector<int>> ToReturn = { {0}, {} };
	return ToReturn;
}