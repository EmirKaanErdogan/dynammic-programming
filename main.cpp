

/*

    BLG336E Analysis of Algorithms
    Assigment -3- 
    <Emir Kaan Erdoğan>
    <150200706>  

*/

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
#include <stack>
#include <unordered_map>
#include <map>
#include <time.h>
using namespace std; 

// store the day in a year ;
map<string, string> month_map = {
    {"01", "January"},
    {"02", "February"},
    {"03", "March"},
    {"04", "April"},
    {"05", "May"},
    {"06", "June"},
    {"07", "July"},
    {"08", "August"},
    {"09", "September"},
    {"10", "October"},
    {"11", "November"},
    {"12", "December"}
};
int temp[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
vector<int> days(temp, temp + sizeof(temp) / sizeof(temp[0]));



int calculate_day_index(int day, int month) {
    int current_day = 0;
    for (int i = 0; i < month - 1; i++) {
        current_day += days[i];
    }
    current_day += day;
    return current_day;
}



struct session{
    string start_time; 
    string end_time ;
}; 
struct availability{
    string start_date; 
    string end_date; 
};
struct Hall {
    // a hall has capacity and sessions 
    string name ;
    int capacity;
    vector <session*> sessions;  
};
typedef struct {
    // salon name,halls
    string name ;
    vector <Hall*> hall;
    vector <availability*> dates; 
    int total_value ;
    /*
    void sort_availability() {
       sort(dates.begin(), dates.end(), [](const availability* a1, const availability* a2) {
            return a1->end_date < a2->end_date;
        });
    }
    */
} Cinema;



bool compare(session s1, session s2)
{
    // util function to sort the sessions wrt the end time ;
    return (s1.end_time < s2.end_time);
}
bool is_compatible(const pair<Hall*, session*>& p1, const pair<Hall*, session*>& p2) {
    return (p1.second->end_time<=p2.second->start_time);
}
bool compare_session(const pair<Hall*, session*>& p1, const pair<Hall*, session*>& p2) {
    // util function to sort the sessions wrt the end time ;
    return (p1.second->end_time < p2.second->end_time);
}
void fetch_availability (vector<Cinema*>& cinema, string idx_file ){
    ifstream file (idx_file); 
    string line ; 
    int cinema_index=0 ;
    int date_index=0  ; 
    bool first_row = true; 
    if(!(file.is_open())){
        cout<<"The file cannot be opened ! "<<endl ; 
        return; 
    }
    while(getline(file,line)){
        // the txt file has columns: place_name, starting_date,ending_date :
        if(first_row){
            first_row=false; 
            continue;
        }
        istringstream iss(line);
        string place_name  ;
        string start_date; 
        string end_date ; 

        if (iss >> place_name >> start_date>>end_date) {
            if(!(cinema[cinema_index]->name==place_name)){
                cinema_index+=1; 
                date_index=0 ; 
            }
            cinema[cinema_index]->dates.push_back(new availability());
            cinema[cinema_index]->dates[date_index]->start_date=start_date; 
            cinema[cinema_index]->dates[date_index]->end_date=end_date;
            date_index+=1; 
        }
    }

}
vector <Cinema*> fetch_cinema(string idx_file ){
    
    // create vector of pointers to Cinema struct, which has name and capacity attributes; 
    // in adition it has also time vector that has the start and end time of the halls 
    // the vector attribute is to fetch in another function ;
    vector<Cinema*> cinema ; 
    ifstream file (idx_file); 
    string line ;
    string prev_place_name="";
    string prev_hall_name=""; 
    int cinema_counter=-1; 
    if(!(file.is_open())){
        cout<<"The file cannot be opened ! "<<endl ; 
        return cinema; 
    }
    while(getline(file,line)){
        istringstream iss(line);
            string place_name; 
            string hall_name;
            int capacity;

            if (iss >> place_name>>hall_name >> capacity) {
                if(!(prev_place_name==place_name)){
                    if(prev_hall_name==hall_name){
                        cout<<"The content is not appopriate !" <<endl; 
                    } 
                    cinema.push_back(new Cinema );
                    cinema_counter+=1; 
                    cinema[cinema_counter]->name=place_name;
                    cinema[cinema_counter]->hall.push_back(new Hall() ) ;
                    cinema[cinema_counter]->hall.back()->name= hall_name; 
                    cinema[cinema_counter]->hall.back()->capacity= capacity ;   
                }
                else {
                    cinema[cinema_counter]->hall.push_back(new Hall()); 
                    cinema[cinema_counter]->hall.back()->name= hall_name; 
                    cinema[cinema_counter]->hall.back()->capacity= capacity;
                }
                prev_place_name= place_name; 
                prev_hall_name= hall_name ; 
            }
            else {
                cout<<" "<<endl ; 
            } 
    }
    file.close(); 
    return cinema; 
}
void add_time_interval(vector<Cinema*>& cinema, string idx_file){
    ifstream file(idx_file); 
    string line ;
    int cinema_index= 0 ; 
    int hall_index= 0; 
    int session_counter=0 ; 
    cout<<endl ; 
    bool first_line = true ; 
    if(!(file.is_open())){
        cout<<"The file cannot be opened ! "<<endl ; 
        return;
    }    
    
    while(getline(file,line)){
        istringstream iss(line);
        string place_name; 
        string hall_name;
        string start_time ;
        string end_time ;         
        if(first_line){
            first_line=false; 
            continue;
        }
        if (iss >> place_name>>hall_name >> start_time>>end_time) {
            if(!(place_name==cinema[cinema_index]->name)){
                cinema_index+=1;
                hall_index=0 ;
                session_counter=0 ;
            }
            else {
                if(!(cinema[cinema_index]->hall[hall_index]->name==hall_name)){
                    hall_index+=1; 
                    session_counter=0; 
                }
            }
            cinema[cinema_index]->hall[hall_index]->sessions.push_back(new session);
            cinema[cinema_index]->hall[hall_index]->sessions[session_counter]->start_time=start_time ;
            cinema[cinema_index]->hall[hall_index]->sessions[session_counter]->end_time=end_time ;
            session_counter+=1; 
        }
        else{
            cout<<" "<<endl ;
        }
    }
}
class Place {
    private:
        string name;
        vector<Cinema> cinemas;

    public:
        Place() {}
        // Constructor with parameters
        Place(const string& place_name, const string& start_date, const string& end_date, const vector<Cinema>& cinemas)
            : name(place_name),cinemas(cinemas) {}

    // Getters
        string get_name() const {
            return this->name;
        }
        vector<Cinema> getCinemas() const {
            return this-> cinemas;
        }
    // Setters
        void set_name(const string& place_name) {
            this->name = place_name;
        }

        void set_cinemas(const vector<Cinema>& cinemas) {
            this->cinemas = cinemas;
        }
        /*
        void print_object()const {
            cout<<"Name: "<<this->name<<" , Start Date: "<<this->start_date<<" , End Date: , "<<this->end_date; 
            for (int i=0;i<this->cinemas.size();i++){
                for(int j=0;j<cinemas[i].capacity.size();j++){
                    for(int z=0 ; z<cinemas[i].size();z++)
                    cout<<"Cinema: "<<cinemas[i].name<<"Hall Name:"<<cinemas[i].capacity[] <<" , Capacity: " <<cinemas[i].capacity[z].capacity
                    <<" ,start time"<<cinemas[i].time_vector[j].start_time
                    <<" , end time: "<<cinemas[i].time_vector[j].end_time<<" "; 
                }
                cout<<endl ; 
        }
            cout<<endl ; 
        }
        */
};



class Asset { 
    private:
        string name ; 
        double price ; 
        double value; 
    public:
        Asset(); // default constructor 
        Asset(string,double,double); // constructor with parameters; 
        Asset(Asset*);// copy constructor 
        // Getters 
        string get_name()const {
            return this->name ; 
        }
        double get_price()const {
            return this->price; 
        }
        double get_value()const {
            return this->value; 
        }
        // Setters
        void set_name(string name){
            this->name= name ;
        }
        void set_price(double price){
            this->price= price; 
        }
        void set_value(double value){
            this->value=value ; 
        }
        void print_object()const {
            cout<<"Name: "<<this->name<<" , Price: "<<this->price<<" , Value: "<<this->value<<endl ; 
        }
}; 

Asset::Asset(){
    cout<<"Welcome to Default Constructor ! "<<endl ; 
}

Asset::Asset(string name,double price,double value){
    this->name=name;
    this->price=price;
    this->value=value; 
}
Asset::Asset(Asset* copy_asset){
    this->name=copy_asset->name;
    this->price=copy_asset->price;
    this->value=copy_asset->value; 
}

vector<Asset*> fetch_asset_array(string idx_file){
    // create vector of pointers to asset object, which has price and value attributes; 
    vector<Asset*> assets; 
    ifstream file (idx_file); 
    string line ;
    if(!(file.is_open())){
        cout<<"The file cannot be opened ! "<<endl ; 
        return assets; 
    }
    while(getline(file,line)){
        istringstream iss(line);
            string name;
            double price;
            double value;
            if (iss >> name >> price >> value) {
                assets.push_back(new Asset(name, price, value));
            }
            else {
                cout<<""<<endl ; 
            } 
    }
    file.close(); 
    return assets; 
}

// Consider jobs in ascending order of finish time.
vector <pair<Hall*,session*> > session_araranger(Cinema idx_cinema){
    // first store the sessions in different halls into a vector ;
    // the vector store
    vector <pair<Hall*,session*> > all_sessions ; 
    for(int i=0;i<idx_cinema.hall.size();i++){
        for(int j=0;j<idx_cinema.hall[i]->sessions.size();j++){
            all_sessions.push_back(make_pair(idx_cinema.hall[i],idx_cinema.hall[i]->sessions[j])); 
        }
    }

    sort(all_sessions.begin(), all_sessions.end(), compare_session);
    return all_sessions; 
} 

vector <int> compatibility_array(const vector <pair<Hall*,session*> > all_sessions){

    vector <int> result(all_sessions.size(),0) ; 
    for(int i=0;i<all_sessions.size();i++){
        for(int j= i-1 ; j>=0; j--){
            if(is_compatible(all_sessions[j],all_sessions[i])){
                result[i]=j+1; 
                break ;
            }
        }
    }
    return result ;
}

vector <int> opt (const vector <pair<Hall* ,session*> >&all_sessions,const vector <int> &comp_array){

    vector <int> result(all_sessions.size()+1,0); 
    for(int i=1;i<=all_sessions.size();i++){
        result[i]= max(result[i-1],all_sessions[i-1].first->capacity+result[comp_array[i-1]]);
    }
    return result; 
}

vector<pair<Hall*, session*> > find_opt(const vector<pair<Hall*, session* > >& all_sessions, const vector<int>& comp_array, const vector<int>& opt) {
    vector<pair<Hall*, session*> > solution;
    int i = opt.size() - 1;

    while (i > 0) {
        if (i == 1 || opt[i] != opt[i - 1]) {
            solution.push_back(all_sessions[i - 1]);
            i = comp_array[i - 1];
        } else {
            i--;
        }
    }

    reverse(solution.begin(), solution.end());
    return solution;
}
int find_value(const vector <int>obt_vector){
    int max=0 ; 
    for(int i=0;i<obt_vector.size();i++){
        if(max<obt_vector[i]){
            max=obt_vector[i];
        }
    }
    return max; 
}
void write_best_for_each_place(const vector<pair<Hall*, session*> > cinema_vector, const Cinema idx_cinema,int value){
    ofstream outfile("best_for_each_place.txt", ios::app); 
    if(!outfile.is_open()){
        return ;
    }
    outfile <<idx_cinema.name<<" --> "<<value<<endl ; 
    for(int k=0 ;k<cinema_vector.size();k++){
        outfile <<idx_cinema.name<<"\t\t"<<cinema_vector[k].first->name<< "\t\t"
        << cinema_vector[k].second->start_time<<"\t\t"<<cinema_vector[k].second->end_time<<endl ;
    }
    outfile<<endl ; 
    outfile.close();
}

void best_for_each_place(const vector<vector<pair<Hall*, session*> > > cinema_vector,const vector<Cinema*>&my_cinema,vector <int>& value_vector){
    // size of all the cinemas  ;
    int size= cinema_vector.size() ; 

    for (int i=0;i<size;i++){
        
        vector <int> p= compatibility_array(cinema_vector[i]); 
        vector<int> opt_vector= opt(cinema_vector[i],p);
        int max= find_value(opt_vector);
        vector<pair<Hall*, session*> > solution = find_opt(cinema_vector[i], p, opt_vector);
        value_vector.push_back(max);
        write_best_for_each_place(solution,*my_cinema[i],max);
    }
}


bool compare_date_util(const std::pair<Cinema*, availability*>& v1, const std::pair<Cinema*, availability*>& v2) {
    // utiltiy function to compare dates 
    // it converts the date of type string into integer and take into comparison 
    
    int day1 = stoi(v1.second->end_date.substr(0, 2));
    int day2 = stoi(v2.second->end_date.substr(0, 2));
    int month1 = stoi(v1.second->end_date.substr(3, 2));
    int month2 = stoi(v2.second->end_date.substr(3, 2));
    if(month1<month2){
        return true; 
    }
    else if(month1==month2){
        return day1<day2; 
    }
    else {
        return false; 
    }
}

vector <pair <Cinema*,availability*> > arrange_availability(const vector <Cinema*>&cinema){
    // function to combine all dates of each cinema into a single vector ; 
    vector <pair <Cinema*,availability*> > combined_cinemas ; 
    int index=0; 
    for(;index<cinema.size();index++){
        for(int i=0;i<cinema[index]->dates.size();i++){
            combined_cinemas.push_back(make_pair(cinema[index],cinema[index]->dates[i]));
        }
    }

    sort(combined_cinemas.begin(),combined_cinemas.end(),compare_date_util);
    return combined_cinemas;
}
bool is_compatible_tour(const pair<Cinema*, availability*>& v1, const pair<Cinema*, availability*>& v2) {
    // Overload the func for the second task to determine the best tour 
    int end_day1 = stoi(v1.second->end_date.substr(0, 2));
    int start_day2 = stoi(v2.second->start_date.substr(0, 2));
    int end_month1 = stoi(v1.second->end_date.substr(3, 2));
    int start_month2 = stoi(v2.second->start_date.substr(3, 2));
    if(end_month1<start_month2){
        return true; 
    }
    else if(end_month1==start_month2){
        return (end_day1<=start_day2); 
    }
    else {
        return false; 
    }
}
vector <int> compatibility_array(const vector <pair <Cinema*,availability* > > &combined_cinemas ){
    // overload the function with the new parameters; 
    vector <int> result (combined_cinemas.size(), 0);
    for(int i=0;i<combined_cinemas.size();i++){
        for(int j=i-1;j>=0;j--){
            if(is_compatible_tour(combined_cinemas[j],combined_cinemas[i])){
                result[i]=j+1; 
                break; 
            }
        }
    }
    return result ;
}

int calculate_day_value_tour(const vector <pair <Cinema*,availability* > > &combined_cinemas,int index){

    int end_day = stoi(combined_cinemas[index].second->end_date.substr(0, 2));
    int start_day = stoi(combined_cinemas[index].second->start_date.substr(0, 2));
    int month_start = stoi(combined_cinemas[index].second->start_date.substr(3, 2));
    int month_end = stoi(combined_cinemas[index].second->end_date.substr(3, 2));
    int index1 = calculate_day_index(start_day, month_start);
    int index2 = calculate_day_index(end_day, month_end);
    int result = (index2 - index1) * combined_cinemas[index].first->total_value;
    return result;
}

vector <int> opt_tour (const vector <pair <Cinema*,availability* > > combined_cinemas,const vector<int>& comp_array)
{
    // since the paramaeter and the return type changes, overload the function 
    vector <int> result (combined_cinemas.size()+1,0);
    for (int i=1;i<=combined_cinemas.size();i++){
        result[i]= max(result[i-1],calculate_day_value_tour(combined_cinemas,i-1)+result[comp_array[i-1]]);
    }
    return result ; 
}

vector<pair<Cinema*, availability*> > find_opt_tour(const vector<pair<Cinema*, availability* > >& combined_cinemas,
const vector<int>& comp_array, const vector<int>& opt) {
    vector <pair <Cinema* , availability*> > solution ; 
    int i= opt.size()-1 ; 
    while(i>0){
        if(i==1 || opt[i]!= opt[i-1]){
            solution.push_back(combined_cinemas[i-1]);
            i=comp_array[i-1];
        }
        else {
            i-- ; 
        }
    }
    reverse(solution.begin(),solution.end());
    return solution;
}

string find_month_name(string idx_date){
    string month = (idx_date.substr(3, 2));
    map<string,string>::iterator map_it=month_map.find(month) ;
    if(map_it != month_map.end() )
    {
        return map_it->second; 
    }
    return "The Month does not exist ! "; 
}
void best_tour(const vector<pair<Cinema*, availability* > >& result,int total_value){
    
    ofstream outfile ("best_tour.txt",ios::app);
    if(!outfile.is_open()){
        return ; 
    }
    outfile<<"Total Revenue −−> "<<total_value<<endl; 

    for(int i=0;i<result.size();i++){
        /*
        outfile<<result[i].first->name<<"\t\t"
        <<find_month_name(result[i].second->start_date)<<" "<<result[i].second->start_date.substr(0,2) 
        <<"\t\t"
        <<find_month_name(result[i].second->end_date)<<""<<result[i].second->end_date.substr(0,2)<<endl ; 
        */
        outfile<<result[i].first->name<<"\t\t"
        <<result[i].second->start_date<<"\t\t"<<result[i].second->end_date<<endl ; 
    }       

}

// Functions for the Knapsack ;
void upgrade_list(const vector<Asset*> result,double total_value){
    ofstream outfile("upgrade_list.txt",ios::app); 
    if(!outfile.is_open()){
        return ; 
    }
    outfile<<"Total Value --> "<<total_value<<endl ;
    for(int i=0;i<result.size();i++){
        outfile<<result[i]->get_name()<<endl ;
    }
}
vector <Asset*>find_solution(const vector<Asset*>&assets, double total_value) {
    vector<vector<double> > dp(assets.size()+ 1, vector<double>(total_value + 1, 0.0));
    // Build the knapsack table
    for (int i = 1; i <= assets.size(); i++) {
        for (int w = 0; w <= total_value; w++) {
            if (assets[i - 1]->get_price() <= w) {
                dp[i][w] = max(dp[i - 1][w], assets[i - 1]->get_value() + dp[i - 1][w - assets[i - 1]->get_price()]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }
    // Backtracking
    vector<Asset*> result;
    int i = assets.size();
    int w = total_value;
    while (i > 0 && w > 0) {
        if (dp[i][w] != dp[i - 1][w]) {
            result.push_back(assets[i - 1]);
            w = (w-assets[i - 1]->get_price());
        }
        i--;
    }
    
    return result;
}

int main(int argc,char* argv[]){
    string case_no= argv[1]; 
    cout<<"Case No: "<<case_no<<endl ;
    string file_1= "inputs/"+case_no+"/"+"assets.txt"; 
    cout<<"file 1 :"<<file_1<<endl ; 
    string file_2="inputs/"+ case_no+"/"+"availability_intervals.txt"; 
    string file_3="inputs/"+ case_no+"/"+"capacity.txt"; 
    string file_4="inputs/"+ case_no+"/"+"daily_schedule.txt"; 

    vector <Asset*> asset= fetch_asset_array(file_1); 
    vector <Cinema*> my_cinema= fetch_cinema(file_3); 
    add_time_interval(my_cinema,file_4); 
    fetch_availability(my_cinema,file_2);
    vector<vector<pair<Hall*, session*> > > cinema_vector;
    for (int i=0 ;i<my_cinema.size();i++){
            vector <pair<Hall*,session*> > all_sessions=session_araranger(*my_cinema[i]);
            cinema_vector.push_back(all_sessions);
    } 
    vector <int> value_vector ; 
    best_for_each_place(cinema_vector,my_cinema,value_vector); 
    for(int i=0;i<my_cinema.size();i++){
        my_cinema[i]->total_value=value_vector[i]; 
    }
    // IMPLEMENT SECOND TASK, DETERMINE THE BEST TOUR 
    vector <pair <Cinema*,availability*> > a_dates =arrange_availability(my_cinema);
    vector <int> vd= compatibility_array(a_dates);
    vector <int> vector_dates=opt_tour(a_dates,vd);
    vector<pair<Cinema*, availability*> > solution = find_opt_tour(a_dates,vd, vector_dates);
    int value=0  ;
    for(int i=0;i<solution.size();i++){
        value += calculate_day_value_tour(solution,i);
    }
    
    best_tour(solution,value);

    // THIRD TASK KNAPSACK; 
    // the value represents the positive impact of the asset, maximize the value 

    vector<Asset*> asset_result = find_solution(asset,value);
    double asset_value=0 ; 
    for(int i=0;i<asset_result.size();i++){
        asset_value+=asset_result[i]->get_value();
    }
    upgrade_list(asset_result,asset_value);
    return EXIT_SUCCESS ; 
}



