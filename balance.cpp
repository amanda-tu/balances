#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

int month = 0;
int year = 0;

void extractDate(string date)
{
    month = stoi(date.substr(0,2));
    year = stoi(date.substr(6,10));
}

int main()
{
    //read in string
    string fileName;
    cout << "Please enter filename: ";
    cin >> fileName;

    fstream file(fileName, ios::in);
    vector<string> row;
    string line,word;

    string ID;
    string date;
    string transaction;
    unordered_map<string,unordered_map<int,unordered_map<int,vector<int> > > > balances;

    if(file.is_open())
    {
        getline(file, line); 
        while(!file.eof())
        {
            getline(file, line);
            stringstream str(line);
           
            getline(str,word, ',');
            if(word.length() == 0)
                continue;
            ID = word; 

            getline(str,word, ',');
            date = word;
            extractDate(date);

            getline(str,word, ',');
            transaction = word;
            int amount = stoi(transaction);

            if(balances[ID][year].find(month) == balances[ID][year].end())
            {
                balances[ID][year][month].push_back(min(amount, INT_MAX));
                balances[ID][year][month].push_back(max(amount, 0));
                balances[ID][year][month].push_back(amount);
            }
            else
            {
                balances[ID][year][month][0] = min(amount + balances[ID][year][month][2], balances[ID][year][month][0]);
                balances[ID][year][month][1] = max(amount + balances[ID][year][month][2], balances[ID][year][month][1]);
                balances[ID][year][month][2] += amount;
            }
        }
    }

   unordered_map<string,unordered_map<int,unordered_map<int,vector<int> > > >::iterator iter;
   unordered_map<int,unordered_map<int,vector<int> > >::iterator yr;
   unordered_map<int,vector<int> >::iterator mth;

   for(iter = balances.begin(); iter != balances.end();iter++)
   {
        for(yr = iter->second.begin(); yr != iter->second.end();yr++)
        {
            for(mth = yr->second.begin(); mth != yr->second.end();mth++)
            {
                cout << iter->first << ", " << yr->first << "," << mth->first << ", " ;
                for(int i = 0; i < 3;i++)
                    cout << balances[iter->first][yr->first][mth->first][i] << ",";
                cout <<endl;
            }
        }
   }
}