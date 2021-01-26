// SIGUIENTE AJUSTE USANDO LISTAS SIMPLES (VECTOR EN C++)

#include <iostream>
#include <vector>

using namespace std;

// Global
// Tamaño de memoria (KB)
const int memory_size = 64;
// Tamaño de bloques de memoria (KB)
const int block_size = 1;
// Cantidad de bloques en la memoria
const int block_qty = memory_size / block_size;

// Región de memoria
struct reg{
	char tag;
    int start;
    int size;

    reg(char c, int x, int y){
    	tag = c; start = x; size = y;
    };

    reg() {
    	tag = '\0'; start = 0; size = 0; 
    }
};

//Métodos
void Allocation(vector<reg> &H, vector<reg> &P, char* M, int A[][2], int numProc);
void Deallocation(vector<reg> &H, vector<reg> &P, char* M, char F[], int numProc);
void ShowMemory(vector<reg> const &H, vector<reg> const &P, char* M);
void Compaction(vector<reg> &H, vector<reg> &P, char* M);

int main(int argc, char const *argv[])
{
	// Memoria Ram
	char memory[block_qty];
	for (int i = 0; i < block_qty; ++i)
	{
		memory[i] = ' ';
	}
	// Lista de procesos
	vector<reg> processes;
	// Lista de huecos
	vector<reg> holes;
	// Init lista de huecos
	holes.push_back(reg(' ', 0, block_qty));

	//START OF INPUT
	// Procesos a asignar (Nombre y tamaño)
	int alloc[][2] = {
		{'A', 4}, {'a', 10}, {'B', 6}, {'b', 4}, {'C', 8}, {'c', 19}, {'D', 2}, {'d', 11}
	};
	// Asignacion de memoria
	Allocation(holes, processes, memory, alloc, 8);

	// Procesos a desasignar
	char free[] = {'a', 'b', 'c', 'd'};
	// Desasignacion de memoria
	Deallocation(holes, processes, memory, free, 4);

	//Mostrar listas 
	cout<<"Lista de procesos(start, size): ";
	for (int i = 0; i < processes.size(); ++i)
	{
		cout<<"("<<processes[i].start<<","<<processes[i].size<<")->";
	}
	cout<<"\nLista de huecos(start, size): ";
	for (int i = 0; i < processes.size(); ++i)
	{
		cout<<"("<<holes[i].start<<","<<holes[i].size<<")->";
	}
	cout<<"\n";

	// Procesos a asignar (Nombre y tamaño)
	int alloc1[][2] = {
		{'E', 12}
	};
	// Asignacion de memoria
	Allocation(holes, processes, memory, alloc1, 1);

	//Mostrar listas 
	cout<<"Lista de procesos(start, size): ";
	for (int i = 0; i < processes.size(); ++i)
	{
		cout<<"("<<processes[i].start<<","<<processes[i].size<<")->";
	}
	cout<<"\nLista de huecos(start, size): ";
	for (int i = 0; i < processes.size(); ++i)
	{
		cout<<"("<<holes[i].start<<","<<holes[i].size<<")->";
	}
	cout<<"\n";

	// Procesos a asignar (Nombre y tamaño)
	int alloc2[][2] = {
		{'F', 10}
	};
	// Asignacion de memoria
	Allocation(holes, processes, memory, alloc2, 1);

	//Mostrar listas 
	cout<<"Lista de procesos(start, size): ";
	for (int i = 0; i < processes.size(); ++i)
	{
		cout<<"("<<processes[i].start<<","<<processes[i].size<<")->";
	}
	cout<<"\nLista de huecos(start, size): ";
	for (int i = 0; i < processes.size(); ++i)
	{
		cout<<"("<<holes[i].start<<","<<holes[i].size<<")->";
	}
	cout<<"\n";

	// Procesos a asignar (Nombre y tamaño)
	int alloc3[][2] = {
		{'G', 9}
	};
	// Asignacion de memoria
	Allocation(holes, processes, memory, alloc3, 1);

	//Mostrar listas 
	cout<<"Lista de procesos(start, size): ";
	for (int i = 0; i < processes.size(); ++i)
	{
		cout<<"("<<processes[i].start<<","<<processes[i].size<<")->";
	}
	cout<<"\nLista de huecos(start, size): ";
	for (int i = 0; i < processes.size(); ++i)
	{
		cout<<"("<<holes[i].start<<","<<holes[i].size<<")->";
	}
	cout<<"\n";

	Compaction(holes, processes, memory);

	//END OF INPUT

	return 0;
}

// Asignación de memoria
void Allocation(vector<reg> &H, vector<reg> &P, char* M, int A[][2], int numProc) {
	char tagP;
	int i, j, sizeP;
	bool overflow;
	reg currentH = H.back(), currentP;
	
	// Buscar siguiente ajuste
	for (i = 0; i < numProc; ++i)
	{
		tagP = (char)A[i][0];
		sizeP = A[i][1]; 
		cout<<"ALLOC("<<tagP<<")=\t";

		// El proceso puede asignarse en el hueco actual
		if (sizeP <= currentH.size)
		{
			// Asignar proceso
			P.push_back(reg(tagP, currentH.start, sizeP));
			currentP = P.back();
			// Reasignar hueco actual
			H.back() = reg(' ', currentH.start + sizeP, currentH.size - sizeP);
			currentH = H.back();
			// LLenar RAM
			for (j = currentP.start; j < currentP.start + currentP.size; ++j)
			{
				M[j] = tagP;
			}
			ShowMemory(H, P, M);
		}
		// EL proceso no pudo asignarse en el hueco actual
		else 
		{
			overflow = true;
			// Buscar espacio en huecos siguientes
			for (j = 0; j < H.size(); ++j)
			{
				if (sizeP <= H[j].size)
				{
					currentH = H[j];
					overflow = false;
					break;
				}
			}

			// Hay espacio de memoria para el proceso
			if (!overflow)
			{
				// Asignar proceso
				P.push_back(reg(tagP, currentH.start, sizeP));
				currentP = P.back();
				// Reasignar hueco actual
				H.back() = reg(' ', currentH.start + sizeP, currentH.size - sizeP);
				currentH = H.back();
				for (j = currentP.start; j < currentP.start + currentP.size; ++j)
				{
					M[j] = tagP;
				}
				ShowMemory(H, P, M);
			}
			else {
				cout<<"No hay espacio de memoria para el proceso "<<tagP<<", intente compactar.\n";
			}
		}
	}
}	

// Desasignación de memoria
void Deallocation(vector<reg> &H, vector<reg> &P, char* M, char F[], int numProc) {
	char tagP;
	int i, j, k;
	reg currentP;
	for (i = 0; i < numProc; ++i)
	{
		tagP = (char)F[i];
		cout<<"FREE("<<tagP<<")=\t";
	
		// Buscar en memoria el proceso a desasignar
		for(j=0; j < P.size(); j++) {
			// El proceso buscado está presente en memoria
   			if (P[j].tag == tagP)
   			{
   				currentP = P[j];
				// Vaciar región de RAM
				for (k = currentP.start; k < currentP.start + currentP.size; ++k)
				{
					M[k] = ' ';
				}
				// Asignar hueco
				H.push_back(reg(' ', currentP.start, currentP.size));
   				// Desasignar proceso
   				P.erase(P.begin() + j);
   				ShowMemory(H, P, M);
   				break;
   			}
		}
	}
}

// Mostrar memoria
void ShowMemory(vector<reg> const &H, vector<reg> const &P, char* M) {
	cout<<"Procesos:["<<P.size()<<"]\tHuecos:["<<H.size()<<"]\tMemoria: [";
	for (int i = 0; i < block_qty; ++i)
	{
		cout<<M[i];
	}
	cout<<"]\n";
}

// Compactación 
void Compaction(vector<reg> &H, vector<reg> &P, char* M) {
	// Concatenar procesos a inicio de memoria
	P[0].start = 0;
	int i, j, end = P[0].size;
	cout<<"COMPACT= \t";

	for (i = 0; i < end; ++i)
	{
		M[i] = P[0].tag;
	}
	for (i = 1; i < P.size(); ++i)
	{
		P[i].start = P[i-1].size + P[i-1].start; 
		end += P[i].size;

		// Reordenar procesos en memoria RAM
		for (j = P[i].start; j < end; ++j)
		{
			M[j] = P[i].tag;
		}
	};

	for (i = end; i < block_qty; ++i)
	{
		M[i] = ' ';
	}

   	// Compactar huecos de memoria 
   	H.clear();
   	H.push_back(reg(' ', end, block_qty - end));

   	ShowMemory(H, P, M);
}