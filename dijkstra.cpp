#include <iostream>
#include <fstream>
#include <algorithm>
#include <climits>
#include <sstream>
#include <string>
#include <cctype>
#include "graph.h"
#include "hashtable.h"

using namespace std;

//This struct represents a single row in the dijkstra's algorithm table
struct row
{

	//These are the four properties of each row needed to find the shortest path in the given graph
	string vertex;
	bool mark;
	int distance;
	string previous;

	//default constructor that initializes all values to the default
	row()
	{
		vertex = "ZZZ";
		mark = false;
		distance = INT_MAX;
		previous = "N/A";
	}

	//constructor that takes in a vertex
	row(string vertex)
	{
		this -> vertex = vertex;
		mark = false;
		distance = INT_MAX;
		previous = "N/A";
	}

	//Operator overloading of < so that rows will be compared by their vertices
	bool operator < (row otherRow) const
	{
		return vertex < otherRow.vertex;
	}

};


//Prototypes
string pad(string String);
int fillGraph(Graph<string> &myGraph, string fileName, vector<row> &table);
void printMenu(vector<row> table, int numVertices);
bool isValid(vector<row> table, string startingV);
int locate(vector<row> table, string vertex);
void printRow(row currentRow);
bool isDuplicate(vector<string> temp, string vertex);
bool isEdge(Queue<string> edgeQ, string adjV);
void heapify(Graph<string> myGraph, row currentV, vector<string> &temp, vector<row> table, int size, int index, Queue<string> edgeQ);
void heapSort(Graph<string> myGraph, row currentV, Queue<string> &myQ, vector<row> table, Queue<string> &edgeQ);
void makeQueue(Graph<string> myGraph, Queue<string> &myQ, vector<row> table, row currentV, Queue<string> &edgeQ);
void aggregateDistance(Graph<string> &myGraph, Queue<string> myQ, vector<row> &table, row currentV, int &edgesVisited, Queue<string> edgeQ);
string determineMin(Graph<string> &myGraph, Queue<string> myQ, vector<row> &table, row currentV);
void detectCycle(Graph<string> &myGraph, Queue<string> myQ, vector<row> &table, Queue<string> &cycleQ, int index);
void printCycle(Queue<string> cycleQ);
void dijkstra(Graph<string> &myGraph, Queue<string> myQ, vector<row> table, string startingV, bool &isFirstPass, Queue<string> edgeQ);


int main(int argc, char *argv[])
{	
	//Makes sure an input file was passed in on the command line. If not,
	//then the program terminates.
	if(argc != 2)
	{
		cout << "Invalid Command Line Arguments" << endl;
		exit(1);
	}

	//Graph filled with vertices and edges from the input file
	Graph<string> myGraph(50);
	//Queue that will contain all unmarked vertices in Dijsktra's
	Queue<string> myQ(100);
	//Queue that will hold the edges of the current vertex in Dijsktra's
	Queue<string> edgeQ(100);

	//A vector of rows that represents the table used to preform Dijkstra's
	vector<row> table;
	//The number of vertices placed in the graph
	int numVertices;
	//starting vertex
	string startingV;
	//Indicates to Dijkstra's whether or not it is the first recursive call
	bool isFirstPass = true;

	//numVertices is initialized with the number of vertices added to myGraph.
	//myGraph and table are also simultaneously initialized from the input file
	numVertices = fillGraph(myGraph, argv[1], table);
	
	//Table is sorted so that vertices appear in alphabetical order
	sort(table.begin(), table.end());
	//The initial menu is printed
	printMenu(table, numVertices);

	//Program asks the user for a starting vertex
	cout << "Please Input Your Starting Vertex : ";

	
	getline(cin, startingV);
 
 


	//If the starting vertex entered by the user is not one that can be found in the table,
	//then the user is once more prompted for a vertex and will continue to be prompted
	//until they enter a correct vertex
	while(!isValid(table, startingV))
	{
		cout << "\n" << "Invalid Vertex! Please Enter A Valid Vertex To Continue! ";
		getline(cin, startingV);
	}
 
 cout << "\n" << "\n" << pad("Vertex") << pad("Distance") << pad("Previous") << "\n" << endl;
  
	//With all of the preliminary steps out of the way, Dijkstra's is called
	dijkstra(myGraph, myQ, table, startingV, isFirstPass, edgeQ);	
	

	return 0;
}

//This method takes in a string and adds blank space to it until its total length is 30
//Used for outputting information neatly
string pad(string String)
{
	string pading = "";

	for(int i = 0; i < 30 - String.length(); i++)
	{
		pading += " ";
	}

	return pading + String;
}

//This method fills both the graph and the table of rows from the input file and returns the number
//of vertices in the graph
int fillGraph(Graph<string> &myGraph, string fileName, vector<row> &table)
{
	//Variables
	ifstream inFile(fileName);
	string line;
	string vertex1;
	string vertex2;
	string weightString;
	int weight;
	//Not found value for the hash table
	string notFound = "ZZZ";
	//counter variable for the vertices
	int countV = 0;
	
	//Creates a new hashtable
	hashtable verticies(50, notFound);

	//This loop grabs text from the input file line by line
	//Then, a string stream is used to deliminate between each piece
	while(getline(inFile, line))
	{
		stringstream mySStream(line);

		getline(mySStream, vertex1, ';');
	
		getline(mySStream, vertex2, ';');
	
		getline(mySStream, weightString);
	
		
		//If the vertex is not found in the hash table, add it to the hash table and graph and
		//create a new row in the table
		if(!verticies.find(vertex1, notFound))
		{
			myGraph.AddVertex(vertex1);
			verticies.insert(vertex1, notFound);
			row Row(vertex1);
			table.push_back(Row);
			countV++;
		}
   
		if(!verticies.find(vertex2, notFound))
		{
			myGraph.AddVertex(vertex2);
			verticies.insert(vertex2, notFound);
			row Row(vertex2);
			table.push_back(Row);
			countV++;
		}
   	//Add the edge between the two vertices to the graph
	myGraph.AddEdge(vertex1, vertex2, stoi(weightString));
	
	}


	return countV;
}

//This method prints the menu telling the user what vertices are present in the graph
void printMenu(vector<row> table, int numVertices)
{
	cout << "^^^^^^^^^^^^^^^^   DIJKSTRA'S ALGORITHM   ^^^^^^^^^^^^^^^^" << "\n"
	   <<"\n"
	     << "A Weighted Graph Has Been Built For These " << numVertices << " Vertices" << "\n" <<  endl;
	
	//Loop to print vertices
	for(int i = 0; i < table.size(); i++)
	{
		cout << pad(table[i].vertex);

		if((i + 1) % 3 == 0)
			cout << endl;
	}

	cout << "\n" << "\n" << "------------------------------" << endl;
}

//Method that checks to see if the vertex entered by the user is in the graph/table
bool isValid(vector<row> table, string startingV)
{
  //The strings are transformed into lower case versions for comparison
  transform(startingV.begin(), startingV.end(), startingV.begin(), ::tolower);

	for(int i = 0; i < table.size(); i++)
	{
		transform(table[i].vertex.begin(), table[i].vertex.end(), table[i].vertex.begin(), ::tolower);

		if(startingV == table[i].vertex)
			return true;
	}

	return false;
	
}

//This method is used to find the given vertex's index in the table
int locate(vector<row> table, string vertex)
{
	transform(vertex.begin(), vertex.end(), vertex.begin(), ::tolower);

	for(int i = 0; i < table.size(); i++)
	{
		transform(table[i].vertex.begin(), table[i].vertex.end(), table[i].vertex.begin(), ::tolower);

		if(vertex == table[i].vertex)
			return i;
	}
 
	return 0;
}

//This method checks to see if the vertex being added to the queue containing the unmarked vertices is a duplicate.
//If it is, it is not added
bool isDuplicate(vector<string> temp, string vertex)
{	
	for(int i = 0; i < temp.size(); i++)
	{
		if(temp[i] == vertex)
			return true;
	}
  
	return false;
}

//This method checks the vertices contained in the queue containing the edges of the current vertex to see if they are edges of the adjacent vertrex
bool isEdge(Queue<string> edgeQ, string adjV)
{
	while(!edgeQ.isEmpty())
	{
		if(edgeQ.dequeue() == adjV)
			return true;
	}
  
	return false;
}


//This method heapifies the queue containing all the unmarked vertices
//so that they can be sorted by heap sort
//It creates a max heap, and heap sort reverses it to make a min heap
void heapify(Graph<string> myGraph, row currentV, vector<string> &temp, vector<row> table, int size, int index, Queue<string> edgeQ)
{

  //starting index, left child, right child, and their weights are initialized
  int max = index;
  int left = 2 * index + 1;
  int right = 2 * index + 2;
  int weightLeft = INT_MAX;
  int weightRight = INT_MAX;
  int weightMax = INT_MAX;
  
  
	//If the left child is an edge, check to see if its new value or previous value in the table
	//is greater, and set its value equal to that
	if(left < size && isEdge(edgeQ, temp[left]))
	{
		if(myGraph.WeightIs(currentV.vertex, temp[left]) + currentV.distance < table[locate(table, temp[left])].distance)
			weightLeft = myGraph.WeightIs(currentV.vertex, temp[left]) + currentV.distance;
		else
			weightLeft = table[locate(table, temp[left])].distance;
    
	}
	else if(left < size )
	{
		weightLeft = table[locate(table, temp[left])].distance;
	}
  
  	//The same is done for the right child
	if(right < size && isEdge(edgeQ, temp[right]))
	{
		if(myGraph.WeightIs(currentV.vertex, temp[right]) + currentV.distance < table[locate(table, temp[right])].distance)
			weightRight = myGraph.WeightIs(currentV.vertex, temp[right]) + currentV.distance;
		else
			weightRight = table[locate(table, temp[right])].distance;
    
	}
	else if(right < size)
	{ 
		weightRight = table[locate(table, temp[right])].distance;
	}
  
  
	//The same is done for the starting vertex
	if(isEdge(edgeQ, temp[max]))
	{
		if(myGraph.WeightIs(currentV.vertex, temp[max]) + currentV.distance < table[locate(table, temp[max])].distance)
			weightMax = myGraph.WeightIs(currentV.vertex, temp[max]) + currentV.distance;
		else
			weightMax = table[locate(table, temp[max])].distance;
    
	}
	else
	{
		weightMax = table[locate(table, temp[max])].distance; 
	}
  
 	//If the left child is greater than the current max, set max to the left child's value 
	if(left < size && weightLeft > weightMax)
	{
   
		max = left; 
		weightMax = weightLeft;
	}
    
	//The same is checked with the right child
	if(right < size && weightRight > weightMax)
	{
		max = right; 
		weightMax = weightRight;
	}
    
        //If max has been changed, swap the new max and the starting vertex and heapify again
	if(max != index)
	{
		swap(temp[index], temp[max]);
   
		heapify(myGraph, currentV, temp, table, size, max, edgeQ);
	}
  
}

//Heap sort heapifies the contents of the queue containing the unmarked vertices and then sorts them into a min heap
void heapSort(Graph<string> myGraph, row currentV, Queue<string> &myQ, vector<row> table, Queue<string> &edgeQ)
{
  
  
  vector<string> temp;
  int size;
  //bool newEntry = false;
  
	//Places the edges of the current vertex in the edge queue
	myGraph.GetToVertices(currentV.vertex, edgeQ);

	//while the queue is not empty, add non duplicates to the vector so that they can be processed
	while(!myQ.isEmpty())
	{
  
		if(!table[locate(table, myQ.getFront())].mark  && !isDuplicate(temp, myQ.getFront()))
		{
   
			if(table[locate(table, myQ.getFront())].distance == INT_MAX)
			{
				//newEntry = true;
			}
    
			temp.push_back(myQ.dequeue());
		}
		else
		{
			myQ.dequeue();
		}
	}
  
	size = temp.size();
  

	//if(newEntry)
	//{
  
	//Heapify the vector, starting at the middle vertex and working backwards
	for(int i = size/2 - 1; i >= 0; i--)
		heapify(myGraph, currentV, temp, table, size, i, edgeQ);
    
    
	//Sort the vertices into a min heap and haepify as needed
	for(int i = size - 1; i > 0; i--)
	{
    
		swap(temp[0], temp[i]);
   
		heapify(myGraph, currentV, temp, table, i, 0, edgeQ);
	}
  
	//}
 
	//Place the vertices back in the queue
	for(int i = 0; i < temp.size(); i++)
	{
		myQ.enqueue(temp[i]);
	}
  
  
}

//Initializes the queue for each pass and sort the queue
void makeQueue(Graph<string> myGraph, Queue<string> &myQ, vector<row> table, row currentV, Queue<string> &edgeQ)
{
	//Empty the queue
	edgeQ.makeEmpty();
	//Add the vertices that the current vertex can reach into the queue
	myGraph.GetToVertices(currentV.vertex, myQ);

	heapSort(myGraph, currentV, myQ, table, edgeQ);
}

//Prints a single row of the table
void printRow(row currentRow)
{
	cout << pad(currentRow.vertex) << pad(to_string(currentRow.distance)) << pad(currentRow.previous) << endl;
}


//This method takes the non marked vertices and and aggregates the distance from the current vetex to them and 
//places this value, along with the current vertex as the previous vertex, in the table
void aggregateDistance(Graph<string> &myGraph, Queue<string> myQ, vector<row> &table, row currentV, Queue<string> edgeQ)
{
  string adjVertex;
  row adjVRow;
  int sum = 0;
  int index;
  bool unmarked = false;
 
 
 
	if(!myQ.isEmpty())
	{	
		//Initializes the adjacent vertex and row from the queue
		adjVRow = table[locate(table, myQ.getFront())];
		adjVertex = adjVRow.vertex;
		index = locate(table, adjVertex);
 
		//If the vertex is not marked and is an edge, check its weight and if it is less
		//than the value already in the table, replace that value
		if(!adjVRow.mark && isEdge(edgeQ, adjVertex))
		{
 
			sum = myGraph.WeightIs(currentV.vertex, adjVertex) + currentV.distance;
 
 
			if(sum < table[index].distance)
			{
				table[index].distance = sum;
				table[index].previous = currentV.vertex;
      
			}
 
		//Continue to aggregate distances until the queue is empty
		myQ.dequeue();
		aggregateDistance(myGraph, myQ, table, currentV, edgeQ);
      
 
		}
		else
		{	//If the vertex is marked or is not an edge, dequeue it and continue aggregating
			myQ.dequeue();
			aggregateDistance(myGraph, myQ, table, currentV, edgeQ);
		}
 
	}
 
}

//This method determines the smallest distance to the next vertex and sets the current vertex equal to that vertex
string determineMin(Graph<string> &myGraph, Queue<string> myQ, vector<row> &table, row currentV)
{
  string adjVertex;
  row adjVRow;	
  row minVRow;
  string minV;
  int index;
  
 
 
	//If the queue is not empty and the vertex is not marked, set the new vertex equal to the
	//first value in the queue
	if(!myQ.isEmpty())
	{
   
		adjVRow = table[locate(table, myQ.getFront())];
		adjVertex = adjVRow.vertex;
		index = locate(table, adjVertex);
   
		if(!adjVRow.mark)
		{
			minVRow = adjVRow;
			minV = minVRow.vertex;
     
			return minV;
		}
		else
		{	//Otherwise, check the next vertex in the queue
			myQ.dequeue();
			determineMin(myGraph, myQ, table, currentV);
		}
 
	}
	else
	{	//If the queue is empty, all vertices have been processed
		return "AAA";
	}
 
 
	return minV;
}




//Dijkstras utilizes a queue built as a min heap to determine the shortest path from the given
//initial vertex to all other vertices it can reach.
void dijkstra(Graph<string> &myGraph, Queue<string> myQ, vector<row> table, string startingV, bool &isFirstPass, Queue<string> edgeQ)
{


  string newCurrentV;
  int index;
  index = locate(table, startingV);

	//If this is the first recursive pass, set the current vertex's distance = 0	
	if(isFirstPass)
	{
		table[index].distance = 0;
		isFirstPass = false;
	}
 
	//Mark the current vertex
	table[index].mark = true;

	//Set the current row to the one containing the current vertex, and print the row
	row currentV = table[index];
	printRow(currentV);

	//Make the queue and sort it
	makeQueue(myGraph, myQ, table, currentV, edgeQ);
  
	//If the queue is empty, that means all vertices have been processed
	if(myQ.isEmpty())
	{
  
	cout << "------------------------------" << endl;
	
  
		return;
	}

	//Aggregate all the distances of the current vertex's adjacent vertices
	aggregateDistance(myGraph, myQ, table, currentV, edgeQ);
 
	//The new minimum vertex is determined
	newCurrentV = determineMin(myGraph, myQ, table, currentV);
 
	//recursive call
	dijkstra(myGraph, myQ, table, newCurrentV, isFirstPass, edgeQ);
	
}
