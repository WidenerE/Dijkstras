//Implementation of hash table

//Constructor
hashtable::hashtable(int size, string notFound)
{
	numLocs = size * 2;
	array = new string[numLocs];
	for(int i = 0; i < numLocs; i++)
		array[i] = notFound;
}

//Folds string into a value accepted by the hash table
int hashtable::fold(string key)
{
	string String;
	int sum = 0;

	for(int i = 0; i < key.length(); i++)
		String += tolower(key[i]);

	for(int i = 0; i < String.length(); i++)
		sum += String[i];

	return sum;
}

//Insert key into hash table
void hashtable::insert(string key, string notFound)
{
	int slot = fold(key);
	int i = slot % numLocs;

	while(array[i] != notFound)
		i = (i + 1) % numLocs;

	array[i] = key;
	
}

//Find the requested key in the hash table
bool hashtable::find(string key, string notFound)
{
	int slot = fold(key);
	int i = slot % numLocs;
	
	while(array[i] != notFound)
	{
		if(array[i] == key)
			return true;
		i = (i + 1) % numLocs;
	}

	return false;
}
