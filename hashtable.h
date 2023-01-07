//Class representing a hash table
class hashtable
{
	public:
		hashtable(int numLocs, string notFound);
		void insert(string key, string notFound);
		bool find(string key, string notFound);

	private:
		string *array;
		int numLocs;
		int fold(string key);
};

#include "hashtable.cpp"
