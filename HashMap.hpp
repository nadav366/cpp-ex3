#ifndef CPP_EX3_HASHMAP_HPP
#define CPP_EX3_HASHMAP_HPP

#include <utility>
#include <vector>
#include <stdexcept>
#include <iostream>

/**  Minimum capacity on the map  */
const int MIN_CAPACITY = 1;

/**  Default values  */
const double DEF_LOW_FACTOR = 0.25;
const double DEF_HIGH_FACTOR = 0.75;
const int DEF_SIZE = 0;
const int DEF_CAPACITY = 16;

/**  Table resizing parameter  */
const int RESIZE_PARM = 2;

/** Default iterator values  */
const int INIT_TABLA_INDEX = 0;
const int INIT_LIST_INDEX = -1;


/**
 * @brief An object that is a map data structure, by an open hash table
 * @tparam KeyT The key type
 * @tparam ValueT The value type
 */
template<class KeyT, class ValueT>
class HashMap
{
    using pairs = std::pair<KeyT, ValueT>;

private:
    /**   Current table size         */
    int _capacity;
    /**   The current number of values in the map         */
    int _size;
    /**   Low Load Factor bar        */
    double _lowLoadFactor;
    /**   high Load Factor bar          */
    double _highLoadFactor;
    /**   The current data table         */
    std::vector<pairs> *_table;
    /**    tha use Hash function        */
    std::hash<KeyT> _hashFanc;

    /**
     * @brief A function that resizes the table size
     * @param newCap New table size
     */
    void _reSize(int newCap)
    {
        if (newCap < MIN_CAPACITY)
        {
            return;
        }
        auto newTable = new std::vector<pairs>[newCap];
        for (auto cur : *this)
        {
            newTable[_hashFanc(cur.first) & (newCap - 1)].push_back(cur);
        }

        delete[] _table;
        _table = newTable;
        _capacity = newCap;
    }

    /**
     * @brief A function that matches the index in the table to a particular key
     * @param key Key to index
     * @return The appropriate index
     */
    int _index(const KeyT & key) const
    {
        return (_hashFanc(key) & (capacity() - 1));
    }

public:
    /**
     * @brief Default constructor
     */
    HashMap() : HashMap(DEF_LOW_FACTOR, DEF_HIGH_FACTOR)
    {}

    /**
     * @brief constructor
     * @param lowFactor Low Load Factor bar
     * @param higeFactor high Load Factor bar
     */
    HashMap(double lowFactor, double higeFactor) :
            _capacity(DEF_CAPACITY),
            _size(DEF_SIZE),
            _lowLoadFactor(lowFactor),
            _highLoadFactor(higeFactor),
            _table(new std::vector<pairs>[_capacity])
    {
        // Input integrity check
        if (lowFactor <= 0 || lowFactor >= 1 ||
            higeFactor <= 0 || higeFactor >= 1 ||
            higeFactor < lowFactor)
        {
            delete[] _table;
            throw std::invalid_argument("The resulting arguments are invalid");
        }
    }

    /**
     * @brief Constructor that receives vector keys and vector values and adds them to the map
     * @param keyVec Vector keys
     * @param valVec Vector values
     */
    HashMap(const std::vector<KeyT> & keyVec, const std::vector<ValueT> & valVec) : HashMap()
    {
        // Check that the vectors are the same size
        if (keyVec.size() != valVec.size())
        {
            delete[] _table;
            throw std::invalid_argument("The resulting vectors are not the same size");
        }

        for (size_t i = 0; i < keyVec.size(); ++i)
        {
            (*this)[keyVec[i]] = valVec[i];
        }
    }

    /**
     * @brief Copy Constructor
     * @param other Map object to copy
     */
    HashMap(const HashMap & other) :
            _capacity(other._capacity),
            _size(other._size),
            _lowLoadFactor(other._lowLoadFactor),
            _highLoadFactor(other._highLoadFactor),
            _table(new std::vector<pairs>[other._capacity])
    {
        for (int i = 0; i < _capacity; ++i)
        {
            _table[i] = other._table[i];
        }
    }

    /**
     * @brief move Constructor
     * @param other Map object to move from
     */
    HashMap(HashMap && other) noexcept :
            _capacity(other._capacity),
            _size(other._size),
            _lowLoadFactor(other._lowLoadFactor),
            _highLoadFactor(other._highLoadFactor)
    {
        _table = other._table;
        other._table = nullptr;
    }

    /**
     * @brief destructor
     */
    ~HashMap()
    {
        delete[] _table;
    }

    /**
     * @brief A function that returns the number of values on the map
     * @return number of values on the map
     */
    int size() const
    {
        return _size;
    }

    /**
     * @brief A function that returns the map capacity
     * @return the map capacity
     */
    int capacity() const
    {
        return _capacity;
    }

    /**
     * @brief A function that returns the current load factor
     * @return the current load factor
     */
    double getLoadFactor() const
    {
        return ((double) size()) / capacity();
    }

    /**
     * @brief Function that returns whether the map is empty
     * @return true if is empty, false otherwise
     */
    bool empty() const
    {
        return (size() == DEF_SIZE);
    }

    /**
     * @brief A function that adds a new pair to the map
     * @param key Key to add
     * @param val Value to add
     * @return True with the added success, false if not
     */
    bool insert(const KeyT & key, const ValueT & val)
    {
        if (containsKey(key))
        {
            return false;
        }
        ++_size;
        if (getLoadFactor() > _highLoadFactor)
        {
            _reSize(_capacity * RESIZE_PARM);
        }
        _table[_index(key)].push_back(std::make_pair(key, val));
        return true;
    }

    /**
     * @brief A function that checks whether a particular key exists on the map
     * @param key key to chack
     * @return true if it exists, false if not
     */
    bool containsKey(const KeyT & key) const
    {
        for (auto pair :  _table[_index(key)])
        {
            if (pair.first == key)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief A function that returns a value by a particular key if it is on the map
     * @param keyToSearch A key to look for on the map
     * @return Reference the correct value (can be inserted into it)
     */
    ValueT & at(const KeyT & keyToSearch)
    {
        for (size_t i = 0; i < _table[_index(keyToSearch)].size(); i++)
        {
            if (_table[_index(keyToSearch)].at(i).first == keyToSearch)
            {
                return _table[_index(keyToSearch)].at(i).second;
            }
        }
        throw std::out_of_range("The key does not exist on the map");
    }

    /**
     * @brief A function that returns a value by a particular key if it is on the map
     * @param keyToSearch A key to look for on the map
     * @return const Reference the correct value (can't be inserted into it)
     */
    const ValueT & at(const KeyT & keyToSearch) const
    {
        for (size_t i = 0; i < _table[_index(keyToSearch)].size(); i++)
        {
            if (_table[_index(keyToSearch)].at(i).first == keyToSearch)
            {
                return _table[_index(keyToSearch)].at(i).second;
            }
        }
        throw std::out_of_range("The key does not exist on the map");
    }

    /**
     * @brief A function that deletes a pair from the map, by a specific key
     * @param ketToDel Key to delete
     * @return false if the deletion was successful, otherwise false
     */
    bool erase(const KeyT & ketToDel)
    {
        auto itr = _table[_index(ketToDel)].begin();
        while (itr != _table[_index(ketToDel)].end())
        {
            if ((*itr).first == ketToDel)
            {
                _table[_index(ketToDel)].erase(itr);
                --_size;
                if (getLoadFactor() < _lowLoadFactor)
                {
                    _reSize(_capacity / RESIZE_PARM);
                }
                return true;
            }
            ++itr;
        }
        return false;
    }

    /**
     * @brief  function that returns the basket size to a specific key - that is, the number of
     *          values mapped to that cell in the table
     * @param key Search key
     * @return basket size to a specific key
     */
    int bucketSize(const KeyT & key) const
    {
        if (!containsKey(key))
        {
            throw std::out_of_range("The key does not exist on the map");
        }
        return _table[_index(key)].size();
    }

    /**
     * @brief A function that deletes all values on the map
     */
    void clear()
    {
        delete[] _table;
        _table = new std::vector<pairs>[_capacity];
        _size = 0;
    }

    /**
     * @brief Placement Operator
     * @param other Map object to copy
     * @return Reference to the object itself
     */
    HashMap & operator=(const HashMap & other)
    {
        if (this == &other)
        {
            return *this;
        }

        delete[] _table;
        _capacity = other._capacity;
        _size = other._size;
        _lowLoadFactor = other._lowLoadFactor;
        _highLoadFactor = other._highLoadFactor;
        _table = new std::vector<pairs>[other._capacity];

        for (int i = 0; i < _capacity; ++i)
        {
            _table[i] = other._table[i];
        }
        return *this;
    }

    /**
     * @brief Operator Value Access by Key
     * @param key A key to accessing its value
     * @return Reference to the appropriate value
     */
    ValueT & operator[](const KeyT & key)
    {
        insert(key, ValueT());
        return at(key);
    }

    /**
     * @brief Operator Value Access by Key
     * @param key A key to accessing its value
     * @return const Reference to the appropriate value
     */
    const ValueT & operator[](const KeyT & key) const
    {
        for (auto & pair :  _table[_index(key)])
        {
            if (pair.first == key)
            {
                return pair.second;
            }
        }
        return _table[_index(key)].at(0).second;
    }

    /**
     * @brief Comparison operator
     * @param other Map object for comparison
     * @return True if the objects are equal, false otherwise
     */
    bool operator==(const HashMap & other) const
    {
        if (size() != other.size() || capacity() != other.capacity())
        {
            return false;
        }
        try
        {
            for (auto pair : *this)
            {
                if (other.at(pair.first) != pair.second)
                {
                    return false;
                }
            }
        }
        catch (std::out_of_range & e)
        {
            return false;
        }
        return true;
    }

    /**
     * @brief ancomparison operator
     * @param other Map object for comparison
     * @return False if the objects are equal, otherwise true
     */
    bool operator!=(const HashMap & other) const
    {
        return !(*this == other);
    }

    /**
     * @brief A constant iterator on the map values
     */
    class const_iterator
    {
    private:
        /**   Pointer to the map object          */
        const HashMap<KeyT, ValueT> *_myMap;
        /**   The current table index      */
        int _tableIndex;
        /**   Current basket index      */
        int _listIndex;

    public:
        /**
         * @brief Default constructor - to start map
         * @param myMap Pointer to the map object
         */
        explicit const_iterator(const HashMap<KeyT, ValueT> *myMap) :
                _myMap(myMap),
                _tableIndex(INIT_TABLA_INDEX),
                _listIndex(INIT_LIST_INDEX)
        {
            operator++(); // Looking for the first object
        }

        /**
         * @brief constructor - Indexes to the beginning of the iterator
         * @param tableI  Initial Table Index
         * @param listI Initial list index
         * @param myMap Pointer to the map object
         */
        const_iterator(int tableI, int listI, const HashMap<KeyT, ValueT> *myMap) :
                _myMap(myMap),
                _tableIndex(tableI),
                _listIndex(listI)
        {}

        /**
         * @brief Access operator asterisk
         * @return A reference to the appropriate pair for the iterator
         */
        const std::pair<KeyT, ValueT> & operator*() const
        {
            return _myMap->_table[_tableIndex].at(_listIndex);
        }

        /**
         * @brief Access operator arrow
         * @return Pointer to the appropriate pair
         */
        const std::pair<KeyT, ValueT> *operator->() const
        {
            return &_myMap->_table[_tableIndex].at(_listIndex);
        }

        /**
         * @brief Progress operator
         * @return Pointer to the iterator object
         */
        const_iterator & operator++()
        {
            if ((long) _myMap->_table[_tableIndex].size() > ++_listIndex)
            {
                return *this;
            }

            _listIndex = 0;
            do
            {
                ++_tableIndex;
                if ((long) _myMap->_table[_tableIndex].size() > _listIndex)
                {
                    return *this;
                }
            } while (_tableIndex < _myMap->capacity());
            return *this;
        }

        /**
         * @brief Progress operator
         * @return iterator object
         */
        const_iterator operator++(int)
        {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * @brief Comparison operator
         * @param rhs Another iterator object
         * @return True if they are equal, false otherwise
         */
        bool operator==(const_iterator const & rhs) const
        {
            // Check whether both are at the end of the iterator
            if (_tableIndex == _myMap->capacity() && rhs._tableIndex == _myMap->capacity())
            {
                return true;
            }
            // Check if one of them is at the end
            if (_tableIndex == _myMap->capacity() || rhs._tableIndex == _myMap->capacity())
            {
                return false;
            }
            return *(*this) == *rhs;
        }

        /**
         * @brief Comparison operator
         * @param rhs Another iterator object
         * @return false if they are equal, true otherwise
         */
        bool operator!=(const_iterator const & rhs) const
        {
            return !(*this == rhs);
        }
    };

    /**
     * @brief A function that returns a directed iterator to the beginning of the map
     * @return iterator to the beginning of the map
     */
    const_iterator begin() const
    {
        return const_iterator(this);
    }

    /**
     * @brief A function that returns an iterator directed to the end of the map
     * @return iterator directed to the end of the map
     */
    const_iterator end() const
    {
        return const_iterator(capacity(), INIT_LIST_INDEX, this);
    }

    /**
     * @brief A function that returns a directed iterator to the beginning of the map
     * @return iterator to the beginning of the map
     */
    const_iterator cbegin() const
    {
        return const_iterator(this);
    }

    /**
     * @brief A function that returns an iterator directed to the end of the map
     * @return iterator directed to the end of the map
     */
    const_iterator cend() const
    {
        return const_iterator(capacity(), INIT_LIST_INDEX, this);
    }
};

#endif //CPP_EX3_HASHMAP_HPP
