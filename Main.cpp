#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

// Appliance class definition
class Appliance {
private:
    string name;
    double powerRating;  // in watts
    double dailyHours;   // hours used per day
    
public:
    // Constructors
    Appliance() : name(""), powerRating(0), dailyHours(0) {}
    
    Appliance(string n, double p, double h) {
        setName(n);
        setPowerRating(p);
        setDailyHours(h);
    }
    
    // Setter methods with validation
    void setName(string n) {
        if (!n.empty()) {
            name = n;
        } else {
            name = "Unknown Appliance";
        }
    }
    
    bool setPowerRating(double p) {
        if (p > 0) {
            powerRating = p;
            return true;
        }
        return false;
    }
    
    bool setDailyHours(double h) {
        if (h >= 0 && h <= 24) {
            dailyHours = h;
            return true;
        }
        return false;
    }
    
    // Getter methods
    string getName() const { return name; }
    double getPowerRating() const { return powerRating; }
    double getDailyHours() const { return dailyHours; }
    
    // Calculate daily energy consumption in kWh
    double calculateDailyEnergy() const {
        return (powerRating * dailyHours) / 1000.0;  // Convert watt-hours to kWh
    }
    
    // Calculate monthly energy consumption (30 days)
    double calculateMonthlyEnergy() const {
        return calculateDailyEnergy() * 30;
    }
    
    // Display appliance details
    void display() const {
        cout << left << setw(20) << name 
             << setw(15) << powerRating 
             << setw(15) << dailyHours 
             << setw(15) << fixed << setprecision(2) << calculateDailyEnergy()
             << setw(15) << calculateMonthlyEnergy() << endl;
    }
    
    // Convert appliance to string for file storage
    string toFileString() const {
        return name + "|" + to_string(powerRating) + "|" + to_string(dailyHours);
    }
    
    // Create appliance from file string
    static Appliance fromFileString(const string& line) {
        Appliance app;
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        
        if (pos1 != string::npos && pos2 != string::npos) {
            string n = line.substr(0, pos1);
            double p = stod(line.substr(pos1 + 1, pos2 - pos1 - 1));
            double h = stod(line.substr(pos2 + 1));
            
            app.setName(n);
            app.setPowerRating(p);
            app.setDailyHours(h);
        }
        return app;
    }
};

// Function prototypes
void displayMainMenu();
void addAppliance(vector<Appliance>& appliances);
void viewAllAppliances(const vector<Appliance>& appliances);
void searchAppliance(const vector<Appliance>& appliances);
void calculateEnergySummary(const vector<Appliance>& appliances);
void calculateBill(const vector<Appliance>& appliances);
void saveToFile(const vector<Appliance>& appliances);
void loadFromFile(vector<Appliance>& appliances);
void saveBillingSummary(const vector<Appliance>& appliances, double tariff);
void clearInputBuffer();
bool getValidDouble(double& value, double min, double max, const string& prompt);

int main() {
    vector<Appliance> appliances;
    int choice;
    
    // Load data from file at startup
    loadFromFile(appliances);
    
    cout << "\n===============================================\n";
    cout << "   ELECTRICAL LOAD MONITORING & BILLING SYSTEM   \n";
    cout << "===============================================\n";
    
    do {
        displayMainMenu();
        cout << "Enter your choice (1-8): ";
        cin >> choice;
        
        // Handle invalid input
        if (cin.fail()) {
            cin.clear();
            clearInputBuffer();
            cout << "\n[ERROR] Invalid input! Please enter a number.\n";
            continue;
        }
        clearInputBuffer();
        
        switch(choice) {
            case 1:
                addAppliance(appliances);
                break;
            case 2:
                viewAllAppliances(appliances);
                break;
            case 3:
                searchAppliance(appliances);
                break;
            case 4:
                calculateEnergySummary(appliances);
                break;
            case 5:
                calculateBill(appliances);
                break;
            case 6:
                saveToFile(appliances);
                break;
            case 7:
                loadFromFile(appliances);
                break;
            case 8:
                cout << "\nSaving data before exit...\n";
                saveToFile(appliances);
                cout << "Thank you for using the system. Goodbye!\n";
                break;
            default:
                cout << "\n[ERROR] Invalid choice! Please enter a number between 1 and 8.\n";
        }
        
    } while (choice != 8);
    
    return 0;
}

void displayMainMenu() {
    cout << "\n------------------ MAIN MENU ------------------\n";
    cout << "1. Add New Appliance\n";
    cout << "2. View All Appliances\n";
    cout << "3. Search Appliance by Name\n";
    cout << "4. Calculate Energy Consumption Summary\n";
    cout << "5. Calculate Electricity Bill\n";
    cout << "6. Save Data to File\n";
    cout << "7. Load Data from File\n";
    cout << "8. Exit\n";
    cout << "------------------------------------------------\n";
}

void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool getValidDouble(double& value, double min, double max, const string& prompt) {
    cout << prompt;
    cin >> value;
    
    if (cin.fail()) {
        cin.clear();
        clearInputBuffer();
        cout << "[ERROR] Invalid input! Please enter a number.\n";
        return false;
    }
    
    if (value < min || value > max) {
        cout << "[ERROR] Value must be between " << min << " and " << max << ".\n";
        return false;
    }
    
    return true;
}

void addAppliance(vector<Appliance>& appliances) {
    cout << "\n----------- ADD NEW APPLIANCE -----------\n";
    
    string name;
    double power, hours;
    
    // Get appliance name
    cout << "Enter appliance name: ";
    getline(cin, name);
    
    // Get power rating
    while (!getValidDouble(power, 0.1, 10000, "Enter power rating (watts): ")) {
        // Continue until valid input
    }
    
    // Get daily usage hours
    while (!getValidDouble(hours, 0, 24, "Enter daily usage hours (0-24): ")) {
        // Continue until valid input
    }
    
    // Create and add appliance
    Appliance newAppliance(name, power, hours);
    appliances.push_back(newAppliance);
    
    cout << "\n[SUCCESS] Appliance added successfully!\n";
}

void viewAllAppliances(const vector<Appliance>& appliances) {
    cout << "\n----------- ALL REGISTERED APPLIANCES -----------\n";
    
    if (appliances.empty()) {
        cout << "No appliances registered yet.\n";
        return;
    }
    
    // Table header
    cout << left << setw(20) << "Name" 
         << setw(15) << "Power (W)" 
         << setw(15) << "Hours/Day"
         << setw(15) << "Daily (kWh)"
         << setw(15) << "Monthly (kWh)" << endl;
    cout << string(80, '-') << endl;
    
    // Display each appliance
    for (const auto& app : appliances) {
        app.display();
    }
    
    cout << string(80, '-') << endl;
    cout << "Total appliances: " << appliances.size() << endl;
}

void searchAppliance(const vector<Appliance>& appliances) {
    cout << "\n----------- SEARCH APPLIANCE -----------\n";
    
    if (appliances.empty()) {
        cout << "No appliances registered yet.\n";
        return;
    }
    
    string searchName;
    cout << "Enter appliance name to search: ";
    getline(cin, searchName);
    
    // Convert search term to lowercase for case-insensitive search
    string searchLower = searchName;
    transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
    
    vector<const Appliance*> found;
    
    for (const auto& app : appliances) {
        string appNameLower = app.getName();
        transform(appNameLower.begin(), appNameLower.end(), appNameLower.begin(), ::tolower);
        
        if (appNameLower.find(searchLower) != string::npos) {
            found.push_back(&app);
        }
    }
    
    if (found.empty()) {
        cout << "No appliances found with name containing \"" << searchName << "\".\n";
    } else {
        cout << "\nFound " << found.size() << " matching appliance(s):\n\n";
        
        // Table header
        cout << left << setw(20) << "Name" 
             << setw(15) << "Power (W)" 
             << setw(15) << "Hours/Day"
             << setw(15) << "Daily (kWh)" << endl;
        cout << string(65, '-') << endl;
        
        for (const auto* app : found) {
            cout << left << setw(20) << app->getName() 
                 << setw(15) << app->getPowerRating() 
                 << setw(15) << app->getDailyHours()
                 << setw(15) << fixed << setprecision(2) << app->calculateDailyEnergy() << endl;
        }
    }
}

void calculateEnergySummary(const vector<Appliance>& appliances) {
    cout << "\n----------- ENERGY CONSUMPTION SUMMARY -----------\n";
    
    if (appliances.empty()) {
        cout << "No appliances registered yet.\n";
        return;
    }
    
    double totalDaily = 0;
    double totalMonthly = 0;
    
    // Table header
    cout << left << setw(20) << "Name" 
         << setw(15) << "Daily (kWh)" 
         << setw(15) << "Monthly (kWh)"
         << setw(15) << "Annual (kWh)" << endl;
    cout << string(65, '-') << endl;
    
    // Calculate and display individual consumption
    for (const auto& app : appliances) {
        double daily = app.calculateDailyEnergy();
        double monthly = app.calculateMonthlyEnergy();
        double annual = monthly * 12;
        
        cout << left << setw(20) << app.getName() 
             << setw(15) << fixed << setprecision(2) << daily
             << setw(15) << monthly
             << setw(15) << annual << endl;
        
        totalDaily += daily;
        totalMonthly += monthly;
    }
    
    cout << string(65, '-') << endl;
    cout << left << setw(20) << "TOTAL" 
         << setw(15) << totalDaily
         << setw(15) << totalMonthly
         << setw(15) << (totalMonthly * 12) << endl;
    
    cout << "\nSummary Statistics:\n";
    cout << "Number of appliances: " << appliances.size() << endl;
    cout << "Average daily consumption: " << fixed << setprecision(2) 
         << (appliances.empty() ? 0 : totalDaily / appliances.size()) << " kWh\n";
}

void calculateBill(const vector<Appliance>& appliances) {
    cout << "\n----------- ELECTRICITY BILL CALCULATION -----------\n";
    
    if (appliances.empty()) {
        cout << "No appliances registered yet.\n";
        return;
    }
    
    double tariff;
    cout << "Enter electricity tariff per kWh (GHS/kWh): ";
    cin >> tariff;
    
    if (cin.fail() || tariff <= 0) {
        cin.clear();
        clearInputBuffer();
        cout << "[ERROR] Invalid tariff value! Please enter a positive number.\n";
        return;
    }
    
    double totalMonthly = 0;
    
    cout << "\n" << left << setw(20) << "Appliance" 
         << setw(15) << "Monthly (kWh)" 
         << setw(15) << "Cost (GHS)" << endl;
    cout << string(50, '-') << endl;
    
    for (const auto& app : appliances) {
        double monthly = app.calculateMonthlyEnergy();
        double cost = monthly * tariff;
        totalMonthly += monthly;
        
        cout << left << setw(20) << app.getName() 
             << setw(15) << fixed << setprecision(2) << monthly
             << setw(15) << cost << endl;
    }
    
    double totalCost = totalMonthly * tariff;
    
    cout << string(50, '-') << endl;
    cout << left << setw(20) << "TOTAL" 
         << setw(15) << totalMonthly
         << setw(15) << totalCost << endl;
    
    cout << "\nBILL SUMMARY:\n";
    cout << "Tariff rate: GHS " << fixed << setprecision(2) << tariff << " per kWh\n";
    cout << "Total monthly consumption: " << totalMonthly << " kWh\n";
    cout << "Total monthly cost: GHS " << totalCost << endl;
    cout << "Estimated quarterly cost: GHS " << (totalCost * 3) << endl;
    cout << "Estimated annual cost: GHS " << (totalCost * 12) << endl;
    
    // Ask if user wants to save billing summary
    char saveChoice;
    cout << "\nSave billing summary to file? (y/n): ";
    cin >> saveChoice;
    
    if (tolower(saveChoice) == 'y') {
        saveBillingSummary(appliances, tariff);
    }
    
    clearInputBuffer();
}

void saveToFile(const vector<Appliance>& appliances) {
    cout << "\n----------- SAVE DATA TO FILE -----------\n";
    
    ofstream outFile("appliances.txt");
    
    if (!outFile.is_open()) {
        cout << "[ERROR] Could not create or open appliances.txt for writing.\n";
        return;
    }
    
    outFile << appliances.size() << endl;
    
    for (const auto& app : appliances) {
        outFile << app.toFileString() << endl;
    }
    
    outFile.close();
    
    cout << "[SUCCESS] " << appliances.size() << " appliances saved to appliances.txt\n";
}

void loadFromFile(vector<Appliance>& appliances) {
    cout << "\n----------- LOAD DATA FROM FILE -----------\n";
    
    ifstream inFile("appliances.txt");
    
    if (!inFile.is_open()) {
        cout << "No existing data file found. Starting with empty list.\n";
        return;
    }
    
    int count;
    inFile >> count;
    inFile.ignore(); // Ignore newline after count
    
    if (count <= 0 || count > 100) {
        cout << "[ERROR] Invalid data in file.\n";
        inFile.close();
        return;
    }
    
    appliances.clear();
    string line;
    
    for (int i = 0; i < count; i++) {
        if (getline(inFile, line)) {
            Appliance app = Appliance::fromFileString(line);
            appliances.push_back(app);
        }
    }
    
    inFile.close();
    
    cout << "[SUCCESS] Loaded " << appliances.size() << " appliances from appliances.txt\n";
}

void saveBillingSummary(const vector<Appliance>& appliances, double tariff) {
    ofstream outFile("billing_summary.txt");
    
    if (!outFile.is_open()) {
        cout << "[ERROR] Could not create billing_summary.txt\n";
        return;
    }
    
    double totalMonthly = 0;
    
    outFile << "ELECTRICAL LOAD MONITORING - BILLING SUMMARY\n";
    outFile << "==============================================\n";
    outFile << "Date: " << __DATE__ << " " << __TIME__ << endl;
    outFile << "Tariff Rate: GHS " << fixed << setprecision(2) << tariff << " per kWh\n\n";
    
    outFile << left << setw(20) << "Appliance" 
            << setw(15) << "Power (W)" 
            << setw(15) << "Hours/Day"
            << setw(15) << "Monthly (kWh)"
            << setw(15) << "Cost (GHS)" << endl;
    outFile << string(80, '-') << endl;
    
    for (const auto& app : appliances) {
        double monthly = app.calculateMonthlyEnergy();
        double cost = monthly * tariff;
        totalMonthly += monthly;
        
        outFile << left << setw(20) << app.getName() 
                << setw(15) << app.getPowerRating()
                << setw(15) << app.getDailyHours()
                << setw(15) << fixed << setprecision(2) << monthly
                << setw(15) << cost << endl;
    }
    
    double totalCost = totalMonthly * tariff;
    
    outFile << string(80, '-') << endl;
    outFile << left << setw(50) << "TOTAL MONTHLY CONSUMPTION:" 
            << setw(15) << totalMonthly << " kWh" << endl;
    outFile << left << setw(50) << "TOTAL MONTHLY COST:" 
            << setw(15) << "GHS " << totalCost << endl;
    outFile << left << setw(50) << "TOTAL ANNUAL COST:" 
            << setw(15) << "GHS " << (totalCost * 12) << endl;
    
    outFile.close();
    
    cout << "[SUCCESS] Billing summary saved to billing_summary.txt\n";
}
