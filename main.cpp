// ######################################################################################################################
// ############################################### Importing modules ######################################### Section 0
//                                                                                                           ############
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <ctime>
using namespace std;

// ######################################################################################################################
// ################################################ Defining classes ######################################### Section 1
//                                                                                                           ############
class person{
private:
  int infection; // 1 means the user is infected, 0 means they are not
  int immune; // 1 means the person is immune, 0 means they're not
  int symptoms; // 1 means that the individual is showing symptoms, 0 means they're not
  int caution; // 0 means that a person is ignorant, 1 means that a person is aware and 2 means they are cautious
  int dayssick = 0; // tracks the number of days that the person has been sick for, starts at 0 for obvious reasons
  int age; // influences an individuals chance of dying from the disease
  int sickness_capacity; // everyone has a number of days they're going to be sick for before recovering
  
public:
  // initialisation functions
    person(int infstart, int deadstart, int imstart, int symptomsstart, int cautionstart){infection = infstart; immune = imstart; symptoms = symptomsstart; caution = cautionstart; dayssick = 0;}
    person(int age_input, int sickness_capacity_input){ infection = 1  ; immune = 0 ; symptoms = 0; caution = 0; dayssick = 0; age = age_input; sickness_capacity = sickness_capacity_input;} // creates a fresh infected person of custom age
  int checkhealth();
  int checkimmune();
  int checksymptoms();
  int checkcaution();
  int checkage();
  int checkdayssick();
  int checksicknesscapacity();
  void changehealth(int newval);
  void changeimmune(int newval);
  void changesymptoms(int newval);
  void changecaution(int newval);
  int newday();
  void adddaysick();

};

class region{
friend class person;
private:
    int lockdown_threshold; // acceptable number of deaths before country locks down
    vector <person> people; // where all the people are stored
    int total_population;
    int average_age;
    int total_dead = 0;
    int total_immune = 0;
    int lockdown_state = 0;
    string regionname;
    
    // record the people going in or out via air travel
    int air_travel_in;
    int air_travel_out;
    
    
public:
    void construct_region_normally(string name, int region_population, int threshold_for_lockdown, int average_age, int initial_infected);
    void print_region_info();
    vector <person> getpeople(){return(people);}
    void setsize(int i);
    int getsize();
    int get_average_age();
    int get_lockdown_threshold();
    int get_lockdown_state();
    unsigned long getinfected();
    vector <person> return_people();
    void add_infected(int a_a);
    void addperson(person citizen);
    void run_meetings_once(int lockdown_stat);
    void killpeople(vector <int> agebrackets_input, vector <double> lethality_input);
    void updatesick(int days_symptomless_input);
    string getlabel();
    int calculate_abroad_number(double percent_chance);
    void peoplein(unsigned long inpeople);
    void runregionday(int symptomless_limit_input, vector <double> lethality_daily_values, double seriouschance , int infected_or_dead, int average_abroad_rate);
};

// ######################################################################################################################
// ################################################## Initialisations ######################################## Section 2
//                                                                                                           ############

// ############################# Simulation variables ############################# Section 2.1

// Define the variables that are presets (COVID - 19)
int dayswithoutsymptoms = 14;
vector <int> agebrackets = {9,19,49,59,69,79,200};
// lethality statistics found at https://www.cebm.net/global-covid-19-case-fatality-rates/ // (Accessed March 25th 2020)
vector <double> lethalitytotal = {0.000,0.0018,0.0032,0.0130,0.0360,0.0800,0.1480};

// Country populations
int UK_pop = (66.44 * 1000000); // 66.4 million
int Italy_pop = (60.48 * 1000000); //60.48 million
int China_pop = (1.386 * 1000000000); // 1.386 Billion

vector <person> limbo = {};
// this is where people will go when they are travelling between countries
// It starts off empty because no one travels on the first day

// Initialise the variables that the user can input if they want
int user_quarantine_threshold; // the user can choose how many deaths they will accept before locking down a country
// int deaths_or_symptoms;
// double proportion_who_take_it_seriously;


// ######################################################################################################################
// ##################################################    Functions    ######################################## Section 3
//                                                                                                           ############
// ############################# Checking functions ############################# Section 3.1
int person::checkhealth()
{
    return(infection);
}

int person::checksymptoms()
{
    return(symptoms);
}


int person::checkcaution()
{
    return(caution);
}

int person::checkimmune()
{
    return(immune);
}

int person::checkage(){
    return(age);
}

int person::checkdayssick(){
    return(dayssick);
}

int person::checksicknesscapacity(){
    return sickness_capacity;
}

int region::getsize(){
    return(total_population);
}


int region::get_average_age(){
    return(average_age);
}

int region::get_lockdown_threshold(){
    return(lockdown_threshold);
}
int region::get_lockdown_state(){
    return(lockdown_state);
}

unsigned long region::getinfected(){
    return(people.size());
}

string region::getlabel(){
    return(regionname);
}

vector <person> region::return_people(){
    return(people);
}

// ############################# Updating  functions ############################# Section 3.2
// define functions that update parameters based on input

void person::changehealth(int newval)
{
    infection = newval;
}

void person::changesymptoms(int newval)
{
    symptoms = newval;
}

void person::changecaution(int newval)
{
    caution = newval;
}


void person:: changeimmune(int newval)
{
    immune = newval;
}

void region:: setsize(int i)
{
    total_population = i;
}

void person::adddaysick(){
    dayssick++;
}

// ############################## Utility functions ############################# Section 3.3

// Function that returns random double between 0 and 1
double rnd()
{
    return static_cast<double>(rand()) / (static_cast<long>(RAND_MAX)+1);
}

double random_gauss(double upperbound, double lowerbound){
    
    // two random numbers between -1 and 1
    double xrand1 = rnd();
    xrand1 = (xrand1 * 2) - 1;
    double xrand2 = rnd();
    xrand1 = (xrand2 * 2) - 1;
    
    // standard deviation centred about zero with sigma 1
    double basic1 = (1 / (sqrt( 2 * M_PI ))) * pow(M_E,  ( -(xrand1)*(xrand1) )/ (2)) ;
    double basic2 = (1 / (sqrt( 2 * M_PI ))) * pow(M_E,  ( -(xrand2)*(xrand2) )/ (2)) ;
    basic1 = basic1 * lowerbound;
    basic2 = basic2 * upperbound;
    double basic = (basic1 + basic2 / 2);
    
    
    // add on the mean from the input
    if (basic < 0)
    {
        basic = 0;
    }
    
    return(basic);
    
}

vector <double> calculate_daily_probs(vector <double> total_probs, int disease_lifetime){
    vector <double> output_vector;
    for(int i = 0; i < total_probs.size();i++){
        
        // The total chance of dying in the disease lifetime is P (The number the news shows)
        // say the prob of living on any particular day (that one shows symptoms)...
        // ...is A and dying is D
        // In a timespan of N days, the chance of living is A^N...
        // ...therefore the chance of dying is 1 - (A^N) (at least one death result)
        // P = 1 - (A^N)
        // but A = 1 - D, and D is what we are solving for
        // P = 1 - ((1-D)^N)
        // ((1-D)^N) = 1-P
        // 1-D = (1-P)^(1/N)
        // D = 1 - (1-P)^(1/N)
        // probabiltiy of dying on any particular day if one have symptoms (based on age)
        double value_prob = (1 - pow((1 - total_probs[i]),(1.0/disease_lifetime)));
        output_vector.push_back(value_prob);
    } // end of loop
    
    return(output_vector);
}


int random_days_sick(){
    // returns a random number between 3 and 14 for the number of days someone can be sick for
    double roll = rnd();
    // there are 11 days in the interval and a 3 day offset
    roll = (roll * 11) + 3;
    int output = round(roll);
    return(output);
    
}

int random_age(int average_age_input){
    // returns a random age from a gaussian distribution, for construction of regions
    int output;
    
    // Use box muller method to pull a random number (Z) from a gaussian centred on zero with standard deviation 1
    double U1 = rnd();
    double U2 = rnd();
    
    double Z;
    
    Z = sqrt( -2 * log(U1)) * cos( 2 * M_PI * U2 );
    Z = (Z + 1) * average_age_input;
    output = round(Z);
    if (output <= 0){
        output = 0;
    }
    
    return(output);
    
} // end of random_age

int double_random_age(int average_age_input){
    // I'm finding that random_age produces 0 a little too often...
    // So if the first run is 0 it runs again
    
    int age_output = random_age(average_age_input);
    
    if (age_output == 0){
        age_output = random_age(average_age_input);
    }
    return(age_output);
    
} // end of double_random_age

// ###############################  Region functions ############################# Section 3.4
// For these functions the approach is to only take up memory by addding people when they are sick
// The 'blank' space between the last sick person and the population cap is treated as healthy


void region::add_infected(int a_age){
    person citizen(double_random_age(a_age),random_days_sick());
    people.push_back(citizen);
}

void region::print_region_info(){

    // create a visual buffer so that the information isn't lost in the console
    cout << "----------------------------- Info for " << regionname << " -----------------------------" << endl;
    
    // Output the information
    cout << "Total population: " << total_population << endl;
    cout << "Infected people: " << people.size() << endl;
    cout << "Healthy people: " << (total_population - people.size()) << endl;
    cout << "Total dead: " << total_dead << endl;
    cout << "Total immune: " << total_immune << endl;
    
    // another visual bookend to end the data
    cout << "-----------------------------";
    for (int i = 0; i < (regionname.length()+11); i++)
    {
        cout << "-";
    }
    cout << "-----------------------------" << endl << endl;
    
} // end of print_region_info
    

// ########################### Simulation functions ############################# Section 3.4

// Region construction function

void region::construct_region_normally(string name, int population_input, int lockdown_threshold_input, int average_age_input, int initial_infected_input){
    
    // label the region
    regionname = name;
    
    // Define the size of the region
    setsize(population_input);
    
    // Set the lockdown threshold
    lockdown_threshold = lockdown_threshold_input;
    
    // Add the infected people
    int made_people = 0;
    while (made_people != initial_infected_input){
        int drawn_age = double_random_age(20);
        person citizen(drawn_age,random_days_sick());
        people.push_back(citizen);
        made_people++;
    }
    // cout << regionname << " constructed of " << population_input<< " people with " << initial_infected_input << " infected people." << endl;
    
} // end of construct_region_normally

void region::addperson(person citizen){
    unsigned long count = people.size();
    people.push_back(citizen);
    
    if (people.size() != (count + 1)){
        cout << "transfer error into second region" << endl;
    }
}

void region::updatesick(int symptomless_limit_input){
    // This function takes all te sick people in the region and adds a day onto the days that they have been sick
    for (int i = 0; i < people.size(); i++){ // for every infected person
        people[i].adddaysick();
        
        // If the person has met the required number of days without symptoms and isn't showing symptoms...
        if (people[i].checkdayssick() == symptomless_limit_input and people[i].checksymptoms() == 0){
            // give them symptoms
            people[i].changesymptoms(1);
            
            // because they have symptoms, they are aware of the risk of infection now
            people[i].changecaution(1);
            
        }
        
        // If the person has been showing symptoms for the same number of days as their capacity to get sick...
        if (people[i].checkdayssick() == people[i].checksicknesscapacity() + symptomless_limit_input){
            // remove the person from the region
            people.erase (people.begin()+i);
            
            // decrease the total population size by 1, because that person cannot be infected again
            total_population--;
            
            // document that one more person is immune
            total_immune++;
        }
    } // end of for every infected person
} // end of updatesick

void region::killpeople(vector <int> agebrackets_input, vector <double> lethality_input){
    // searches the regions infected and kills people based on the lethality of the virus, should be ran once per day
    
    for (int i = 0; i < people.size(); i++){ // for each infected person
        int persons_symptoms = people[i].checksymptoms();
        
        switch(persons_symptoms){ // Assuming people cannot die without symptoms
            case 0:
                // no symptoms, live another day
                break;
            case 1:
                double roll = rnd(); // determines the persons chance of living
                // if the person is showing symptoms the chance of dying depends on their age
                int their_age = people[i].checkage();
                
                for (int j = 0; j < agebrackets_input.size(); j++) // for all the age brackets
                {
                    if (their_age <= agebrackets_input[j]){ // if they are in the bracket j...
                        if (roll < lethality_input[j]){
                            // ...roll a 0-1 dice, if it is less than the chance for the j bracket...
                            // ...the person dies, remove them from vector of people and record death
                            people.erase (people.begin()+i);
                            total_dead++;
                            // because it starts at the youngest bracket and works up, there's no repeating of brackets
                            break;
                        }
                    }
                    
                }// end of for all age brackets
        } // end of switch statement
    } // end of for each infected person
}// end of killpeople



void region::run_meetings_once(int lockdown_stat){
    
    // this function makes a certain proportion of infected people meet one other person at random...
    // depending on the level of lockdown
    
    // since healthy people can meet each other as much as they want and not change a thing, only the meetings with infected people count
    
    
    unsigned long number_of_meetings = 0;
    
    switch(lockdown_stat){
        case 0:
            // in the case there is no lockdown at all every infected person meets someone at random
            number_of_meetings = people.size();
            // I am aware that this is a conversion issue, however it is impossible for people.size() to be negative, so the unsigned aspect of 'unsigned long'...
            // ... is not impactful in the conversion.
            
            break;
        case 1:
            // in the case of lockdown only people without symptoms leave the house, once a week to meet someone unique (shopping etc.).
            
            // find out how many people have don't have symptoms
            int finepeople = 0;
            for(int i = 0; i < people.size(); i++){
                if(people[i].checksymptoms() == 0){
                    finepeople++;
                }
            }
            
            number_of_meetings = round(finepeople / 7.0);
    }
    
    // fetch average age of people in the region for constructing new people if needed
    int a_a = get_average_age();


    // each meeting is defined by who stands on the 'left' and 'right' the person on the left is predetermined...
    // and the person on the right is random
    // ... for instance, each infected person will meet somebody but who they meet is random.
    
    
    for (int i = 0; i < number_of_meetings; i++){ // for every infected person that has been chosen
        
        // record their caution level
        int persons_caution = people[i].checkcaution();

            // pick a random participant on the right
            int other_person = floor(rnd()*total_population);
            if (other_person <= (people.size())){ // if the right person is infected
                // nothing happens
                break;
            }
            else{ // the person on the right is not infected, chance for infection occurs
                double roll = rnd();
                
                
                switch(persons_caution){
                    case 0:
                        // person is not cautious, 90% chance they will infect the other party
                        if (roll < 0.90){
                            add_infected(a_a);
                        }
                        break;
                    case 1:
                    // person is aware, 50% chance they will infect the other party
                    if (roll < 0.50){
                            add_infected(a_a);
                        }
                        break;
                    case 2:
                        // person is cautious, 5% chance they will infect the other party
                        if (roll < 0.05){
                            add_infected(a_a);
                        }
                        break;
                } // end of switch statement
            } // end of else (chance of infection occuring)
    } // end of for every infected person
    
} // end of run_day_region

int region::calculate_abroad_number(double percent_chance){
    // depending on the chance of travel, this will find how many (infected) people are travelling out of a region
    
    int count = 0;
    
    for (int i = 0; i < people.size(); i++){ // for each infected person
        double travelroll = rnd(); // roll a 0-1 die to see if they travel
        if (travelroll < percent_chance){
            count++;
        }
    }// end of for each infected person
    
    return(count);
}

void region::peoplein(unsigned long inpeople){
    // inpeople is how many people are drawn from limbo
    for (int i = 0; i < inpeople; i++){
        people.push_back(limbo[i]);
        limbo.erase (limbo.begin());
        total_population++;
    }
    //cout << inpeople << " added to " << getlabel() << endl;
}

void region::runregionday(int symptomless_limit_input, vector <double> lethality_daily_values, double seriouschance , int infected_or_dead, int average_abroad_rate){
    // runs region for one day
    
    // average abroad rate is how many days between a person traveling abroad (on average)
    double probtravel = (1.0 / average_abroad_rate);
    
    // How many people will be leaving the country
    int peopleout = 0;
    // people are added into the region from limbo elsewhere, so that all countried acn be updated simulataneously with passegners
    
    // concerning the lockdown threshold, set a ticker that tracks
    int threshold_tally = 0;
            
    switch(infected_or_dead){
        case(0):
            // if 0 then the input is number of infected, not the number of dead
            for(int i = 0; i < people.size(); i++){ // for every infected person
                // if they show symptoms
                if(people[i].checksymptoms() == 1){
                    threshold_tally++; // add onto the tally
                }
            }
            break;
        case 1:
            // if 1 then the threshold is based on the number of dead
            threshold_tally = total_dead;
            break;
                    
    }
    
            
    if (get_lockdown_threshold() > threshold_tally){
        // if the lockdown threshold is larger than the number of people that are infected and showing symptoms, everything continues as normal
        // people meet
        run_meetings_once(0);
        
        // find out how many people are going to limbo
        peopleout += calculate_abroad_number(probtravel);
        
    }
    else{
        // if the lockdown threshold is met then lockdown is enforced
        // very minimal air travel will be possible, 0.5% of infected people without symptoms will still travel (overseas trade etc)
        probtravel = probtravel * 0.005;
        // If this is the first day the lockdown is enforced then record it
        if (lockdown_state == 0){ // if lockdown not in place
            int cautionticker = 0;
                    
            lockdown_state = 1; // enforce lockdown
            cout << endl <<  "! ! ! ! ! ! ! ! ! ! ! ! ! ! " << regionname << " lockdown ! ! ! ! ! ! ! ! ! ! ! ! ! ! " << endl << endl;
                    
            // when lockdown starts, if a person is aware they have a chance of taking the virus more seriously
            for (int i = 0; i < people.size(); i++){ // for every infected person
                // if they are already aware
                        
                if(people[i].checkcaution() == 1){
                    double roll = rnd(); // roll a die
                    // if it is less than the seriouschance then the person takes the virus seriously
                    if(roll < seriouschance){
                        people[i].changecaution(2);
                        cautionticker++; // they become cautious
                    }
                }
            }
            if (cautionticker == 1){
                cout << cautionticker << " person in " << getlabel() << " became cautious because of lockdown." << endl;
            }
            else{
                cout << cautionticker << " people in " << getlabel() << " became cautious because of lockdown." << endl;
            }
            
        } // end of if lockdown is not in place
        
        // run the meetings
        run_meetings_once(1);
        
        // essential workers must still travel abroad sometimes, flights are reduced by 99%
        probtravel = probtravel * 0.01;
        
        // send the few that still travel on their way
        peopleout += calculate_abroad_number(probtravel);
    }
       
    
    
    // Update everyone, give them a chance to recover or die
    updatesick(symptomless_limit_input);
    killpeople(agebrackets, lethality_daily_values);
    
    // send the people who are travelling to limbo
    for (int i = 0; i < peopleout; i++){ // for every person going out
        // pick a random infected person
        double roll = rnd() * people.size();
        int index = floor(roll);
        
        // send them to limbo
        limbo.push_back(people[index]);
        
        // eliminate them from the country
        total_population--;
        people.erase(people.begin() + index);
        
    }
    
    // cout <<"Outsize: " << peopleout.size() << endl;
    // the day returns the people coming out of the country for further processing (where they're going etc)
    ;
    
} // end of runregionday


void simulate( vector <region> world, int days , vector <double> lethality_in, int printdays ){
    
    cout << endl << endl << "Starting simulation" << endl;
    
    // start the simulation by printing the info of all the regions invloved
    unsigned long world_population = 0;

    
    for (int i = 0; i < world.size(); i++){ // for all regions
        world[i].print_region_info();
        world_population += world[i].getsize();
    }
    
    cout << "Total world population: " << world_population << endl;
    
    // begin by clearing limbo in case there's any funny business
    limbo.clear();
    
    for (int i = 0; i < days; i++) // for all days
    {
        cout << "--------------------------------- Day " << i + 1 << " ---------------------------------" << endl;
        //cout << "people in limbo: " << limbo.size() << endl;
        
        for (int j = 0; j < world.size(); j++){ // for all regions
            
            // decide how many people are coming into the region
            unsigned long into_region_people = 0;
            // people in limbo travel 'randomly' between regions
            for (int k = 0; k < floor(limbo.size()); k++ ){
                double roll = rnd();
                if (roll < (1.0 / world.size())){
                    into_region_people++;
                }
            }
            // add people into regions from previous day
            world[j].peoplein(into_region_people);
        } // end of adding people in
        
        // there are still probably people in limbo left over
        if (limbo.size() != 0){
            cout << "------------------ "<< limbo.size() << " still travelling from previous day  ------------------"  << endl;
        }
        
        // now to run the days and spit people back into limbo for tomorrow
        for (int j = 0; j < world.size(); j++){ // for all regions
            // I'm saying that 90% of people take lockdown seriously...
            // lockdowns are based on recorded cases and not deaths...
            // people go aboad on average once a year
            world[j].runregionday(14, lethality_in, 0.9, 0, 365);
            
            // if the printout is desired or it is the last day, print the information
            if (printdays == 1 or i == (days-1)){
                world[j].print_region_info();
            }
        } // end of running all regions for day
    } // end of for all days
    cout << endl << endl << "End of simulation." << endl;
} // end of simulate









int main()
{
    
    
    vector <double> lethality = calculate_daily_probs(lethalitytotal, 7);
    // assuming that the average lifetime of the virus is a week
    
    region Italy;
    region UK;
    region China;
    region Spain;
    region Isle_of_mann;
    
    // string name, int population_input, int lockdown_threshold_input, int average_age_input, int initial_infected_input
    Italy.construct_region_normally("Italy", 1000, 100, 30, 1);
    UK.construct_region_normally("UK", 900, 100, 20, 0);
    China.construct_region_normally("China", 6000, 100, 20, 0);
    Spain.construct_region_normally("Spain", 950, 100, 20, 0);
    Isle_of_mann.construct_region_normally(" Isle of Mann", 2, 1, 25, 0);

    
    
    vector <region> World;
    World.push_back(Italy);
    World.push_back(UK);
    World.push_back(China);
    World.push_back(Spain);
    World.push_back(Isle_of_mann);
    
    simulate(World, 365, lethality, 0);
    
} // end of main


    






