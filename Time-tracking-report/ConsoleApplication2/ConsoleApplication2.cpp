#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

struct employee
{
    string name;
    string email;
    string departmens;
    string position;
    string project;
    string task;
    string date;
    int hours;
};

vector<string> columns;
string input = "input.txt", output = "output.txt";


bool separate(char ch);
string getDate(string date);
void printWorkersDay(vector<employee> workers);
void printWorkersMonth(vector<employee> workers);
void calculateHoursOfMonth(vector<employee> workers);
void calculateHoursOfDays(vector<employee> workers);
void collumnsNumber(string columnsName, vector<employee> workers);

int main()
{
    ifstream fin;
    string name, line = "";
    
    fin.open(input);

   
    vector<employee> workers;

    int count = 0;
    while (getline(fin, line))
    {
        int q = -1;
        
        count++;
        int in = 0;

        employee employee;

        for (int i = 0; i < line.length()+1; i++)
        {

            if (separate(line[i]) || i == line.length())
            {
                string a;                
                try 
                {       
                    a = line.substr(in, i - in);
                    in = i + 1;
                }
                catch (int e)
                {                    
                    std::cerr << e;
                } 
                if (count == 1)
                {
                    columns.push_back(a);
                }
                else
                {
                    q++;
                    switch (q)
                    {
                    case 0: employee.name = a; break;
                    case 1: employee.email = a; break;
                    case 2: employee.departmens = a; break;
                    case 3: employee.position = a; break;
                    case 4: employee.project = a; break;
                    case 5: employee.task = a; break;
                    case 6: employee.date = a; break;
                    case 7: employee.hours = stoi(a); break;
                    }                
                }
            }
        }

        if (count != 1)
        {
            workers.push_back(employee);
        }
    } 
    fin.close();
   
    //Write how many hours employee work during month
    //calculateHoursOfMonth(workers);
    
    //Write how many hours employee work during day
    //calculateHoursOfDays(workers);
    
    //Write specific data about all workers
    //collumnsNumber("name;email;date",workers);

    cout << "Runtime = " << clock() / 1000.0 << "ms\n";
    return 0;
}

void collumnsNumber(string collumbsName, vector<employee> workers)
{
    int in = 0;
    vector<int> numbers;
    for (int i = 0; i < collumbsName.length()+1; i++)
    {

        if (separate(collumbsName[i]) || i == collumbsName.length())
        {
            string tmpValue;
            try
            {
                tmpValue = collumbsName.substr(in, i - in);
                in = i + 1;
            }
            catch (int e)
            {
                std::cerr << e;
            }

            for (int j = 0; j < columns.size(); j++)
            {

                if (columns[j] == tmpValue)
                {
                    numbers.push_back(j);
                        continue;
                }
            }

        }
    }
    ofstream fout(output);
    if (!fout)
        cout << "Error open file" << endl;
    for (int i = 0; i < numbers.size(); i++)
    {
        fout << columns[numbers[i]];
        if (i == numbers.size() - 1)
        {
            fout << endl;
            break;
        }
        fout << ";";
    }

    for (int i = 0; i < workers.size(); i++)
    {
        for (int j = 0; j < numbers.size(); j++)
        {   
            string tmpValue;
            switch (numbers[j])
            {
            case 0: tmpValue = workers[i].name; break;
            case 1: tmpValue = workers[i].email; break;
            case 2: tmpValue = workers[i].departmens; break;
            case 3: tmpValue = workers[i].position; break;
            case 4: tmpValue = workers[i].project; break;
            case 5: tmpValue = workers[i].task; break;
            case 6: tmpValue = workers[i].date; break;
            case 7: tmpValue = workers[i].hours; break;
            }

            fout << tmpValue;
            if (j == numbers.size() - 1)
                fout << endl;
            else
                fout << ";";
        }
    }
    fout.close();
}


bool separate(char ch)
{
    vector<char> chars = { ',',';','/','\n'};

    for (int i = 0; i < chars.size(); i++)    
        if (ch == chars[i])
            return true;    
    return false;
}


void calculateHoursOfMonth(vector<employee> workers)
{
    vector<employee> resultWorkers;

    for (int i = 0; i < workers.size(); i++)
    {

        employee tmpWorker = workers[i];
        tmpWorker.date = tmpWorker.date.substr(0, 7);

        employee tmp = workers[workers.size() - 1];
        workers[i] = tmp;
        workers.pop_back();
        i--;

        for (int j = 0; j < workers.size(); j++)
        {
            if (workers[j].name == tmpWorker.name && workers[j].date.substr(0, 7) == tmpWorker.date)
            {
                tmpWorker.hours += workers[j].hours;
                tmp = workers[workers.size() - 1];
                workers[j] = tmp;
                workers.pop_back();
                j--;
            }
        }
        resultWorkers.push_back(tmpWorker);
    }
    printWorkersMonth(resultWorkers);
}


void calculateHoursOfDays(vector<employee> workers)
{
    vector<employee> resultWorkers;

    for (int i = 0; i < workers.size(); i++)
    {

        employee tmpWorker = workers[i];

        employee tmp = workers[workers.size() - 1];
        workers[i] = tmp;
        workers.pop_back();
        i--;

        for (int j = 1; j < workers.size(); j++)
        {
            if (workers[j].name == tmpWorker.name && workers[j].date == tmpWorker.date)
            {
                tmpWorker.hours += workers[j].hours;
                 tmp = workers[workers.size() - 1];
                workers[j] = tmp;
                workers.pop_back();
                j--;
            }
        }

        if (tmpWorker.hours > 24)
            cout << "Error: " << tmpWorker.name << " can`t work " << tmpWorker.hours << " hours on " << tmpWorker.date << endl;
        
        resultWorkers.push_back(tmpWorker);
    }

    printWorkersDay(resultWorkers);

}

void printWorkersMonth(vector<employee> workers)
{
    ofstream fout(output);
    if (!fout)
        cout << "Error open file" << endl;
    fout << "Name;Month;Total time\n";
    for (int i = 0; i < workers.size(); i++)
        fout << workers[i].name << ";" << getDate(workers[i].date) << ";" << workers[i].hours << "\n";
    fout.close();
}

void printWorkersDay(vector<employee> workers)
{
    ofstream fout(output);
    if (!fout)
        cout << "Error open file" << endl;
    fout << "Name;Day;Total time\n";
    for (int i = 0; i < workers.size(); i++)
        fout << workers[i].name << ";" << workers[i].date << ";" << workers[i].hours << "\n";
    fout.close();
}

string getDate(string date)
{
    string month[12] = {
        "January","February", 
        "March", "April", 
        "May", "June", 
        "July", "August", 
        "September", "October", 
        "November", "December"};
    return month[stoi(date.substr(5,2)) - 1] + " " + date.substr(0, 4);
}

