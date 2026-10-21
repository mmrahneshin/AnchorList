// Copyright (C) Mohammad Mahdi Rahneshin, Ferdowsi Univerity of Mashhad, 2024 (1404 Hijri Shamsi)
//
// Author: Mohammad Mahdi Rahneshin, Kamaledin Ghiasi-Shirazi

#include <iostream>
#include <stack>
#include <sstream>
#include <optional>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <filesystem>
#include <chrono>   // Add this for high precision timing
#include <iomanip>  // Add this for setprecision
#include <unistd.h> // sysconf, _SC_PAGE_SIZE
#include <malloc.h> // malloc_trim

#include "../src/IndexedPPS23RBBinaryTreeNode.h"
#include "../src/IndexedPPS23RedBlackBinaryTree.h"
#include "../src/AnchorList.h"

using namespace std;
using namespace std::chrono; // Add this

int *values;
int *indexes;
int *deleteIndexes;
high_resolution_clock::time_point start_time, end_time; // Change these to chrono time points

double timeTaken;
void insertToAnchorList(AnchorList<int> *ell, int size,
                        map<int, double> *timeTaken_map = nullptr)
{
    start_time = high_resolution_clock::now();
    for (int i = 0; i < size; i++)
    {
        // cout << "index: " << indexes[i] << "\t value:"
        //      << values[i] << endl;

        ell->insert(indexes[i], values[i]);
        // ell->drawTree();
        // ell->iterateInOrder();
        // cout << "===============" << endl;
    }
    end_time = high_resolution_clock::now();

    auto duration = duration_cast<nanoseconds>(end_time - start_time);
    timeTaken = duration.count() / 1e9; // Convert nanoseconds to seconds with high precision
    if (timeTaken_map != nullptr)
    {
        timeTaken_map->insert({size, timeTaken});
    }
    // cout << "insert time taken anchor list: " << timeTaken << endl;
}

void insertToVector(vector<int> *vec, int size,
                    map<int, double> *timeTaken_map = nullptr)
{

    start_time = high_resolution_clock::now();
    int i;
    for (i = 0; i < size; i++)
    {
        vec->insert(vec->begin() + indexes[i], values[i]);
    }
    end_time = high_resolution_clock::now();

    auto duration = duration_cast<nanoseconds>(end_time - start_time);
    timeTaken = duration.count() / 1e9; // Convert nanoseconds to seconds with high precision

    if (timeTaken_map != nullptr)
    {
        timeTaken_map->insert({size, timeTaken});
    }
    // cout << "insert time taken vector: " << timeTaken << endl;
}

// void pushFrontToList(int i, list<int> *lst, int size)
// {
//     start = clock();
//     for (; i < size; i++)
//     {
//         lst->push_front(values[i]);
//     }
//     end_clock = clock();
//     timeTaken = double(end_clock - start) / double(CLOCKS_PER_SEC);
//     cout << "insert time taken list (push_front): " << timeTaken << endl;
// }

// void pushBackToList(int i, list<int> *lst, int size)
// {
//     start = clock();
//     for (; i < size; i++)
//     {
//         lst->push_back(values[i]);
//     }
//     end_clock = clock();
//     timeTaken = double(end_clock - start) / double(CLOCKS_PER_SEC);
//     cout << "insert time taken list (push_back): " << timeTaken << endl;
// }

bool valueCheck(AnchorList<int> *ell, vector<int> *vec, int size,
                map<int, double> *ell_timeTaken_map = nullptr, map<int, double> *vec_timeTaken_map = nullptr)
{

    bool result = true;
    // cout << "\n**anchor list :\nYours\tCorrect" << endl;
    double ellTimeTaken = 0;
    double vectorTimeTaken = 0;
    int ellValue, vectorValue;
    for (int i = 0; i < size; i++)
    {
        if ((*ell)[i] != (*vec)[i])
        {
            result = false;
            break;
        }
    }

    start_time = high_resolution_clock::now();
    for (int i = 0; i < size; i++)
    {
        ellValue = (*ell)[i];
    }
    end_time = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end_time - start_time);
    ellTimeTaken = duration.count() / 1e9;

    start_time = high_resolution_clock::now();
    for (int i = 0; i < size; i++)
    {
        vectorValue = (*vec)[i];
    }
    end_time = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(end_time - start_time);
    vectorTimeTaken = duration.count() / 1e9;

    // cout << "ell get time taken: " << ellTimeTaken << endl;
    // cout << "vector get time taken: " << vectorTimeTaken << endl;

    if (ell_timeTaken_map != nullptr)
    {
        ell_timeTaken_map->insert({size, ellTimeTaken});
    }

    if (vec_timeTaken_map != nullptr)
    {
        vec_timeTaken_map->insert({size, vectorTimeTaken});
    }

    return result;
}

// bool valueCheck(AnchorList<int> *ell, list<int> *lst, int size)
// {
//     bool result = true;
//     double ellTimeTaken = 0;
//     double listTimeTaken = 0;
//     int ellValue, listValue;

//     auto listIt = lst->begin(); // Iterator for the list

//     for (int i = 0; i < size; i++, ++listIt)
//     {
//         start = clock();
//         ellValue = (*ell)[i];
//         end_clock = clock();
//         ellTimeTaken += double(end_clock - start) / double(CLOCKS_PER_SEC);

//         start = clock();
//         listValue = *listIt;
//         end_clock = clock();
//         listTimeTaken += double(end_clock - start) / double(CLOCKS_PER_SEC);

//         if (ellValue != listValue)
//         {
//             result = false;
//             break;
//         }
//     }

//     cout << "ell get time taken: " << ellTimeTaken << endl;
//     cout << "list get time taken: " << listTimeTaken << endl;

//     return result;
// }

void removeFromAnchorList(AnchorList<int> *ell, int deleteIndexesSize,
                          map<int, double> *timeTaken_map = nullptr)
{
    start_time = high_resolution_clock::now();
    for (int i = 0; i < deleteIndexesSize; i++)
    {
        // cout << "index: " << indexes[i] << "\t value:"
        //      << values[i] << endl;

        ell->remove(deleteIndexes[i]);
        // ell->drawTree();
        // ell->iterateInOrder();
        // cout << "===============" << endl;
    }
    end_time = high_resolution_clock::now();
    malloc_trim(0);

    auto duration = duration_cast<nanoseconds>(end_time - start_time);
    timeTaken = duration.count() / 1e9;
    if (timeTaken_map != nullptr)
    {
        timeTaken_map->insert({deleteIndexesSize, timeTaken});
    }
    // cout << "remove time taken anchor list: " << timeTaken << endl;
}

void removeFromVector(vector<int> *vec, int deleteIndexesSize,
                      map<int, double> *timeTaken_map = nullptr)
{

    start_time = high_resolution_clock::now();
    int i;
    for (i = 0; i < deleteIndexesSize; i++)
    {
        vec->erase(vec->begin() + deleteIndexes[i]);
    }
    end_time = high_resolution_clock::now();
    vec->shrink_to_fit();

    malloc_trim(0);
    auto duration = duration_cast<nanoseconds>(end_time - start_time);
    timeTaken = duration.count() / 1e9;
    if (timeTaken_map != nullptr)
    {
        timeTaken_map->insert({deleteIndexesSize, timeTaken});
    }
    // cout << "remove time taken vector: " << timeTaken << endl;
}

// void popFrontFromList(int i, list<int> *lst, int deleteIndexesSize)
// {
//     start = clock();
//     for (; i < deleteIndexesSize; i++)
//     {
//         lst->pop_front();
//     }
//     end_clock = clock();
//     timeTaken = double(end_clock - start) / double(CLOCKS_PER_SEC);
//     cout << "remove time taken list (pop_front): " << timeTaken << endl;
// }

// void popBackFromList(int i, list<int> *lst, int deleteIndexesSize)
// {
//     start = clock();
//     for (; i < deleteIndexesSize; i++)
//     {
//         lst->pop_back();
//     }
//     end_clock = clock();
//     timeTaken = double(end_clock - start) / double(CLOCKS_PER_SEC);
//     cout << "remove time taken list (pop_back): " << timeTaken << endl;
// }

int main()
{

    // time taken results
    map<int, double> *pushBack_ellResult = new map<int, double>();
    map<int, double> *get_after_pushBack_ellResult = new map<int, double>();

    map<int, double> *popBackAfterPushBack_ellResult = new map<int, double>();
    map<int, double> *get_after_popBackAfterPushBack_ellResult = new map<int, double>();

    map<int, double> *popFrontAfterPushBack_ellResult = new map<int, double>();
    map<int, double> *get_after_popFrontAfterPushBack_ellResult = new map<int, double>();

    map<int, double> *removeRandomIndicesAfterPushBack_ellResult = new map<int, double>();
    map<int, double> *get_after_removeRandomIndicesAfterPushBack_ellResult = new map<int, double>();

    map<int, double> *pushBack_vecResult = new map<int, double>();
    map<int, double> *get_after_pushBack_vecResult = new map<int, double>();

    map<int, double> *popBackAfterPushBack_vecResult = new map<int, double>();
    map<int, double> *get_after_popBackAfterPushBack_vecResult = new map<int, double>();

    map<int, double> *popFrontAfterPushBack_vecResult = new map<int, double>();
    map<int, double> *get_after_popFrontAfterPushBack_vecResult = new map<int, double>();

    map<int, double> *removeRandomIndicesAfterPushBack_vecResult = new map<int, double>();
    map<int, double> *get_after_removeRandomIndicesAfterPushBack_vecResult = new map<int, double>();

    // push_front
    map<int, double> *pushFront_ellResult = new map<int, double>();
    map<int, double> *get_after_pushFront_ellResult = new map<int, double>();

    map<int, double> *popFrontAfterPushFront_ellResult = new map<int, double>();
    map<int, double> *get_after_popFrontAfterPushFront_ellResult = new map<int, double>();

    map<int, double> *popBackAfterPushFront_ellResult = new map<int, double>();
    map<int, double> *get_after_popBackAfterPushFront_ellResult = new map<int, double>();

    map<int, double> *pushFront_vecResult = new map<int, double>();
    map<int, double> *get_after_pushFront_vecResult = new map<int, double>();

    map<int, double> *popFrontAfterPushFront_vecResult = new map<int, double>();
    map<int, double> *get_after_popFrontAfterPushFront_vecResult = new map<int, double>();

    map<int, double> *popBackAfterPushFront_vecResult = new map<int, double>();
    map<int, double> *get_after_popBackAfterPushFront_vecResult = new map<int, double>();

    // insert random indices
    map<int, double> *insertRandomIndices_ellResult = new map<int, double>();
    map<int, double> *get_after_insertRandomIndices_ellResult = new map<int, double>();

    map<int, double> *removeRandomIndices_ellResult = new map<int, double>();
    map<int, double> *get_after_removeRandomIndices_ellResult = new map<int, double>();

    map<int, double> *insertRandomIndices_vecResult = new map<int, double>();
    map<int, double> *get_after_insertRandomIndices_vecResult = new map<int, double>();

    map<int, double> *removeRandomIndices_vecResult = new map<int, double>();
    map<int, double> *get_after_removeRandomIndices_vecResult = new map<int, double>();

    // push_front then push_back
    map<int, double> *pushFront_pushBack_ellResult = new map<int, double>();
    map<int, double> *get_after_pushFront_pushBack_ellResult = new map<int, double>();

    map<int, double> *popFront_popBack_ellResult = new map<int, double>();
    map<int, double> *get_after_popFront_popBack_ellResult = new map<int, double>();

    map<int, double> *pushFront_pushBack_vecResult = new map<int, double>();
    map<int, double> *get_after_pushFront_pushBack_vecResult = new map<int, double>();

    map<int, double> *popFront_popBack_vecResult = new map<int, double>();
    map<int, double> *get_after_popFront_popBack_vecResult = new map<int, double>();

    // push_back then push_front
    map<int, double> *pushBack_pushFront_ellResult = new map<int, double>();
    map<int, double> *get_after_pushBack_pushFront_ellResult = new map<int, double>();

    map<int, double> *popBack_popFront_ellResult = new map<int, double>();
    map<int, double> *get_after_popBack_popFront_ellResult = new map<int, double>();

    map<int, double> *pushBack_pushFront_vecResult = new map<int, double>();
    map<int, double> *get_after_pushBack_pushFront_vecResult = new map<int, double>();

    map<int, double> *popBack_popFront_vecResult = new map<int, double>();
    map<int, double> *get_after_popBack_popFront_vecResult = new map<int, double>();

    for (int step = 1; step <= 1000000; step *= 10)
    {
        for (int size = step; size < step * 10 && size <= 2000000; size += step)
        {
            cout << size << endl;
            values = new int[size];
            indexes = new int[size];
            deleteIndexes = new int[size / 2];
            for (int i = 0; i < size; i++)
            {
                values[i] = i + 1;
                indexes[i] = i;
            }

            bool result = true;
            AnchorList<int> *ell;
            vector<int> *vec;
            // list<int> *lst;

            // double ellUpdateTimeTaken = 0;
            // double ellRemoveTimeTaken = 0;

            // cout << "The test has started now with " << size << " insertions and " << size / 2 << " deletions:" << endl;
            for (int input = 0; input < 16; input++)
            {
                switch (input)
                {
                case 0: // insert tests
                    // cout << endl
                    //      << "1: test anchor list vs list push_back insert" << endl;
                    ell = new AnchorList<int>;
                    vec = new vector<int>();

                    insertToAnchorList(ell, size, pushBack_ellResult);
                    // std::cout << " RSS(KB): " << rss << "\n";
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    insertToVector(vec, size, pushBack_vecResult);
                    result = valueCheck(ell, vec, size, get_after_pushBack_ellResult, get_after_pushBack_vecResult);
                    break;
                case 1: // remove tests
                    // cout << endl
                    //      << "2: test anchor list vs list pop_back" << endl;
                    for (int i = 0; i < size / 2; i++)
                    {
                        deleteIndexes[i] = size - (i + 1);
                    }

                    removeFromAnchorList(ell, size / 2, popBackAfterPushBack_ellResult);

                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;

                    removeFromVector(vec, size / 2, popBackAfterPushBack_vecResult);

                    result = valueCheck(ell, vec, size / 2,
                                        get_after_popBackAfterPushBack_ellResult,
                                        get_after_popBackAfterPushBack_vecResult);

                    break;
                case 2: // insert tests
                    // cout << endl
                    //      << "3: test anchor list vs vector push_back insert" << endl;
                    ell = new AnchorList<int>;
                    vec = new vector<int>();
                    insertToAnchorList(ell, size);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    insertToVector(vec, size);
                    result = valueCheck(ell, vec, size);
                    break;
                case 3: // remove tests
                    // cout << endl
                    //      << "4: test anchor list vs vector pop_front" << endl;
                    for (int i = 0; i < size / 2; i++)
                    {
                        deleteIndexes[i] = 0;
                    }
                    removeFromAnchorList(ell, size / 2,
                                         popFrontAfterPushBack_ellResult);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    removeFromVector(vec, size / 2, popFrontAfterPushBack_vecResult);
                    result = valueCheck(ell, vec, size / 2,
                                        get_after_popFrontAfterPushBack_ellResult,
                                        get_after_popFrontAfterPushBack_vecResult);

                    break;
                case 4: // insert tests
                    // cout << endl
                    //      << "5: test anchor list vs list push_front" << endl;
                    ell = new AnchorList<int>;
                    vec = new vector<int>();

                    for (int i = 0; i < size; i++)
                    {
                        indexes[i] = 0;
                    }
                    insertToAnchorList(ell, size, pushFront_ellResult);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    insertToVector(vec, size, pushFront_vecResult);
                    result = valueCheck(ell, vec, size, get_after_pushFront_ellResult, get_after_pushFront_vecResult);
                    break;
                case 5: // remove tests
                    // cout << endl
                    //      << "6: test anchor list vs list pop_front" << endl;
                    removeFromAnchorList(ell, size / 2, popFrontAfterPushFront_ellResult);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    removeFromVector(vec, size / 2, popFrontAfterPushFront_vecResult);
                    result = valueCheck(ell, vec, size / 2,
                                        get_after_popFrontAfterPushFront_ellResult,
                                        get_after_popFrontAfterPushFront_vecResult);
                    break;
                case 6: // insert tests
                    // cout << endl
                    //      << "7: test anchor list vs list push_front" << endl;
                    ell = new AnchorList<int>;
                    vec = new vector<int>();

                    insertToAnchorList(ell, size);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    insertToVector(vec, size);
                    result = valueCheck(ell, vec, size);
                    break;
                case 7: // remove tests
                    // cout << endl
                    //      << "8: test anchor list vs list begin pop_back" << endl;
                    for (int i = 0; i < size / 2; i++)
                    {
                        deleteIndexes[i] = size - (i + 1);
                    }
                    removeFromAnchorList(ell, size / 2, popBackAfterPushFront_ellResult);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    removeFromVector(vec, size / 2, popBackAfterPushFront_vecResult);
                    result = valueCheck(ell, vec, size / 2,
                                        get_after_popBackAfterPushFront_ellResult,
                                        get_after_popBackAfterPushFront_vecResult);
                    break;
                case 8: // insert tests
                    // cout << endl
                    //      << "9: test anchor list vs vector push_back" << endl;
                    ell = new AnchorList<int>;
                    vec = new vector<int>();
                    for (int i = 0; i < size; i++)
                    {
                        indexes[i] = i;
                    }
                    insertToAnchorList(ell, size);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    insertToVector(vec, size);
                    result = valueCheck(ell, vec, size);
                    break;
                case 9: // remove tests
                    // cout << endl
                    //      << "10: test anchor list vs vector random indices remove" << endl;
                    for (int i = 0; i < size / 2; i++)
                    {
                        deleteIndexes[i] = rand() % (size - (i + 1));
                    }
                    removeFromAnchorList(ell, size / 2, removeRandomIndicesAfterPushBack_ellResult);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    removeFromVector(vec, size / 2, removeRandomIndicesAfterPushBack_vecResult);
                    result = valueCheck(ell, vec, size / 2,
                                        get_after_removeRandomIndicesAfterPushBack_ellResult,
                                        get_after_removeRandomIndicesAfterPushBack_vecResult);
                    break;
                case 10: // insert tests
                    // cout << endl
                    //      << "11: test anchor list vs vector random indices insert" << endl;
                    ell = new AnchorList<int>;
                    vec = new vector<int>();
                    for (int i = 0; i < size; i++)
                    {
                        indexes[i] = rand() % (i + 1);
                    }
                    insertToAnchorList(ell, size, insertRandomIndices_ellResult);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    insertToVector(vec, size, insertRandomIndices_vecResult);
                    result = valueCheck(ell, vec, size,
                                        get_after_insertRandomIndices_ellResult, get_after_insertRandomIndices_vecResult);
                    break;
                case 11: // remove tests
                    // cout << endl
                    //      << "12: test anchor list vs vector random indices remove" << endl;
                    for (int i = 0; i < size / 2; i++)
                    {
                        deleteIndexes[i] = rand() % (size - (i + 1));
                    }
                    removeFromAnchorList(ell, size / 2, removeRandomIndices_ellResult);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    removeFromVector(vec, size / 2, removeRandomIndices_vecResult);
                    result = valueCheck(ell, vec, size / 2,
                                        get_after_removeRandomIndices_ellResult, get_after_removeRandomIndices_vecResult);
                    break;

                case 12: // insert tests
                    // cout << endl
                    //      << "13: test anchor list vs list half push_front then push_back the other half" << endl;
                    ell = new AnchorList<int>;
                    vec = new vector<int>();

                    for (int i = 0; i < size / 2; i++)
                    {
                        indexes[i] = 0;
                        indexes[i + size / 2] = i + size / 2;
                    }

                    insertToAnchorList(ell, size, pushFront_pushBack_ellResult);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    insertToVector(vec, size, pushFront_pushBack_vecResult);

                    result = valueCheck(ell, vec, size,
                                        get_after_pushFront_pushBack_ellResult,
                                        get_after_pushFront_pushBack_vecResult);
                    break;
                case 13: // remove tests
                    // cout << endl
                    //      << "14: test anchor list vs list begin half pop_front then pop_back the other half" << endl;
                    for (int i = 0; i < size / 4; i++)
                    {
                        deleteIndexes[i] = 0;
                        deleteIndexes[i + size / 4] = size - (i + size / 4 + 1);
                    }
                    removeFromAnchorList(ell, size / 2, popFront_popBack_ellResult);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    removeFromVector(vec, size / 2, popFront_popBack_vecResult);

                    result = valueCheck(ell, vec, size / 2,
                                        get_after_popFront_popBack_ellResult,
                                        get_after_popFront_popBack_vecResult);
                    break;
                case 14: // insert tests
                    // cout << endl
                    //      << "15: test anchor list vs list half push_back then push_front the other half" << endl;
                    ell = new AnchorList<int>;
                    vec = new vector<int>();

                    for (int i = 0; i < size / 2; i++)
                    {
                        indexes[i] = i;
                        indexes[i + size / 2] = 0;
                    }

                    insertToAnchorList(ell, size, pushBack_pushFront_ellResult);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    insertToVector(vec, size, pushBack_pushFront_vecResult);

                    result = valueCheck(ell, vec, size,
                                        get_after_pushBack_pushFront_ellResult,
                                        get_after_pushBack_pushFront_vecResult);
                    break;
                case 15: // remove tests
                    // cout << endl
                    //      << "16: test anchor list vs list begin half pop_back then pop_front the other half" << endl;
                    for (int i = 0; i < size / 4; i++)
                    {
                        deleteIndexes[i] = size - (i + 1);
                        deleteIndexes[i + size / 4] = 0;
                    }
                    removeFromAnchorList(ell, size / 2, popBack_popFront_ellResult);
                    // cout << "a1LeftInsertion: " << ell->mIPPS23RBbt->a1LeftInsertion << endl;
                    // cout << "a1RightInsertion: " << ell->mIPPS23RBbt->a1RightInsertion << endl;
                    // cout << "a2LeftInsertion: " << ell->mIPPS23RBbt->a2LeftInsertion << endl;
                    // cout << "a2RightInsertion: " << ell->mIPPS23RBbt->a2RightInsertion << endl;
                    // cout << "b1LeftInsertion: " << ell->mIPPS23RBbt->b1LeftInsertion << endl;
                    // cout << "b1RightInsertion: " << ell->mIPPS23RBbt->b1RightInsertion << endl;
                    // cout << "b2LeftInsertion: " << ell->mIPPS23RBbt->b2LeftInsertion << endl;
                    // cout << "b2RightInsertion: " << ell->mIPPS23RBbt->b2RightInsertion << endl;
                    removeFromVector(vec, size / 2, popBack_popFront_vecResult);

                    result = valueCheck(ell, vec, size / 2,
                                        get_after_popBack_popFront_ellResult,
                                        get_after_popBack_popFront_vecResult);
                    break;
                }

                if (result)
                    continue;
                // cout << "** That was correct!" << endl;
                else
                {
                    cout << "** Doesn't match." << endl;
                    cout << "Your code did not pass the tests." << endl;
                    // int dummy;
                    // cin >> dummy;
                    return 0;
                }
            }

            // cout << "Your code passed all the tests." << endl;
            // int dummy;
            // cin >> dummy;

            delete ell;
            delete vec;
        }
    }

    auto saveMapToCSV = [](const map<int, double> *data, const string &filename)
    {
        const filesystem::path outputDirectory =
            filesystem::current_path() / "timeTakenResults" / "half_remove";
        error_code directoryError;
        filesystem::create_directories(outputDirectory, directoryError);
        if (directoryError)
        {
            cout << "Error: Could not create " << outputDirectory << ": "
                 << directoryError.message() << endl;
            return;
        }

        const filesystem::path fullPath = outputDirectory / filename;
        ofstream file(fullPath);
        if (file.is_open())
        {
            file << "Size,Time\n";
            file << fixed << setprecision(9); // Set precision to 9 decimal places
            for (const auto &pair : *data)
            {
                file << pair.first << "," << pair.second << "\n";
            }
            file.close();
            cout << "Saved " << fullPath << endl;
        }
        else
        {
            cout << "Error: Could not create " << fullPath << endl;
            cout << "Make sure the results directory exists" << endl;
        }
    };

    // Save all timing results
    saveMapToCSV(pushBack_ellResult, "pushBack_anchor_results.csv");
    saveMapToCSV(get_after_pushBack_ellResult, "get_after_pushBack_anchor_results.csv");

    saveMapToCSV(popBackAfterPushBack_ellResult, "popBackAfterPushBack_anchor_results.csv");
    saveMapToCSV(get_after_popBackAfterPushBack_ellResult, "get_after_popBackAfterPushBack_anchor_results.csv");

    saveMapToCSV(popFrontAfterPushBack_ellResult, "popFrontAfterPushBack_anchor_results.csv");
    saveMapToCSV(get_after_popFrontAfterPushBack_ellResult, "get_after_popFrontAfterPushBack_anchor_results.csv");

    saveMapToCSV(removeRandomIndicesAfterPushBack_ellResult, "removeRandomIndicesAfterPushBack_anchor_results.csv");
    saveMapToCSV(get_after_removeRandomIndicesAfterPushBack_ellResult, "get_after_removeRandomIndicesAfterPushBack_anchor_results.csv");

    saveMapToCSV(pushBack_vecResult, "pushBack_vec_results.csv");
    saveMapToCSV(get_after_pushBack_vecResult, "get_after_pushBack_vec_results.csv");

    saveMapToCSV(popBackAfterPushBack_vecResult, "popBackAfterPushBack_vec_results.csv");
    saveMapToCSV(get_after_popBackAfterPushBack_vecResult, "get_after_popBackAfterPushBack_vec_results.csv");

    saveMapToCSV(popFrontAfterPushBack_vecResult, "popFrontAfterPushBack_vec_results.csv");
    saveMapToCSV(get_after_popFrontAfterPushBack_vecResult, "get_after_popFrontAfterPushBack_vec_results.csv");

    saveMapToCSV(removeRandomIndicesAfterPushBack_vecResult, "removeRandomIndicesAfterPushBack_vec_results.csv");
    saveMapToCSV(get_after_removeRandomIndicesAfterPushBack_vecResult, "get_after_removeRandomIndicesAfterPushBack_vec_results.csv");

    saveMapToCSV(pushFront_ellResult, "pushFront_anchor_results.csv");
    saveMapToCSV(get_after_pushFront_ellResult, "get_after_pushFront_anchor_results.csv");

    saveMapToCSV(popFrontAfterPushFront_ellResult, "popFrontAfterPushFront_anchor_results.csv");
    saveMapToCSV(get_after_popFrontAfterPushFront_ellResult, "get_after_popFrontAfterPushFront_anchor_results.csv");

    saveMapToCSV(popBackAfterPushFront_ellResult, "popBackAfterPushFront_anchor_results.csv");
    saveMapToCSV(get_after_popBackAfterPushFront_ellResult, "get_after_popBackAfterPushFront_anchor_results.csv");

    saveMapToCSV(pushFront_vecResult, "pushFront_vec_results.csv");
    saveMapToCSV(get_after_pushFront_vecResult, "get_after_pushFront_vec_results.csv");

    saveMapToCSV(popFrontAfterPushFront_vecResult, "popFrontAfterPushFront_vec_results.csv");
    saveMapToCSV(get_after_popFrontAfterPushFront_vecResult, "get_after_popFrontAfterPushFront_vec_results.csv");

    saveMapToCSV(popBackAfterPushFront_vecResult, "popBackAfterPushFront_vec_results.csv");
    saveMapToCSV(get_after_popBackAfterPushFront_vecResult, "get_after_popBackAfterPushFront_vec_results.csv");

    saveMapToCSV(insertRandomIndices_ellResult, "insertRandomIndices_anchor_results.csv");
    saveMapToCSV(get_after_insertRandomIndices_ellResult, "get_after_insertRandomIndices_anchor_results.csv");

    saveMapToCSV(removeRandomIndices_ellResult, "removeRandomIndices_anchor_results.csv");
    saveMapToCSV(get_after_removeRandomIndices_ellResult, "get_after_removeRandomIndices_anchor_results.csv");

    saveMapToCSV(insertRandomIndices_vecResult, "insertRandomIndices_vec_results.csv");
    saveMapToCSV(get_after_insertRandomIndices_vecResult, "get_after_insertRandomIndices_vec_results.csv");

    saveMapToCSV(removeRandomIndices_vecResult, "removeRandomIndices_vec_results.csv");
    saveMapToCSV(get_after_removeRandomIndices_vecResult, "get_after_removeRandomIndices_vec_results.csv");

    saveMapToCSV(pushFront_pushBack_ellResult, "pushFront_pushBack_anchor_results.csv");
    saveMapToCSV(get_after_pushFront_pushBack_ellResult, "get_after_pushFront_pushBack_anchor_results.csv");

    saveMapToCSV(popFront_popBack_ellResult, "popFront_popBack_anchor_results.csv");
    saveMapToCSV(get_after_popFront_popBack_ellResult, "get_after_popFront_popBack_anchor_results.csv");

    saveMapToCSV(pushFront_pushBack_vecResult, "pushFront_pushBack_vec_results.csv");
    saveMapToCSV(get_after_pushFront_pushBack_vecResult, "get_after_pushFront_pushBack_vec_results.csv");

    saveMapToCSV(popFront_popBack_vecResult, "popFront_popBack_vec_results.csv");
    saveMapToCSV(get_after_popFront_popBack_vecResult, "get_after_popFront_popBack_vec_results.csv");

    saveMapToCSV(pushBack_pushFront_ellResult, "pushBack_pushFront_anchor_results.csv");
    saveMapToCSV(get_after_pushBack_pushFront_ellResult, "get_after_pushBack_pushFront_anchor_results.csv");

    saveMapToCSV(popBack_popFront_ellResult, "popBack_popFront_anchor_results.csv");
    saveMapToCSV(get_after_popBack_popFront_ellResult, "get_after_popBack_popFront_anchor_results.csv");

    saveMapToCSV(pushBack_pushFront_vecResult, "pushBack_pushFront_vec_results.csv");
    saveMapToCSV(get_after_pushBack_pushFront_vecResult, "get_after_pushBack_pushFront_vec_results.csv");

    saveMapToCSV(popBack_popFront_vecResult, "popBack_popFront_vec_results.csv");
    saveMapToCSV(get_after_popBack_popFront_vecResult, "get_after_popBack_popFront_vec_results.csv");
    // Save all memory results

    // Clean up memory maps

    // Clean up time taken maps
    delete pushBack_ellResult;
    delete get_after_pushBack_ellResult;

    delete popBackAfterPushBack_ellResult;
    delete get_after_popBackAfterPushBack_ellResult;

    delete popFrontAfterPushBack_ellResult;
    delete get_after_popFrontAfterPushBack_ellResult;

    delete removeRandomIndicesAfterPushBack_ellResult;
    delete get_after_removeRandomIndicesAfterPushBack_ellResult;

    delete pushBack_vecResult;
    delete get_after_pushBack_vecResult;

    delete popBackAfterPushBack_vecResult;
    delete get_after_popBackAfterPushBack_vecResult;

    delete popFrontAfterPushBack_vecResult;
    delete get_after_popFrontAfterPushBack_vecResult;

    delete removeRandomIndicesAfterPushBack_vecResult;
    delete get_after_removeRandomIndicesAfterPushBack_vecResult;

    delete pushFront_ellResult;
    delete get_after_pushFront_ellResult;

    delete popFrontAfterPushFront_ellResult;
    delete get_after_popFrontAfterPushFront_ellResult;

    delete popBackAfterPushFront_ellResult;
    delete get_after_popBackAfterPushFront_ellResult;

    delete pushFront_vecResult;
    delete get_after_pushFront_vecResult;

    delete popFrontAfterPushFront_vecResult;
    delete get_after_popFrontAfterPushFront_vecResult;

    delete popBackAfterPushFront_vecResult;
    delete get_after_popBackAfterPushFront_vecResult;

    delete insertRandomIndices_ellResult;
    delete get_after_insertRandomIndices_ellResult;

    delete removeRandomIndices_ellResult;
    delete get_after_removeRandomIndices_ellResult;

    delete insertRandomIndices_vecResult;
    delete get_after_insertRandomIndices_vecResult;

    delete removeRandomIndices_vecResult;
    delete get_after_removeRandomIndices_vecResult;

    delete pushFront_pushBack_ellResult;
    delete get_after_pushFront_pushBack_ellResult;

    delete popFront_popBack_ellResult;
    delete get_after_popFront_popBack_ellResult;

    delete pushFront_pushBack_vecResult;
    delete get_after_pushFront_pushBack_vecResult;

    delete popFront_popBack_vecResult;
    delete get_after_popFront_popBack_vecResult;

    delete pushBack_pushFront_ellResult;
    delete get_after_pushBack_pushFront_ellResult;

    delete popBack_popFront_ellResult;
    delete get_after_popBack_popFront_ellResult;

    delete pushBack_pushFront_vecResult;
    delete get_after_pushBack_pushFront_vecResult;

    delete popBack_popFront_vecResult;
    delete get_after_popBack_popFront_vecResult;
    // delete lst;
    return 0;
}
