#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <unordered_set>
#include <iomanip>

using namespace std;
/*
    Helper class that contains all utility function which my Apriori algorithm use
*/
class helper
{
public:
    static set<int> get_distinct_item_as_set(map<set<int>, int> first_fitemsets)
    {
        set<int> unique_itemset;
        for (auto const& x : first_fitemsets)
        {
            unique_itemset.insert(x.first.begin(), x.first.end());
        }
        return unique_itemset;
    }

    static bool check_if_not_exists_in_frequent_itemset(map<set<int>, int> frequent_itemset, set<int>& temp, int z)
    {
        //if the new set is not in first_fitemsets, then dont accept it on new_c. Otherwise, this set is already in first_fitemsets, so ignore it.
        if (frequent_itemset.find(temp) == frequent_itemset.end())
        {
            temp.insert(z);
            return true;
        }
        return false;
    }

    static int combination_count(int d, int k) {

        if (k == 1) {
            return d;
        }
        if (k == d) {
            return 1;
        }

        if (k > d) {
            return 0;
        }
        return combination_count(d - 1, k) + combination_count(d - 1, k - 1);
    }

    static void generate_subset(set<int>& trans_row, int length, int rule_size, set<int>::iterator position, set<int>& subsetcontainer, map<set<int>, int>& candidate)
    {
        if (rule_size == 0) {
            if (candidate.count(subsetcontainer)) {
                candidate[subsetcontainer] += 1;
            }
            return;
        }

        for (auto it = position; it != trans_row.end(); ++it)
        {
            subsetcontainer.insert(*it);
            generate_subset(trans_row, length, rule_size - 1, ++position, subsetcontainer, candidate);
            subsetcontainer.erase(*it);
        }
        return;
    }

    static string ConvertVectorToStringForPrinting(vector<int> vec) {
        string result = "";
        int i = 0;
        while (i < vec.size())
        {
            result += to_string(vec[i]);
            if (i != vec.size() - 1) {
                result += ", ";
            }
            ++i;
        }
        result += " ";
        return result;
    }
};


/*
    Global variable and function definition goes here.
*/
int CURRENT_CANDIDATE_SET_LENGTH = 1;   //this global var is essential to keep track of current candidate set length
int min_support;
double minimum_support;
double minimum_confidence;
int number_of_strong_rules = 0;
string association_rules_output_file;
string print_property;
vector<set<int>> transactions;
vector<string> transactions_str;
clock_t clock_start, clock_end, c1, c2;
// Global variable to store all the most frequent itemsets
vector<map<set<int>, int>> master_fitemsets;
map<set<int>, int> PruneL1(map<set<int>, int> candidate);
//int combination_count(int N, int K);
map<set<int>, int> StartAprioriAlgorithm(map<set<int>, int> first_fitemsets);
//void generate_subset(set<int>& sset, int size, int left_size, set<int>::iterator index, set<int>& v, map<set<int>, int>& candidate);
//map<set<int>, int> PruneCandidates(map<set<int>, int> c);
vector<tuple<vector<int>, vector<int>, long double, long double> > association_rule_container;
void GenerateAssociationRules(vector<int> items, vector<int> X, vector<int> Y, int index);
int CalculateSupport(vector<int> x1);
void GetSupport(int a, int b);


/*
    Purpose: Getting subsets from a set. Recursively calls to generate subsets.Need to check the time complexity.
*/
vector< vector<int> > getAllSubsets(vector<int> set, int size)
{
    vector< vector<int> > subset;
    vector<int> empty;
    subset.push_back(empty);

    for (int i = 0; i < set.size(); i++)
    {
        vector< vector<int> > subsetTemp = subset;  //making a copy of given 2-d vector.

        for (int j = 0; j < subsetTemp.size(); j++)
            subsetTemp[j].push_back(set[i]);   // adding set[i] element to each subset of subsetTemp. like adding {2}(in 2nd iteration  to {{},{1}} which gives {{2},{1,2}}.

        for (int j = 0; j < subsetTemp.size(); j++)
            subset.push_back(subsetTemp[j]);  //now adding modified subsetTemp to original subset (before{{},{1}} , after{{},{1},{2},{1,2}}) 
    }
    return subset;
}

/*
    Purpose: Another approach to get subsets from a set. Recursively calls to generate subsets.Need to check the time complexity.
*/
void subsetsUtil(set<int>& A, set<set<int> >& res, set<int>& subset, int index)
{
    res.insert(subset);
    for (int i = index; i < A.size(); i++)
    {

        // include the A[i] in subset.
        vector<int> v(A.begin(), A.end());
        subset.insert(v[i]);

        // move onto the next element.
        subsetsUtil(A, res, subset, i + 1);

        // exclude the A[i] from subset and triggers
        // backtracking.
        subset.erase(subset.size() - 1);
    }

    return;
}

/*
    Purpose: Print the subsets to check and validate the subset generation process
*/
set<set<int>> PrintAllSubsets(vector<int> arr, int n, int length, map<set<int>, int>& candidate) {
    int snum = 0;
    set<int> temp;
    set<set<int>> master;
    while (snum < pow(2, n))
    {
        for (int i = 0; i < n; ++i)
        {
            if ((snum & (1 << i)) != 0) {
                //cout << arr[i] << ' ';
                temp.insert(arr[i]);
            }
        }
        if (temp.size() == length)
        {
            master.insert(temp);
        }
        temp.clear();
        //cout << endl;
        ++snum;
    }
    return master;

}

int main(int argc, char** argv)
{
    //************** Testing the subset complexity **************/
    vector<int> arr = { 750, 753, 598, 601 }; // input array
    set<set<int> > res1;
    //res1 = PrintAllSubsets(arr, arr.size(), 3);


    //vector< vector<int> > result = getAllSubsets({ 1,2,3,4 });
    set<int> subset;
    set<set<int> > res;

    // keeps track of current element in vector A;
    int index = 0;
    set<int> set1 = { 10, 20, 30 };
    subsetsUtil(set1, res, subset, 0);

    //*************** Starting clock, reading from file system, and then run the pariori algorithm
    clock_start = clock();
    string transaction_input_file = "";
    //reading command line arguments with display options. print_property = argv[4] is optional
    if (argc >= 4)
    {
        transaction_input_file = argv[1];
        minimum_support = stod(argv[2]);
        minimum_confidence = stod(argv[3]);
        print_property = "";
    }
    if (argc > 4)
        print_property = argv[4];

    association_rules_output_file = "output.txt";

    //Reading input file for processing the transactions further. Read line by line store it into transactions variable which is vector<set<int>>
    ifstream file(transaction_input_file);
    string str;
    while (getline(file, str))
    {
        int pre = 0;
        set<int> arr;
        /*for (int i = 0; i < str.size(); i++) {
            if (str[i] == '\t') {
                int num = atoi(str.substr(pre, i).c_str());
                arr.push_back(num);
                pre = i + 1;
            }
        }
        int num = atoi(str.substr(pre, str.size()).c_str());*/

        //parsing the string using delimeter and storing back into vector 
        string s = str;
        string delimiter = " ";
        size_t pos = 0;
        string token, comma_separated = "";
        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            //std::cout << token << std::endl;
            s.erase(0, pos + delimiter.length());
            arr.insert(stoi(token));
            comma_separated = comma_separated + token + ",";
        }
        if (s != "")
        {
            arr.insert(stoi(s));
            comma_separated = comma_separated + s + ",";
        }
        //std::cout << s << std::endl;
        transactions.push_back(arr);
        transactions_str.push_back(comma_separated);
    }
    file.close();
    //input file processing done

    min_support = round(minimum_support * transactions.size());

    //preparing the candidate set with size = 1
    map<set<int>, int> candidate;
    CURRENT_CANDIDATE_SET_LENGTH = 1;    //1 means its 1 element candidate set
    set<int> transaction_row, temp;
    for (int i = 0; i < transactions.size(); i++)
    {
        transaction_row = transactions[i];
        for (auto const& x : transaction_row)
        {
            int ans = x;
            temp.insert(ans);
            if (candidate.find(temp) != candidate.end())    //if found then increase by 1 else insert as new entry
                candidate[temp] += 1;
            else
                candidate.insert({ temp, 1 });

            temp.clear();
        }
        transaction_row.clear();
    }

    //Prune L1 Candidate Set: Pruning 1 item candidate set based on minimum support
    map<set<int>, int> first_fitemsets = PruneL1(candidate);
    master_fitemsets.push_back(first_fitemsets);     //master_fitemsets will store all the 1,2,...N item frequent sets: its a vector<map<set<int>, int>> type. Array of map<set<int>, int>
    candidate.clear();

    //Now we will create 2,3,4,....N item candidate sets. We have created 1 item candidate set already.
    map<set<int>, int> tmp;
    c1 = clock();
    for (; first_fitemsets.size() != 0;)
    {
        tmp = StartAprioriAlgorithm(first_fitemsets);   //***** Calling AprioriAlgorithm ***** All Apriori task starts here
        master_fitemsets.push_back(tmp);
        first_fitemsets = master_fitemsets[master_fitemsets.size() - 1];
        tmp.clear();
    }
    master_fitemsets.pop_back();	//removing the last element from master_fitemsets.
    c2 = clock();
    //cout << "StartAprioriAlgorithm took = " << round((((double)c2 - (double)c1)) / CLOCKS_PER_SEC);


    clock_end = clock();
    double seconds = round((((double)clock_end - (double)clock_start)) / CLOCKS_PER_SEC);
    double minutes = round(seconds / 60);
    cout << "\nFrequent itemset files and association rules have been created successfully.\n";
    cout << "Total Execution Time (seconds) = " << seconds << ". (In minutes) = " << minutes << endl;


    if (print_property == "")    //if absent then print_property = "", so print only the number of frequent itemset and number of strong association rules
    {
        cout << "Print option is empty. So printing only the number of frequent itemset and number of strong association rules" << endl;
        int i = 0;
        while (i < master_fitemsets.size())
        {
            cout << "Number of frequent " << i + 1 << "_itemsets: " + to_string(master_fitemsets[i].size()) << endl;
            ++i;
        }

        //******************   Printing association rules starts here    *************************************************
        map<set<int>, int> frequent_itemset;
        set<int> tempset;
        for (int i = 1; i < master_fitemsets.size(); i++)
        {
            frequent_itemset = master_fitemsets[i];
            for (auto const& x : frequent_itemset)
            {
                tempset = x.first;
                vector<int> v(tempset.begin(), tempset.end());
                GenerateAssociationRules(v, {}, {}, 0);
            }
        }
        cout << "Number of association rules " << association_rule_container.size() << endl;     
    }

    if (print_property == "r" || print_property == "a")    //When "r", then all strong association rules are displayed.
    {
        //******************   Printing association rules starts here    *************************************************
        map<set<int>, int> frequent_itemset;
        set<int> tempset;
        for (int i = 1; i < master_fitemsets.size(); i++)
        {
            frequent_itemset = master_fitemsets[i];
            for (auto const& x : frequent_itemset)
            {
                tempset = x.first;
                vector<int> v(tempset.begin(), tempset.end());
                GenerateAssociationRules(v, {}, {}, 0);
            }
        }
        cout << "Number of association rules " << association_rule_container.size() << endl;

        ofstream f;
        f.open(association_rules_output_file);
        for (auto& i : association_rule_container) {
            
            /*  IMPORTANT: Uncomment this to print the output on console
            cout << helper::ConvertVectorToStringForPrinting(get<0>(i)) << "-> " << helper::ConvertVectorToStringForPrinting(get<1>(i)) << "(";
            cout << fixed;
            cout.precision(3);
            cout << get<2>(i) << ',';
            cout << fixed;
            cout.precision(3);
            cout << get<3>(i) << ")" << endl;
            */
            string str = helper::ConvertVectorToStringForPrinting(get<0>(i)) + "-> " + helper::ConvertVectorToStringForPrinting(get<1>(i)) + "(" + to_string(get<2>(i)).substr(0, to_string(get<2>(i)).find(".") + 4) + "," + to_string(get<3>(i)).substr(0, to_string(get<3>(i)).find(".") + 4) + ")";
            f << str << endl;
        }
        f.close();
    }
    if (print_property == "f" || print_property == "a")
    {
        //****** IMPOTANT: uncomment the cout lines here to print the result in console
        ofstream f;
        set<int> temp;
        int support;
        for (int i = 0; i < master_fitemsets.size(); i++)
        {
            string j = to_string(i + 1);
            f.open(j + "_itemsets.txt");
            cout << "Number of frequent " << i + 1 << "_itemsets: " + to_string(master_fitemsets[i].size()) << endl;
            for (auto const& x : master_fitemsets[i])
            {
                temp = x.first;
                support = x.second;
                int k = 0;
                string str = "";
                for (auto const& y : temp)
                {
                    if (k < temp.size() - 1)
                    {
                        str = str + to_string(y) + ", ";
                        /*cout << y << ", ";*/
                    }
                    else
                    {
                        str = str + to_string(y) + " ";
                        /*cout << y << " ";*/
                    }
                        
                    k++;
                }
                //cout << fixed;
                //cout.precision(3);
                str = str + "(" + to_string(support).substr(0, to_string(support).find(".") + 4) + ")";
                /*cout << "(" << support << ")" << endl;*/
                f << str << endl;
            }
            f.close();
            cout << "files have been generated successfully.";
            cout << endl;
        }

    }


};


/*
    Purpose: In order to write this function, I have taken help from github (https://github.com/bowbowbow/Apriori/blob/master/apriori.cpp) and stackoverflow. I am sharing the resource url here. This function efficiently prints the association rules. 
*/
void GenerateAssociationRules(vector<int> items, vector<int> X, vector<int> Y, int index)
{
    if (index == items.size())
    {
        if (X.size() == 0 || Y.size() == 0) return;

        set<int> set_items(items.begin(), items.end());
        set<int> set_X(X.begin(), X.end());

        long double XYsupport = master_fitemsets[set_items.size() - 1][set_items];
        long double Xsupport = master_fitemsets[set_X.size() - 1][set_X];

        if (Xsupport == 0) return;

        long double support = (long double)XYsupport;
        long double confidence = (long double)XYsupport / Xsupport * 100.0;
        if (confidence >= minimum_confidence * 100.0)
            association_rule_container.push_back({ X, Y, support, confidence });
        return;
    }

    X.push_back(items[index]);
    GenerateAssociationRules(items, X, Y, index + 1);
    X.pop_back();
    Y.push_back(items[index]);
    GenerateAssociationRules(items, X, Y, index + 1);
}

/*
 Purpose: get the support of a set
*/
int CalculateSupport(vector<int> x1)
{
    set<int> s(x1.begin(), x1.end());
    map<set<int>, int> frequent_itemset;
    set<int> itemset;
    for (int i = 0; i < master_fitemsets.size(); i++)
    {
        frequent_itemset = master_fitemsets[i];
        for (auto const& x : frequent_itemset)
        {
            itemset = x.first;
            if (itemset == s)
            {
                return x.second;
            }
        }
    }
    return 0;
}

/*
    Purpose: First 1 frequent itemset pruning code. 2,3,........N itemset pruning code is done in the StartAprioriAlgorithm function.
*/
map<set<int>, int> PruneL1(map<set<int>, int> candidate)
{
    map<set<int>, int> itemset_L1;
    //itemset_L1.fic = candidate.fic;
    set<int> set_part;
    int counter_part;
    for (auto const& x : candidate) //candidate.hashmap is the map<set<int>, int>
    {
        set_part = x.first;
        counter_part = x.second;

        if (counter_part >= min_support)
            itemset_L1.insert({ set_part, counter_part });

        set_part.clear();
    }
    return itemset_L1;
}

void GetSupport(int a, int b)
{
    //calculating the support
    int result = a + b;
    if (a > minimum_support)
        a = a - b;
    result = a;
}

/*
    Purpose: Main important function which starts the Apriori Algorithm over transaction dataset. It first generates the transaction dataset and then prunes it to keep on the sets that satisfy the criteria.
    I have taken help to write the subset function here from https://github.com/SaeedNajafi/Apriori-CPP/blob/master/main.cpp . The subset generation function is efficient. I have 
    tested my own subset function and put their code here in functions above. But this subset generation code saved a lot of time of mine.
*/
map<set<int>, int> StartAprioriAlgorithm(map<set<int>, int> frequent_itemset)
{
    //FIRST: Generate candidates with CURRENT_CANDIDATE_SET_LENGTH + 1
    map<set<int>, int> candidate_new, temp1;
    set<int> extracted_set, temp, probable_set, transaction_row, test;
    int i = 0, comb_count;
    bool insert_this_set; 
    string row;

    //candidate_new.fic = frequent_itemset.fic + 1;   //creating next element sets, 1 element extra than on frequent_itemset
    CURRENT_CANDIDATE_SET_LENGTH = CURRENT_CANDIDATE_SET_LENGTH + 1;
    set<int> distinct_items = helper::get_distinct_item_as_set(frequent_itemset);

    for (auto const& x : frequent_itemset)
    {
        extracted_set = x.first;   //getting the set<int> from map<set<int>, int> component
        GetSupport(1, x.second);
        for (auto const& y : distinct_items)
        {
            if (extracted_set.find(y) != extracted_set.end())   //if y is in extracted_set. Like we cant take 1 and {1,2}
                continue;

            probable_set = extracted_set;
            probable_set.insert(y);

            if (candidate_new.find(probable_set) == candidate_new.end())   //if probable_set is not in candidate_new
            {
                temp = probable_set;
                insert_this_set = true;

                for (auto const& z : probable_set)
                {
                    temp.erase(z);
                    if (frequent_itemset.count(temp) == 0)
                    {
                        insert_this_set = false;
                        temp.insert(z);
                        break;
                    }
                    temp.insert(z);
                }

                if (insert_this_set)
                {
                    candidate_new.insert({ probable_set, 0 });
                }
                temp.clear();
            }
            probable_set.clear();
        }
        extracted_set.clear();
    }

    distinct_items.clear();
    temp.clear();
    extracted_set.clear();
    temp.clear();


    //SECOND: Pruning phase starts here. We will keep only those sets in Candidate whose support is >= minimum support
    
    while (i < transactions.size())
    {
        transaction_row = transactions[i];
        i++;
        if (transaction_row.size() < CURRENT_CANDIDATE_SET_LENGTH) //if transaction data has less size than the number of items in the frequent itemset then do nothing
            continue;

        comb_count = helper::combination_count(transaction_row.size(), CURRENT_CANDIDATE_SET_LENGTH);

        /*vector<int> vec_transaction_row(transaction_row.begin(), transaction_row.end());
        set<set<int> > result_subsets;
        result_subsets = PrintAllSubsets(vec_transaction_row, vec_transaction_row.size(), candidate.fic, candidate);

        for (auto const& p : result_subsets)
        {
            if (candidate.hashmap.count(p) > 0) {
                candidate.hashmap[p] += 1;
            }
        }*/


        //subset(transaction_row, transaction_row.size(), candidate.fic, transaction_row.begin(), test, candidate);

        if (candidate_new.size() <= comb_count)
        {
            for (auto const& x : candidate_new)
            {
                temp = x.first;
                insert_this_set = true;
                for (auto const& y : temp)
                {
                    if (transaction_row.count(y) == 0) {
                        insert_this_set = false;
                        break;
                    }
                }

                if (insert_this_set)
                    candidate_new[temp] += 1;
            }

        }
        else
        {
            helper::generate_subset(transaction_row, transaction_row.size(), CURRENT_CANDIDATE_SET_LENGTH, transaction_row.begin(), test, candidate_new);
        }
        transaction_row.clear();
    }

    //Separating only those candidates whose support is higher than minimum support
    for (auto const& x : candidate_new) {
        int t = x.second;
        if (t >= min_support)
            temp1.insert({ x.first, t });
    }
    //temp1.fic = candidate_new.fic;
    candidate_new.clear();
    return temp1;
}