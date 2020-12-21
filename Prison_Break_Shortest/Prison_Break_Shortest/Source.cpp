#include<iostream>
#include<string>
#include<iomanip>
#include<fstream>
#include <conio.h>
#include <stack>
#include <queue>
#include <vector>
using namespace std;

void LoadMap(ifstream & Rdr, vector<vector<int>> &Map)
{
	for (int ri = 0; ri < Map.size(); ri++)
	{
		for (int ci = 0; ci < Map[ri].size(); ci++)
		{
			Rdr >> Map[ri][ci];
		}
	}
}
void PrintMap(const vector<vector<int>> &Map)
{
	for (int ri = 0; ri < Map.size(); ri++)
	{
		for (int ci = 0; ci < Map[ri].size(); ci++)
		{
			if (Map[ri][ci] != 4)
				cout << Map[ri][ci];
			else
				cout << char(-37);
		}
		cout << endl;
	}
	cout << endl;
}

struct Position
{
	int ri;
	int ci;
};

void PrisonPosition(vector<vector<int>> & Map, int & Pr, int & Pc)
{
	for (int i = 0; i < Map.size(); i++)
	{
		for (int c = 0; c < Map[i].size(); c++)
		{
			if (Map[i][c] == 2)
			{
				Pr = i;
				Pc = c;
				return;
			}
		}
	}
}

bool CheckOneDirection(vector<vector<int>>&Map, int &calli, int nr, int nc,
						stack<Position>& Sp, stack<int>&Scc)
{
	bool NextFound = false;
	Position Next;
	if (nr<0 || nr == Map.size() || nc<0 || nc == Map[0].size() || Map[nr][nc] == 1 || Map[nr][nc] == 2)
	{
		calli++; Scc.pop();
		Scc.push(calli);
		return false;
	}
	if (Map[nr][nc] == 3)
	{
		// Removing all from SP and inserting it into Path
		Next.ri = nr;
		Next.ci = nc;
		Sp.push(Next);
		return true;
	}
	if (Map[nr][nc] == 0)
	{
		NextFound = true;
		Next.ri = nr;
		Next.ci = nc;
		Map[nr][nc] = 2;
	}
	calli++;
	Scc.pop();
	Scc.push(calli);
	if (NextFound)
	{
		Sp.push(Next);
		Scc.push(0);
	}
	return false;
}

ostream& operator << (ostream&, const Position & P)
{
	cout << "(" << P.ri << "," << P.ci << ")";
	return cout;
}
template<typename T>
void PrintStack(stack<T> S)
{
	int length = 0;
	while (S.empty() == false)
	{
		length++;
		cout << "\t"<<S.top(), S.pop();
		if (S.empty() == false)
			cout << "==>";
		
		if(length%5==0)
			cout << endl;
	}
	cout << endl << "Path Length was: "<<length;
}

bool PrisonBreakFinder(vector<vector<int>> Map, stack<Position>& Path)
{
	int Pr, Pc;
	PrisonPosition(Map, Pr, Pc);	
	stack<Position> Sp;   // holds all the trace path where you have traced through....
	stack<int> Scc;       // along the path on every position which direction has already been traced/checked
	Position pp;		  // the trace of the pen - PenPosition
	pp.ri = Pr, pp.ci = Pc;
	Sp.push(pp);
	Scc.push(0);
	while (Sp.empty()==false)
	{
		Position pp = Sp.top();
		int calli = Scc.top();
		bool PathFound = false;
		if (calli==0)
			PathFound = CheckOneDirection(Map, calli, pp.ri, pp.ci + 1, Sp, Scc);   //  -->
		else if (calli == 1)
			PathFound = CheckOneDirection(Map, calli, pp.ri-1, pp.ci , Sp, Scc);   // ^
		else if (calli == 2)			
			PathFound = CheckOneDirection(Map, calli, pp.ri, pp.ci-1, Sp, Scc);	  // <--
		else if (calli == 3)
			PathFound = CheckOneDirection(Map, calli, pp.ri+1, pp.ci, Sp, Scc);  // !
		else
		{
			Sp.pop();
			Scc.pop();
		}
		if (PathFound == true)
		{
			while (Sp.empty() == false)
				Path.push(Sp.top()), Sp.pop();
			return true;
		}
	}
	return false;
}

bool PrisonBreakFinderShortestWithoutPositions(vector<vector<int>> Map, 
													stack<Position>& Path)
{
	queue<Position> Qp;
	int Pr, Pc;
	PrisonPosition(Map, Pr, Pc);
	Position pp;		  // the trace of the pen - PenPosition
	pp.ri = Pr, pp.ci = Pc;
	Map[pp.ri][pp.ci] = 4;
	Qp.push(pp);
	
	while (Qp.empty() != false)
	{
		pp = Qp.front();
		Qp.pop();
		int nr = pp.ri, nc = pp.ci;
		if (nr >= 0 && nr<Map.size() && nc >= 0 && nc<Map[0].size() 
												&& Map[pp.ri][pp.ci] == 3)
		{
			return true;
		}

		if (nr >= 0 && nr<Map.size() && nc >= 0 && nc<Map[0].size() && Map[pp.ri][pp.ci] != 1 && Map[pp.ri][pp.ci] != 2)
		{
			Map[nr][nc] = 2;
			pp.ri = nr, pp.ci = nc + 1;   //  ===>
			Qp.push(pp);
			pp.ri = nr - 1, pp.ci = nc;   //  ^
			Qp.push(pp);
			pp.ri = nr, pp.ci = nc - 1;   //  <===
			Qp.push(pp);
			pp.ri = nr + 1, pp.ci = nc;   //  \/
			Qp.push(pp);
		}
	}
	return false;
}

bool PrisonBreakFinderShortest(vector<vector<int>> Map, stack<Position>& Path)
{
	queue<Position> Qp;
	queue <stack<Position>> Qpaths;

	int Pr, Pc;
	PrisonPosition(Map, Pr, Pc);
	Position pp;		  // the trace of the pen - PenPosition
	pp.ri = Pr, pp.ci = Pc;
	Map[pp.ri][pp.ci] = 4;
	
	Qp.push(pp);
	stack<Position> S;
	Qpaths.push(S);
	while (Qp.empty() != false)
	{
		pp = Qp.front();
		S = Qpaths.front();
		Qpaths.pop();
		Qp.pop();
		int nr = pp.ri, nc=pp.ci;
		if (nr >= 0 && nr<Map.size() && nc >= 0 && nc<Map[0].size() && Map[pp.ri][pp.ci] == 3)
		{
			S.push(pp);
			while (S.empty() == false)
			{
				Path.push(S.top()), S.pop();
			}
			return true;
		}

		if (nr >= 0 && nr<Map.size() && nc >= 0 && nc<Map[0].size() && Map[pp.ri][pp.ci] != 1 && Map[pp.ri][pp.ci] != 2)
		{
			S.push(pp);
			//Position cpp;
			Map[nr][nc] = 2;

			pp.ri = nr, pp.ci = nc + 1;   //  ===>
			Qp.push(pp);
			Qpaths.push(S);

			pp.ri = nr-1, pp.ci = nc;   //  ^
			Qp.push(pp);
			Qpaths.push(S);

			pp.ri = nr, pp.ci = nc - 1;   //  <===
			Qp.push(pp);
			Qpaths.push(S);

			pp.ri = nr+1, pp.ci = nc;   //  \/
			Qp.push(pp);
			Qpaths.push(S);
		}
	}
	return false;
}

void Updatepaths(vector<vector<int>>&MapWithPath,stack<Position> S)
{
	Position ppos;
	S.pop();
	while (S.empty() == false)
	{
		ppos = S.top();
		S.pop();
		if (S.empty()==false)
			MapWithPath[ppos.ri][ppos.ci] = 4;
	}
}

bool PrisonBreakExplore(vector<vector<int>> & Map, int Pr, int Pc, stack<Position>& Path)
{
	if (Pr < 0 || Pr >= Map.size() || Pc < 0 || Pc >= Map[0].size())
		return false;
	if (Map[Pr][Pc] == 1)
		return false;
	if (Map[Pr][Pc] == 3)
	{
		Position p; p.ri = Pr, p.ci = Pc;
		Path.push(p);
		return true;
	}
	if (Map[Pr][Pc] == 4)   // 4 is the remembering line...
		return false;
	Map[Pr][Pc] = 4;
	bool Found = PrisonBreakExplore(Map, Pr , Pc + 1, Path) ||
		PrisonBreakExplore(Map, Pr, Pc - 1, Path) ||
		PrisonBreakExplore(Map, Pr + 1, Pc, Path) ||
		PrisonBreakExplore(Map, Pr - 1, Pc, Path);
	if (Found)
	{
		Position p; p.ri = Pr, p.ci = Pc;
		Path.push(p);
	}
	return Found;
}
					
bool PrisonBreakDFS(vector<vector<int>> Map, stack<Position>& Path)
{					// Map is passed by value as for trace remembering Map will 
					// be changed and I don't want that the original Map is 
					// destroyed.
	int Pr, Pc;
	PrisonPosition(Map, Pr, Pc);
	return PrisonBreakExplore(Map, Pr, Pc, Path);
}

//PrisonBreakFinderCompact(Map, Apath);
bool CompactPathFinderVisit(int &calli, vector<vector<int>>&Map,
	Position aposition, stack<int>&Scc,
	stack<Position>& Sp, int nri, int nci)
{
	if (nri < 0 || nri == Map.size() || nci < 0 || nci == Map[0].size() ||
		Map[nri][nci] == 2 || Map[nri][nci] == 1)
	{
		calli++;
		Scc.pop();
		Scc.push(calli);
		return false;
	}
	bool NextFound = false;
	Position Next;
	Next.ri = nri;
	Next.ci = nci;

	if (Map[nri][nci] == 3)
	{
		// Removing all from SP and inserting it into Path
		Sp.push(Next);
		return true;
	}
	if (Map[nri][nci] == 0)
	{
		NextFound = true;
		Map[nri][nci] = 2;
	}
	calli++;
	Scc.pop();
	Scc.push(calli);
	if (NextFound)
	{
		Sp.push(Next);
		Scc.push(0);
	}
	return false;
}
void PrintPath(stack<Position> Apath)
{
	int i = 0;
	while (!Apath.empty())
	{
		Position p = Apath.top();
		cout << "(" << p.ri << ", " << p.ci << ")";
		Apath.pop();
		if (i == 0)
			cout << "\t";
		i++;
		if (i % 5 == 0)
			cout << "\n\t";

		if (Apath.empty() == false)
			cout << "\t==> ";
	}
	cout << "\n\nThe length of the path is: " << i << " Steps...";
	cout << endl;
}
bool PrisonBreakFinderCompact(vector<vector<int>> & Map, stack<Position>& Path)
{
	int Pr, Pc;
	PrisonPosition(Map, Pr, Pc);
	stack<Position> Sp;
	stack<int> Scc;
	Position pp;
	pp.ri = Pr, pp.ci = Pc;
	Sp.push(pp);
	Scc.push(0);
	bool PathFound = false;
	while (Sp.empty() == false)
	{
		Position aposition = Sp.top();
		int calli = Scc.top();
		PathFound = false;
		if (calli == 0)
		{
			PathFound = CompactPathFinderVisit(calli, Map, aposition, Scc, Sp, aposition.ri, aposition.ci + 1);
		}
		else if (calli == 1)
		{
			PathFound = CompactPathFinderVisit(calli, Map, aposition, Scc, Sp, aposition.ri - 1, aposition.ci);
		}
		else if (calli == 2)
		{
			PathFound = CompactPathFinderVisit(calli, Map, aposition, Scc, Sp, aposition.ri, aposition.ci - 1);
		}
		else if (calli == 3)
		{
			PathFound = CompactPathFinderVisit(calli, Map, aposition, Scc, Sp, aposition.ri + 1, aposition.ci);
		}
		else
		{
			Sp.pop();    // Remove the last entry as all path already tried...
			Scc.pop();
		}

		if (PathFound)
		{
			while (!Sp.empty())
				Path.push(Sp.top()), Sp.pop();
			return true;
		}
		//		PrintPath(Sp);
		//		_getch();
	}
	return false;
}

int main()
{
	int R, C;
	ifstream Rdr("PrinsonBreak.txt");
	Rdr >> R >> C;
	stack<Position> Apath;
	vector<vector<int>> Map;
	Map.resize(R);
	for (int i = 0; i < R; i++)
		Map[i].resize(C);							// vector<int> ARow(C);
													// Map[i]	= ARow;
	LoadMap(Rdr, Map);
	PrintMap(Map);
	//	if (PrisonBreakFinderShortest(Map, Apath))
	if (PrisonBreakDFS(Map, Apath))
	{
		cout << "Path Found..." << endl;
		vector<vector<int>> MapWithPath = Map;
		Updatepaths(MapWithPath, Apath);
		PrintMap(MapWithPath);
		PrintStack(Apath);
	}
	else
		cout << "Path NOT Found..." << endl;
	system("pause");
	return 0;
}

//#include "GrowableArray.h"
/*
#include "Student.h"
#include "Queue.h"


int main()
{
	Queue<int> Q(3);
	Q.Print();
	Q.Push(1);
	Q.Push(2);
	Q.Push(3);
	Q.Push(4);
	Q.Print();
	cout << Q.top() << endl;
	Q.Pop();
	cout << Q.top() << endl;
	Q.Print();
	return 0;
}
*/

/*
	I will going to write one implementation.... and it will work for every single data type
	Here is how we will do it....
*/

bool IsOpenBracket(char ch)
{
	return ch == '(' || ch == '{' || ch == '[';
}
bool Match(char opench, char cch)
{
	return (opench == '(' && cch == ')') || (opench == '{' && cch == '}')
		|| (opench == '[' && cch == ']');
}

bool IsBalanced(string E)
{
	stack<char> S;
	char ch;
	for (int i = 0; i < E.length(); i++)
	{
		char ch = E[i];
		if (IsOpenBracket(ch))
			S.push(ch);
		else if (!S.empty() && Match(S.top(), ch))
			S.pop();
		else
			return false;
	}
	return S.empty();
}

int main3()
{
	ifstream Rdr("BB.txt");
	int TC;
	Rdr >> TC;
	Rdr.ignore();
	for (int i = 0; i < TC; i++)
	{
		char ch;
		string ACase;
		do
		{
			ch = Rdr.get();
			if ( Rdr && ch != '\n') // First Condition makes sure whether we were able to read from the file or NOT... it will be false in the case of EOF
				ACase += ch;
		}
		while (Rdr && ch != '\n');

		if (IsBalanced(ACase))
			cout << "Y";
		else
			cout << "N";

		cout << endl;
	}
	return 0;
	//{
	//	STACK<int> Sints;
	//	int s;
	//	ifstream SRdr("Ns.txt");
	//	SRdr >> s;
	//	for (int i = 0; i < s; i++)
	//	{
	//		int d;
	//		SRdr >> d;
	//		Sints.push_back(d);
	//	}

	//	while (Sints.is_empty() == false)
	//	{
	//		cout << Sints.top() << " ";
	//		Sints.pop_back();
	//	}
	//}
	//_getch();
	//return 0;
	//{

	//	int S;
	//	ifstream Rdr("R.txt");
	//	Rdr >> S;
	//	GrowableArray<Student> Ss(S);
	//	Rdr >> Ss;
	//	cout << Ss;

	//	/*
	//	int Size;
	//	cout << "Size: ";
	//	cin >> Size;
	//	GrowableArray<int> GAi(Size);
	//	GrowableArray<float> GAf(Size);
	//	cin >> GAi;

	//	GrowableArray<int> GBi=GAi;
	//	GBi[0] = 100;
	//	GAi.push_back(20);
	//	cout << GAi << endl
	//	<< GBi <<endl;
	//	*/
	//}
}
//
//template<typename T>  // Let it be any data type T.... lets call it T
//void SWAP(T & A, T &B)
//{
//	T temp = A;
//	A = B;
//	B = temp;
//}
//template<typename Type>
//void Sort(Type A[], int Size)
//{
//	bool SH;
//	do
//	{
//		SH = false;
//		for (int j = 0;j+1<Size; j++)
//		{
//			if (A[j] > A[j + 1])
//			{
//				SH = true;
//				SWAP(A[j], A[j + 1]);
//			}
//		}
//	}
//	while (SH);
//}
//template<typename T>   // template <class T>
//void Print(const char* N, T A[], int S)
//{
//	cout << N << " = {";
//	for (int i = 0; i < S; i++)
//		cout << setw(5) << left << A[i];
//	cout << " }" << endl;
//}
//int main1()
//{
//	int A[] = { 10, 5, 7, 8, 3, 2, 1, 0, -5 };
//	double D[] = { 10.5, 5.2, 7.6, 8.9, 3.25, 2.9, 1.99, 0.976, -5.8 };
//	char C[] = { 'A', 'D', 'A', 'P', 'F', 'Z', 'Q', 'N', 'M'};
//	Sort(A, sizeof(A) / sizeof(int));   // Sort(A, 36/4);
//	Sort(D, sizeof(D) / sizeof(double));   // Sort(A, 72/8);
//	Sort(C, sizeof(C) / sizeof(char));   // Sort(A, 9/1);
//	
//	Print("A", A, sizeof(A) / sizeof(int));
//	Print("D", D, sizeof(D) / sizeof(double));
//	Print("C", C, sizeof(C) / sizeof(char));
//
//	Student Ss[100];
//	int Size;
//	ifstream Rdr("R.txt");
//	Rdr >> Size;
//	for (int i = 0; i < Size; i++)
//	{
//		string n;
//		int rn;
//		double g;
//		Rdr >> n >> rn >> g;
//		Ss[i].SetNRG(n, rn, g);
//	}
//	
//	Sort(Ss, Size);   // i will not use sizeof here.... because sizeof will give me 100 which is incorrect....
//	Print("Ss", Ss, Size);
//	return 0;
//}
//
////
////void SWAP(char &a, char &b)
////{
////	char temp = a;
////	a = b;
////	b = temp;
////}
////
////void Sort(char A[], int Size)
////{
////	bool CH = true;
////	while (CH)
////	{
////		CH = false;
////		for (int j = 0; j + 1 < Size; j++)
////		{
////			if (A[j] > A[j + 1])
////			{
////				CH = true;
////				SWAP(A[j], A[j + 1]);
////			}
////		}
////	}
////}
////void SWAP(double &a, double &b)
////{
////	double temp = a;
////	a = b;
////	b = temp;
////}
////
////void Sort(double A[], int Size)
////{
////	bool CH = true;
////	while (CH)
////	{
////		CH = false;
////		for (int j = 0; j + 1 < Size; j++)
////		{
////			if (A[j] > A[j + 1])
////			{
////				CH = true;
////				SWAP(A[j], A[j + 1]);
////			}
////		}
////	}
////}
////
////void SWAP(int &a, int &b)
////{
////	int temp = a;
////	a = b;
////	b = temp;
////}
////
////void Sort(int A[], int Size)
////{
////	bool CH = true;
////	while (CH)
////	{
////		CH = false;
////		for (int j = 0; j+1 < Size; j++)
////		{
////			if (A[j] > A[j + 1])
////			{
////				CH = true;
////				SWAP(A[j], A[j + 1]);
////			}
////		}
////	}
////}
////void Print(int A[], int S)
////{
////	for (int i = 0; i < S; i++)
////		cout << left << setw(6) << A[i] << " ";
////	cout << endl;
////}
////
////void Print(char A[], int S)
////{
////	for (int i = 0; i < S; i++)
////		cout << left << setw(6) << A[i] << " ";
////	cout << endl;
////}
////
////void Print(double A[], int S)
////{
////	for (int i = 0; i < S; i++)
////		cout << left << setw(6)<<A[i] << " ";
////	cout << endl;
////}
////int main1()
////{
////	int A[] = {10,9,7,2,4,5,6,7,9,10};
////	double F[] = { 10.2, 9.3, 7.2, 2.9, 4.1, 5.9, 6.8, 7.3, 9.25, 10.12 };
////	char C[] = { 'Z', 'B', 'D', 'T', 'F' };
////	Sort(A, sizeof(A) / sizeof(int));
////	Sort(F, sizeof(F) / sizeof(double));
////	Sort(C, sizeof(C) / sizeof(char));
////	PrintInts(A, sizeof(A) / sizeof(int));
////	PrintDs(F, sizeof(F) / sizeof(double));
////	PrintCs(C, sizeof(C) / sizeof(char));
////}
