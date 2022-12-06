#include <iostream>
#include<vector>
#include <string>
#include<map>
using namespace std;

void set_support_count(map<string, int>& distinct, string arr[10],int number_of_seq);
bool isPart(string subSequence, string sequence);
bool isSubSequenceFound(string subSequence, string sequence);
void support_pruning(map<string, int>& distinct, int min_support, vector<string> &discarded);
void canadate_pruning(map<string, int>& distinct, vector<string> discarded);
void joinSetsTwo(map<string, int>& distinct);
void joinSets(map<string, int>& distinct);
bool isJoinAllowed(string s1, string s2);
string joinTwoStrings(string s1, string s2);

void main() {
	string arr[10];
    int min_support = 0;
    int number_of_seq;
    //------------------------------------------------take inputs from user-----------------------------------------
    cout << "Enter Number of Sequences: ";
    cin >> number_of_seq;
    cout << endl;
    cout << "Enter Minimum Support: ";
    cin >> min_support;
    cout << endl;
    for (int i = 0; i < number_of_seq; i++) {
        cout << "Enter Squence " << i + 1 << " ";
        cin >> arr[i];
    }
    /*------------------------------------------------1st item sets----------------------------------------------------*/
    map<string, int> distinct = map<string, int>();
    vector<string> discarded = vector<string>();
    for (int i = 0; i < number_of_seq; i++) { // calculate distinct letters
        for (char c : arr[i]) {
            if (c == '<' || c == '>' || c == '(' || c == ')') {
                continue;
            }
            else
            {
                string s;
                s += c;
                distinct[s] = 0;
            }

        }
    }
    set_support_count(distinct,arr,number_of_seq);
    support_pruning(distinct, min_support,discarded);
    cout << "Elements for first item sets: " << endl;
    for (auto s : distinct) {
        cout << s.first << " " << s.second << endl;
    }
    /*------------------------------------------------2nd item sets----------------------------------------------------*/
    joinSetsTwo(distinct);
    set_support_count(distinct, arr, number_of_seq);
    support_pruning(distinct, min_support,discarded);
    cout << "Elements for second item sets: " << endl;
    for (auto s : distinct) {
        cout << s.first << " " << s.second << endl;
    }
    /*------------------------------------------------3rd (and more) item sets----------------------------------------------------*/
    int i = 3;
    while (distinct.size() > 1) {
        joinSets(distinct);
        if (distinct.size() == 0)
            break;
        canadate_pruning(distinct, discarded);
        if (distinct.size() == 0)
            break;
        set_support_count(distinct, arr, number_of_seq);
        support_pruning(distinct, min_support, discarded);
        if (distinct.size() == 0)
            break;
        cout << "Elements for "<< i << "rd item sets : " << endl;
        for (auto s : distinct) {
            cout << s.first << " " << s.second << endl;
        }
        i++;
    }
}

void set_support_count(map<string, int>& distinct, string arr[10],int number_of_seq)
{
    for (auto seq : distinct) {
        for (int i = 0; i < number_of_seq; i++) {
            if (isSubSequenceFound(seq.first, arr[i])) {
                distinct[seq.first]++;
            }
        }
    }
}

bool isPart(string subSequence, string sequence) {
    int subSequence_len = subSequence.length();
    int sequence_len = sequence.length();
    int c = 0;
    int i = 0, j = 0;
    for (auto ch : subSequence) {
        for (auto ch1 : sequence) {
            if (ch == ch1)
                c++;
        }
    }
    if (c >= 2)
        return true;
    else
        return  false;
}

bool isSubSequenceFound(string subSequence, string sequence)
{
    int subSequence_len = subSequence.length();
    int sequence_len = sequence.length();
    bool flag = false;
    int i = 0, j = 0;
    while (i < subSequence_len && j < sequence_len)
    {
        // this condition is to check that no two or more chars of the subsequence exist in the sequance () part
        if (sequence[j] == '(' && subSequence[i] != '(' && subSequence.length() != 1) {
            string tmp;
            int index = j+1;
            while (sequence[index] != ')' && index < sequence_len) {
                tmp += sequence[index];
                index++;
            }
            flag = isPart(subSequence, tmp);
            if (flag)
                return false;
        }
        else if (subSequence[i] == sequence[j]) {
            i++;
        }
        j++;
    }
    return i == subSequence_len ;
}

void support_pruning(map<string, int>& distinct, int min_support, vector<string> &discarded)
{
    map<string, int> d = map<string, int>();
    for (auto item : distinct) {
        if (item.second >= min_support)
            d.insert({ item.first,item.second });
        else {
            discarded.push_back(item.first);
        }

    }
    distinct = d;
}

void canadate_pruning(map<string, int>& distinct, vector<string> discarded) {
    map<string, int> d = map<string, int>();
    for (auto item : distinct) {
        for (auto it : discarded) {
            if (isSubSequenceFound(it, item.first) == false)
                d.insert({ item.first,item.second });
        }
    }
    distinct = d;
}

void joinSetsTwo(map<string, int>& distinct)
{
    map<string, int> d = map<string, int>();
    // temporal
    for (auto seq : distinct) {
        for (auto it : distinct){
            string value = joinTwoStrings(seq.first, it.first);
            d.insert({ value, 0 });
        }
    }
    // non temporal 
    map<string, int>::iterator seq = distinct.begin();
    map<string, int>::iterator it = distinct.begin();
    for (auto seq : distinct) {
        bool flag = true;
        for (auto it : distinct){
            if (seq.first != it.first && flag == true)
                continue;
            else if (seq.first == it.first) {
                flag = false;
                continue;
            }
            string value = "(";
            value += joinTwoStrings(seq.first, it.first);
            value += ")";
            d.insert({ value, 0 });
        }
    }
    distinct = d;
}

void joinSets(map<string, int>& distinct)
{
    map<string, int> d = map<string, int>();

    for (auto seq : distinct) {
        for (auto it : distinct)
        {
            if (seq.first == it.first) {
                continue;
            }
            else {
                if (isJoinAllowed(seq.first, it.first)) {
                    string value = joinTwoStrings(seq.first, it.first);
                    d.insert({ value, 0 });
                }
            }

        }
    }

    distinct = d;
}

bool isJoinAllowed(string s1, string s2)
{
    int i = 1;
    int j = 0;
    bool flag = false;
    int s1_len = s1.length();
    int s2_len = s2.length() - 1;
    while (i < s1_len && j < s2_len)
    {
        if (s1[0] == '(') {
            i++;
            string tmp;
            tmp += s2[j];
            flag = isSubSequenceFound(tmp, s1);
        }
        else if (s2[0] == '(') {
            j++;
            string tmp;
            tmp +=s1[i];
            flag = isSubSequenceFound(tmp, s2);
        }
        else if (s1[i] == s2[j]) {
            i++;
            j++;
            flag = true;
        }
        else
        {
            flag = false;
            break;
        }
    }
    return flag;
}

string joinTwoStrings(string s1, string s2)
{
    string joinString;
    // the reseon for ( 
    //if (s1[0] == '(' && s2[0] == '(')
        //joinString;
    if (s1[0] == '(')
        joinString = s1 + s2[s2.length() - 1];
    else if (s2[0] == '(')
        joinString = s1[0] + s2;
    else
        joinString = s1 + s2[s2.length() - 1];
    return joinString;
}
