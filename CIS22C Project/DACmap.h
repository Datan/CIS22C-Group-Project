#pragma once
#include <map>
/*
Written by C. Lee-Klawender

The DACmap is the type used in LinkedGraph and Vertex classes

These classes wwere written so you don't have to understand how to use
the C++ STL map nor its iterator, and has easy-to-use function for access
*/
using namespace std;

// Iterator to retrieve ONLY the ItemTypes in the order of the map
// NOTE: IF YOUR PROJECT NEEDS TO ITERATE TO GET BOTH THE KEY AND THE ITEM,
//     LET ME KNOW AND I'LL WRITE FUNCTIONS FOR THAT and show you how to use it
template <class KeyType, class ItemType>
class DACmapIterator
{
private:
	map<KeyType, ItemType> *pdacMap;
	typename map<KeyType, ItemType>::iterator mapIterator;

public:
	DACmapIterator(map<KeyType, ItemType> *p){
		pdacMap = p;
		mapIterator = (pdacMap->begin());

	}
	ItemType next(){ return (mapIterator++)->second; }
	bool hasNext() const { return mapIterator != (pdacMap->end());  }

};

// Just a map, but for easier use of a map (dictionary)
template <class KeyType, class ItemType>
class DACmap
{
private:
	map<KeyType, ItemType> dacMap;
	DACmapIterator<KeyType, ItemType> *thisIterator;// init. to 0

public:
	DACmap(){ thisIterator = 0; }
	DACmap(const DACmap &source){ dacMap = source.dacMap; thisIterator = 0; } // UPDATED
	~DACmap(){ if (thisIterator != 0) delete thisIterator; thisIterator = 0; }

	bool isEmpty() const { return dacMap.empty(); }
	int getNumberOfItems() const { return dacMap.size(); }
	bool add(const KeyType& searchKey, const ItemType& newItem);
	bool remove(const KeyType& searchKey);

	void clear();
	ItemType getItem(const KeyType& searchKey) const ;
	bool contains(const KeyType& searchKey) const;

	/** Traverses the items in this dictionary in sorted search-key order
	and calls a given client function once for each item. */
	void traverse(void visit(ItemType&)) const;

	// For iterator
	DACmapIterator<KeyType, ItemType> *iterator();
};

template <class KeyType, class ItemType>
bool DACmap<KeyType, ItemType>::add(const KeyType& searchKey, const ItemType& newItem)
{
	if (contains(searchKey))
		return false;
	dacMap[searchKey] = newItem;
	return true;
}

template <class KeyType, class ItemType>
bool DACmap<KeyType, ItemType>::remove(const KeyType& searchKey)
{
	return dacMap.erase(searchKey) == 1;
}

template <class KeyType, class ItemType>
void DACmap<KeyType, ItemType>::clear()
{
	dacMap.clear();
	thisIterator = 0;
}

template <class KeyType, class ItemType>
ItemType DACmap<KeyType, ItemType>::getItem(const KeyType& searchKey) const
{
	typename map<KeyType, ItemType>::const_iterator foundItem = dacMap.find(searchKey);
	if (foundItem == dacMap.end() )
		return ItemType();
	return foundItem->second;
}

template <class KeyType, class ItemType>
bool DACmap<KeyType, ItemType>::contains(const KeyType& searchKey) const
{
	typename map<KeyType, ItemType>::const_iterator itemIter = dacMap.begin();
	// CHANGE TO SEARCH IN A LOOP WITH ITERATOR?
	for (; itemIter != dacMap.end(); ++itemIter)
	{
		if (itemIter->first == searchKey)
		{
			return true;
		}
	}
	return false; //
}

template <class KeyType, class ItemType>
DACmapIterator<KeyType, ItemType> *DACmap<KeyType, ItemType>::iterator()
{
	if (thisIterator != 0)
		delete thisIterator;
	thisIterator = new DACmapIterator<KeyType, ItemType>(&dacMap);
	return thisIterator;
}
