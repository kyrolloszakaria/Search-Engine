#include "website.h"
map<int, website> websites; 
string screen = "";
// Building a search enginge.
template <typename T>
void print_AL(vector<vector<T>> v); // print Adjacency list for web webGraph for debugging purposes.
void classify_search(const string& searchQuery);
void build_web_graph(multimap<int, int>& TransposedwebGraph, vector<vector<int>> &TransposedVector);
void save_key_words();
void save_impressions_number();
void save_clicks();
void menu_spark();
void search_spark();
void OR_search(const vector<string> &searchKeys);
void AND_search(const vector<string>& searchKeys);
void Quote_search(const vector<string>& searchKeys);
void print_results(map<double, website> &resultWebsites, bool isFirst);
void PR_spark(vector<vector<int>>& TransposedVector); // initialize PR calculations;
void Open_web(int webNum, map<double, website>& resultWebsites);
void save_screen(string s);
void print_saved_screen();
void update_files();
// Search functions: assuming that OR and AND search are not case sensitive.
int main()
{
    // Program initialization.
    multimap<int, int> TransposedwebGraph; // map contains edges between numeric values of the websites.
    vector<vector<int>> TransposedVector;
    build_web_graph(TransposedwebGraph,TransposedVector);
    save_key_words();
    save_impressions_number();
    save_clicks();
    PR_spark(TransposedVector);
    menu_spark();
}
void build_web_graph(multimap<int, int>& TransposedwebGraph, vector<vector<int>> &TransposedVector)
{
    ifstream webGraphFile("webGraph.csv");
    if (!webGraphFile.is_open())
    {
        cout << "error opening webGraph file\n";
        exit(1);
    }
    string s;
    while (getline(webGraphFile, s))
    {
        int comma_idx = s.find(',');
        int w = s.find('w');
        int m = s.find('m');
        string part1 = s.substr(w, m - w+1);
        string part2 = s.substr(comma_idx+1, s.find_last_of('m') - comma_idx);

        int web1Num, web2Num;
        int s_webNum_idx = part1.find_first_of("0123456789");
        int e_webNum_idx = part1.find_last_of("0123456789");

        if (s_webNum_idx != e_webNum_idx) // two handle test-websites numbered with two digit number.
            web1Num = (part1[s_webNum_idx] - '0') * 10 + part1[e_webNum_idx] - '0';
        else
            web1Num = (part1[s_webNum_idx] - '0');
 
       s_webNum_idx = part2.find_first_of("0123456789");
       e_webNum_idx = part2.find_last_of("0123456789");
       if (s_webNum_idx != e_webNum_idx) 
           web2Num = (part2[s_webNum_idx] - '0') * 10 + part2[e_webNum_idx] - '0';
       else
           web2Num = (part2[s_webNum_idx] - '0');

       websites.emplace(web1Num, website(web1Num,part1)); // webNum is the web index
       websites.emplace(web2Num, website(web2Num, part2));
       websites[web1Num].updateOutLinks();
       TransposedwebGraph.emplace(web2Num, web1Num); // transposed as needed for PageRank calculations.
    }
    TransposedVector.resize(websites.size()+1);
    for (auto i : TransposedwebGraph) {
        TransposedVector[i.first].push_back(i.second);
    }
    webGraphFile.close();
    // for (auto i : webGraph)                             // for debugging purposes.
    //     cout << i.first << " " << i.second << "\n";
}
void save_key_words()
{
    ifstream keysFile("KeyWords.csv");
    if (!keysFile.is_open())
    {
        cout << "error opening KeyWords file\n";
        exit(1);
    }
    string s;
    int lineNum = 1;
    while (getline(keysFile, s))
    {
        stringstream ss(s);
        string key;
        int i = 0; // to ignore the first word.
                   // Assuming the keywords file is given in a sorted manner.
        while (getline(ss, key, ','))
        {
            if (i != 0)
            {
                //*(key.end() - 1) = '\0'; // to remove the quotation mark.
                websites[lineNum].addKeyWord(key);
            }
            i++;
        }
        lineNum++;
    }
    keysFile.close();
}
void save_impressions_number()
{
    ifstream impFile("impressions.csv");
    if (!impFile.is_open())
    {
        cout << "error opening impressions file\n";
        exit(1);
    }
    string s;
    int lineNum = 1;
    while (getline(impFile, s))
    {
        stringstream ss(s);
        string Imp;
        while (getline(ss, Imp, ','))
        {
        }
        int imp = stoi(Imp);
        websites[lineNum].setImpressions(imp); // assuming the impressions file is given in a sorted manner.
        lineNum++;
    }
    impFile.close();
}

void save_clicks(){
    ifstream clicksFile("clicks.csv");
    if (!clicksFile.is_open())
        return;
    else 
    {
        string s;
        int lineNum = 1;
        while(getline(clicksFile, s))
        {
            stringstream ss(s);
            string Clicks;
            while (getline(ss, Clicks, ','))
            {
            }
            int c = stoi(Clicks);
            websites[lineNum].setClicks(c);  // assuming the impressions file is given in a sorted manner.
            lineNum++;
        }
        clicksFile.close();
    }
}


void menu_spark()
{
    cout << "Welcome!\nWhat would you like to do?\n1. New search.\n2. Exit\n";
    cout << "Type in your choice: ";
    int x;
    cin >> x;
    switch (x)
    {
    case 1:
        search_spark();
        break;
    case 2:
        exit(1);
        break;
    default:
        cout << "Invalid input. Please try again.\n\n\n";
        menu_spark();
        break;
    }
}
template <typename T>
void print_AL(vector<vector<T>> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            cout << v[i][j] << " ";
        }
        cout << "\n";
    }
}
void classify_search(const string& searchQuery) //takes the query and classify the search type and take the important key words form the query and put it in a vector.
{
    vector<string> searchKeys;
    int n = searchQuery.size();
    int searchType = 2; // 0 for AND , 1 for " ... " , 2 for OR.
    // if searchQuery contains AND :
    int pos = searchQuery.find("AND");
    string temp;
    if (pos != std::string::npos)
    {
        searchType = 0;
        temp = searchQuery.substr(0, pos);
        temp += searchQuery.substr(pos + 3, searchQuery.size() - pos - 3);
    }
    // if searchQuery contains quotation marks :
    int lQ = searchQuery.find('"');
    if (lQ != std::string::npos) // lQ -> left Quotation, rQ -> right Quotation
    {
        searchType = 1;
        int rQ = searchQuery.find('"', lQ + 1);
        temp = searchQuery.substr(lQ + 1, rQ - lQ - 1); // takes startign position and length = rQ - lQ -1.
    }
    // if searchQuery contains OR or does not contains any :
    if (searchType == 2)
    {
        temp = searchQuery;
    }
    stringstream ss(temp);
    string word;
    while (getline(ss, word, ' '))
    {
        //if (word != "")
        //    cout << word << "\n";
        searchKeys.push_back(word);
    }
    switch (searchType)
    {
    case 0:
        // AND_search(searchKeys);
        break;

    case1:
        // Quote_search(searchKeys);
        break;

    case 2:
        OR_search(searchKeys);
        break;
    }
}
void OR_search(const vector<string>& searchKeys) 
{
    map<double,website> resultWebsites; // a map <double score, website result>
    for (auto &it : websites) 
    {
        string temp = it.second.getKeyWords();
        if (temp != "")
        {
            for (auto word : searchKeys)
            {
                if (temp.find(word) != std::string::npos) 
                {
                    it.second.updateImpressions(); 
                    resultWebsites.emplace(it.second.getScore(), it.second);
                    break;
                }

            }
        }
    } 
    print_results(resultWebsites,true);
}
void PR_spark(vector<vector<int>>& TransposedVector) {
    websites.begin()->second.calculatePR(websites, TransposedVector);
}
void search_spark() 
{
    cout << "\nWhat do you want to search for?\n";
    string searchQuery;
    cin.ignore(); // to clean the input buffer.
    getline(cin, searchQuery);
    classify_search(searchQuery);
}
void save_screen(string s) {
    screen += s;
}
void print_saved_screen() {
    cout << screen;
    screen = "";
}
void print_results(map<double, website> &resultWebsites, bool isFirst) // isFirst is used to avoid redoing operations again.
{
    cout << "\nSearch Results:\n";
    if (isFirst)
    {
        int i = 1;
        string s = "";
        for (auto it : resultWebsites) {
            cout << i << ". " << it.second.getName() << endl;
            s = s + to_string(i) + ". " + it.second.getName() + "\n";
            i++;
        }
        save_screen(s);
    }
    else
        print_saved_screen();
    if (resultWebsites.empty()) cout << "Currently, there is no website relavent to your query.\n";
    cout << "Would you like to?\n1. Choose a web page to open.\n2. New search.\n3. Exit.\nType in your choice: ";
    int x;
    cin >> x;
    while (x > 3 || x < 1 || (x==1 && resultWebsites.empty())) {
        cout << "Invalid input. Please try again.\nType in your choice: ";
        cin >> x;
    }
    switch (x)
    {
        case 1: 
        {
            int c;
            cout << "Please enter web page number: ";
            cin >> c;
            while (c > resultWebsites.size()) {
                cout << "invalid input! Please try again.\nPlease enter web page number: ";
                cin >> c;
            }
            auto webIt = resultWebsites.begin();
            for (int i = 0; i < c - 1; i++) webIt++;
            websites[webIt->second.getWebNum()].updateClicks();
            Open_web(webIt->second.getWebNum(), resultWebsites);
            break;
        }
        case 2:
        {
            search_spark();
            break;
        }
        case 3:
        {
            update_files();
            exit(1);
            break;
        }
    } 
      
    
}
void Open_web(int webNum, map<double, website>& resultWebsites) // webNum is the web index
{
    cout << "\nYou are now viewing " << websites[webNum].getName() << " Would you like to?\n";
    cout << "1. Back to search results.\n2. New search.\n3. Exit.\nType in your choice: ";
    int x; cin >> x;
    while (x > 3 || x < 1) {
        cout << "Invalid input. Please try again.\nType in your choice: ";
        cin >> x;
    }
    switch (x)
    {
    case 1:
        print_results(resultWebsites, false);   // Back to search results.
        break;
    case 2:
        search_spark();
        break;
    case 3:
        update_files();
        exit(1);
        break;
    }
        
}
void update_files() 
{
    // Update Clicks:
    ofstream file;
    file.open("newclicks.csv");
    for (auto w : websites) {
        file << w.second.getName() << "," << w.second.getClicks() << "\n";
    }
    file.close();
    remove("clicks.csv");
    rename("newclicks.csv", "clicks.csv");

    // Update impressions:
    file.open("newimpressions.csv");
    for (auto w : websites) {
        file << w.second.getName() << "," << w.second.getImpressions() << "\n";
    }
    file.close();
    remove("impressions.csv");
    rename("newimpressions.csv", "impressions.csv");
}



