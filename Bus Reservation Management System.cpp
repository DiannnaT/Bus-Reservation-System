#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <regex>    
#include <stdexcept> 
#include <ctime>
#include <cmath>
#include <string>
#include <vector>
#define N 100
using namespace std;

using StringVector = vector<string>;

int gcd(int a, int b)
{
    int t;
    while (1)
    {
        t = a % b;
        if (t == 0)
            return b;
        a = b;
        b = t;
    }
}
string RSA(string message)
{

    double p = 13;
    double q = 11;
    double n = p * q;
    string encrypted_message;
    double e = 7;
    double phi = (p - 1) * (q - 1);

    while (e < phi)
    {
        if (gcd(e, phi) == 1)
            break;
        else
            e++;
    }
    for (int i = 0; i < message.length(); i++)
    {
        double msg = static_cast<double>(message[i]); 
        double c = pow(msg, e);                       
        c = fmod(c, n);
        char encrypted_letter = static_cast<char>(c);
        encrypted_message.push_back(encrypted_letter);
    }
    return encrypted_message;
}

bool existCityInRomania(string city)
{
    ifstream cities;
    string fcity;
    bool cityFound;

    cities.open("orase.txt");
    while (getline(cities, fcity))
    {
        if (!fcity.compare(city))
        {
            cityFound = true;
        }
    }
    if (!cityFound)
    {
        cout << city << " doesn't exist in database. Try again!\n";
        cityFound = false;
    }
    cities.close();
    return cityFound;
}

bool isValidDateFormat(string dateStr)
{
    // Expresia regulată pentru formatul "YYYY-MM-DD"
    regex pattern(R"(\d{4}-\d{2}-\d{2})");
    return regex_match(dateStr, pattern);
}

bool isValidTimeFormat(string timeStr)
{
    // Expresia regulată pentru formatul "HH:MM"
    regex pattern(R"(\d{2}:\d{2})");
    return regex_match(timeStr, pattern);
}

bool isValidDateInThePast(string dateStr)
{
    int y, m, d;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    y = 1900 + ltm->tm_year;
    m = 1 + ltm->tm_mon;
    d = ltm->tm_mday;

    int year, month, day;
    istringstream iss(dateStr);

    char dash = '-';
    iss >> setw(4) >> year >> dash >> setw(2) >> month >> dash >> setw(2) >> day;

    if (year > y || (year == y && month > m) ||
        (year == y && month == m && day >= d))
    {
        if (month == 2 && day <= 29)
        {
            return true;
        }
        if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day <= 31)
        {
            return true;
        }
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day <= 30)
        {
            return true;
        }
    }
    return false;
}

bool isValidTimeInPast(string timeStr)
{
    int h, m;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    h = ltm->tm_hour;
    m = ltm->tm_min;

    int hour, minute;
    istringstream iss(timeStr);

    char dash; 
    iss >> hour >> dash >> minute;

    if (hour <= 23 && minute <= 59)
    {
        return true;
    }
    return false;
}

bool isValidEmail(string input)
{
    regex pattern(R"(^[\w-\.]+@([\w-]+\.)+[\w-]{2,4}$)");
    return regex_match(input, pattern);
}

bool isValidPassword(string input)
{
    if (input.length() < 8)
    {
        return false;
    }
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    for (char ch : input)
    {
        if (isupper(ch))
        {
            hasUpper = true;
        }
        else if (islower(ch))
        {
            hasLower = true;
        }
        else if (isdigit(ch))
        {
            hasDigit = true;
        }
        else if (!isalnum(ch))
        {
            hasSpecial = true;
        }

        if (hasUpper && hasLower && hasDigit && hasSpecial)
            break;
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

bool isValidCityName(string cityName)
{
    for (char c : cityName)
    {
        if (!isalpha(c) && !isspace(c))
        {
            // Verifică dacă caracterul nu este literă și nu este spațiu
            return false;
        }
    }
    return true;
}

void interface2()
{
    int choice;
    cout << "\t\t\t\t______________________________________\n";
    cout << "\t\t\t\t                                      \n";
    cout << "\t\t\t\t              Welcome!                 \n";
    cout << "\t\t\t\t                                      \n";
    cout << "\t\t\t\t______________________________________\n";
    cout << "\t\t\t\t                                      \n";
    cout << "\t\t\t\t|    1) Operator    |\n";
    cout << "\t\t\t\t|                   |\n";
    cout << "\t\t\t\t|    2) Client      |\n";
    cout << "\t\t\t\t|                   |\n";
    cout << "\t\t\t\t|    3) Exit        |\n";
    cout << "\t\t\t\t|                   |\n";
}

class Bus
{
public:
    string departureCity;
    string destinationCity;
    string date, time;
    string ticketPrice;

    Bus()
    {
        departureCity = "";
        destinationCity = "";
        date = "";
        time = "";
        ticketPrice = "";
    }
    void displayDetails()
    {
        cout << "Bus trip details:\n";
        cout << "Departure city: " << departureCity << "\n";
        cout << "Destination city: " << destinationCity << "\n";
        cout << "Date: " << date << "\n";
        cout << "Ticket price: $" << ticketPrice << "\n";
    }

    vector<Bus> searchRoute(int *foundRoutesCount)
    {
        vector<Bus> availableRoutes(N);
        try
        {
            int i = 0;
            cout << "Departure station: ";
            cin >> departureCity;
            cout << "Destination: ";
            cin >> destinationCity;

            string dep, dest, d, t, price;
            bool destFound = false, depFound = false;
            ifstream routes;
            routes.open("routes.csv");
            if (!routes.is_open())
            {
                throw runtime_error("Error opening routes.csv.");
            }

            while (getline(routes, dep, ','))
            {
                getline(routes, dest, ',');
                getline(routes, d, ',');
                getline(routes, t, ',');
                getline(routes, price);

                if (!dep.compare(departureCity) && !dest.compare(destinationCity))
                {
                    depFound = true;
                    destFound = true;

                    availableRoutes[i].departureCity = dep;
                    availableRoutes[i].destinationCity = dest;
                    availableRoutes[i].date = d;
                    availableRoutes[i].time = t;
                    availableRoutes[i].ticketPrice = price;

                    cout << "\nOption : " << i + 1 << endl;
                    cout << "From: " << dep << "\nTo: " << dest << endl;
                    cout << "Date: " << d << endl;
                    cout << "Time: " << t << endl;
                    cout << "Price: " << price << endl;
                    i++;
                }
            }
            if (!depFound || !destFound)
            {
                throw runtime_error("No route found between specified cities. Try another route!\n\n");
            }
            routes.close();
            *foundRoutesCount = i;
            return availableRoutes;
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
            availableRoutes.clear();
            searchRoute(foundRoutesCount);
            return availableRoutes;
        }
    }
};

class Operator
{
private:
    string userName;
    string password;

public:
    Bus bus;
    void menu()
    {

        int c;
        cout << "\t\t\t___________________________________________\n\n\n";
        cout << "\t\t\t           Welcome to the Login Page        \n\n\n";
        cout << "\t\t\t_______________    MENU    ___________________\n\n\n";
        cout << "                                                     \n\n\n";
        cout << "\t| Press 1 to LOGIN                        |" << endl;
        cout << "\t| Press 2 to RETURN to main menu          |" << endl;
        cout << "\t| Press 3 to EXIT                         |" << endl;
        cout << "\n\t\t\t Please enter your choice : ";
        cin >> c;
        cout << endl;

        switch (c)
        {
        case 1:
            login();
            opAfterLogin();
            break;
        case 2:
            interface2();
            break;
        case 3:
            cout << "\t\t\t Thank you! \n\n";
            break;
        default:
            system("cls");
            cout << "\t\t\t Please select from the options given above \n"
                 << endl;
        }
    }

    void login()
    {
        try
        {
            cout << "\t\t\t Please enter your username : " << endl;
            cout << "\t\t\t USERNAME : " << endl;
            cin >> userName;

            string line, user_id, pass;
            bool userFound = false;
            ifstream fin("operators.csv");

            if (!fin.is_open())
            {
                throw runtime_error("ERROR: Unable to open file");
            }

            while (getline(fin, user_id, ','))
            {
                getline(fin, pass);

                if (!user_id.compare(userName))
                {
                    userFound = true;
                    cout << "\t\t\t PASSWORD : " << endl;
                    cin >> password;

                    if (!pass.compare(password))
                    {
                        cout << "Login was successful!\n\n";
                        fin.close();
                        return;
                    }
                    else
                    {
                        throw runtime_error("Incorrect password!");
                    }
                }
            }
            if (!userFound)
            {
                throw runtime_error("Username not found!");
            }
        }
        catch (const runtime_error &e)
        {
            cout << "Error: " << e.what() << endl;
            login();
        }
    }

    void opAfterLogin()
    {

        int c;
        cout << "\t\t\t___________________________________________\n\n\n";
        cout << "\t\t\t                 Welcome, Operator!        \n\n\n";
        cout << "\t\t\t_______________      MENU     ___________________\n\n\n";
        cout << "                                                     \n\n\n";

        cout << "\t|    Press 1 to Add Route                     |" << endl;
        cout << "\t|    Press 2 to Remove Route                  |" << endl;
        cout << "\t|    Press 3 to Return to operator menu       |" << endl;
        cout << "\t|    Press 4 to EXIT                          |" << endl;
        cout << "\n\t\t\t Please enter your choice : ";
        cin >> c;
        cout << endl;

        switch (c)
        {
        case 1:
            addRoute();
            break;
        case 2:
            removeRoute();
            break;
        case 3:
            menu();
            break;
        case 4:
            cout << "\t\t\t Thank you! \n\n";
            break;
        default:
            system("cls");
            cout << "\t\t\t Please select from the options given above \n"
                 << endl;
        }
    }

    void addRoute()
    {
        fstream fout;
        fout.open("routes.csv", ios::out | ios::app);
        try
        {
            if (bus.departureCity == "")
            {
                cout << "Enter departure city: ";
                cin >> bus.departureCity;
                if (existCityInRomania(bus.departureCity) == false)
                {
                    bus.departureCity = "";
                    throw invalid_argument("The departure city does not exist in Romania.");
                }
            }
            if (bus.destinationCity == "")
            {
                cout << "Enter destination city: ";
                cin >> bus.destinationCity;
                if (!existCityInRomania(bus.destinationCity))
                {
                    bus.destinationCity = "";
                    throw invalid_argument("The destination city does not exist in Romania.");
                }
            }
            if (bus.date == "")
            {
                cout << "Enter date (format: YYYY-MM-DD): ";
                cin >> bus.date;
                if (!isValidDateFormat(bus.date) || !isValidDateInThePast(bus.date))
                {
                    bus.date = "";
                    throw invalid_argument("Invalid date or date is in the past.");
                }
            }
            if (bus.time == "")
            {
                cout << "Enter time (format: HH:MM): ";
                cin >> bus.time;
                if (!isValidTimeFormat(bus.time) || !isValidTimeInPast(bus.time))
                {
                    bus.time = "";
                    throw invalid_argument("Invalid time or time is in the past.");
                }
            }
            cout << "Enter ticket price: $";
            cin >> bus.ticketPrice;

            // Insert the data to file
            fout << bus.departureCity << ","
                 << bus.destinationCity << ","
                 << bus.date << ","
                 << bus.time << ","
                 << bus.ticketPrice
                 << "\n";
        }
        catch (const invalid_argument &e)
        {
            cout << "Error: " << e.what() << endl;
            addRoute();
        }
    }

    void removeRoute()
    {
        cout << "Please enter the Departure City and the Destination city of the route you want to remove from the database: " << endl;
        cout << "Departure City: " << endl;
        cin >> bus.departureCity;
        cout << "Destination City: " << endl;
        cin >> bus.destinationCity;

        string depC, destC, line;
        bool routeFound = false;
        ifstream routes;
        ofstream temp;

        try
        {
            temp.open("temp.csv");
            routes.open("routes.csv");
            if (!temp.is_open() || !routes.is_open())
            {
                throw runtime_error("Error opening file.");
            }

            while (getline(routes, depC, ','))
            {
                getline(routes, destC, ',');
                getline(routes, line);
                if (!(depC.compare(bus.departureCity) == 0 && destC.compare(bus.destinationCity) == 0))
                {
                    temp << depC << ',' << destC << ',' << line << endl;
                }
                if (depC.compare(bus.departureCity) == 0 && destC.compare(bus.destinationCity) == 0)
                {
                    routeFound = true;
                }
            }

            temp.close();
            routes.close();
            remove("routes.csv");
            rename("temp.csv", "routes.csv");

            if (!routeFound)
            {
                throw runtime_error("Route not found in the database.");
            }

            cout << "Route deleted from database\n";
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
            removeRoute();
        }
    }
};

class Utilizator
{
private:
    string userName;
    string password;

public:
    Utilizator()
    {
        userName = "";
        password = "";
    }
    void menu()
    {
        int c;
        cout << "\t\t\t___________________________________________\n\n\n";
        cout << "\t\t\t           Welcome to the Login Page        \n\n\n";
        cout << "\t\t\t_______________    MENU    ___________________\n\n\n";
        cout << "                                                     \n\n\n";
        cout << "\t| Press 1 to LOGIN                        |" << endl;
        cout << "\t| Press 2 to REGISTER                     |" << endl;
        cout << "\t| Press 3 to RETURN to main menu          |" << endl;
        cout << "\t| Press 4 to EXIT                         |" << endl;

        cout << "\n\t\t\t Please enter your choice : ";
        cin >> c;
        cout << endl;

        switch (c)
        {
        case 1:
            login();
            clientAfterLogin();
            break;
        case 2:
            registration();
            break;
        case 3:
            interface2();
            break;
        case 4:
            cout << "\t\t\t Thank you! \n\n";
            break;
        default:
            system("cls");
            cout << "\t\t\t Please select from the options given above \n"
                 << endl;
        }
    }

    void login()
    {
        try
        {
            string user_id, pass, line;
            cout << "\t\t\t Please enter your username : " << endl;
            cout << "\t\t\t USERNAME : " << endl;
            cin >> userName;

            ifstream client("registrations.csv");
            if (!client.is_open())
            {
                throw runtime_error("ERROR: Unable to open file");
            }
            string s;
            bool Found = false;

            while (getline(client, user_id, ','))
            {
                getline(client, pass);
                if (!user_id.compare(userName))
                {
                    Found = true;
                    cout << "\t\t\t PASSWORD : " << endl;
                    cin >> password;
                    if (!pass.compare(RSA(password)))
                    {
                        cout << "Login was successful!\n\n";
                        client.close();
                        return;
                    }
                    else
                    {
                        throw runtime_error("Incorrect password!");
                    }
                }
            }
            if (!Found)
            {
                throw runtime_error("Username not found!");
            }
        }
        catch (const runtime_error &e)
        {
            cout << "Error: " << e.what() << endl;
            login();
        }
    }

    void registration()
    {
        string line, ruser_id, rpassword, rpassword_confirm, s;
        bool Found = false;
        try
        {
            ofstream clientoutput; // pt. scriere
            ifstream clientinput;  // pt. citire
            clientinput.open("registrations.csv");
            clientoutput.open("registrations.csv", ios::app);

            if (userName == "")
            {
                cout << "\t\t\t Enter the username (email) : " << endl;
                cin >> userName;
            }
            bool valid = isValidEmail(userName);

            if (valid)
            {
                while (getline(clientinput, ruser_id, ','))
                {
                    getline(clientinput, rpassword);
                    if (!ruser_id.compare(userName))
                    {
                        Found = true;
                        cout << "Existing username. Try another one!\nEnter username: ";
                        cin >> userName;
                        if (!isValidEmail(userName))
                        {
                            userName = "";
                            throw invalid_argument("Invalid email format. Please use valid email format (name@gmail.com).");
                        }
                        clientinput.clear();
                        clientinput.seekg(0, ios::beg);
                    }
                }

                cout << "\t\t\t Enter the password : " << endl;
                cin >> password;
                if (!isValidPassword(password))
                {
                    throw invalid_argument("Password does not match the criteria. It must contain at least 8 characters, at least one uppercase letter, at least one lowercase letter, at least one number, and at least one symbol.");
                }
                cout << "\t\t\t Confirm password: " << endl;
                cin >> rpassword_confirm;

                if (password != rpassword_confirm)
                {
                    cout << "\t\t\t Passwords don't match. Try again!\n";
                    cout << "\nEnter password: ";
                    cin >> password;
                    if (!isValidPassword(password))
                    {
                        throw invalid_argument("Password does not match the criteria. It must contain at least 8 characters, at least one uppercase letter, at least one lowercase letter, at least one number, and at least one symbol.");
                    }
                    cout << "Confirm password: ";
                    cin >> rpassword_confirm;
                }

                if (clientinput.is_open())
                    clientoutput << userName << ',' << RSA(password) << endl;

                clientinput.close();
                clientoutput.close();
                cout << "\n\t\t\t Registration is successfull! \n";
            }
            else
            {
                userName = "";
                throw invalid_argument("Invalid email format. Please use valid email format (name@gmail.com).");
            }
        }
        catch (const invalid_argument &e)
        {
            cout << "Error: " << e.what() << " Please try again." << endl;
            registration();
        }
    }

    void clientAfterLogin()
    {
        Bus bus;
        int i = 0;
        vector<Bus> bus2(N);
        int c;
        cout << "\t\t\t___________________________________________\n\n\n";
        cout << "\t\t\t                 Welcome, Client!        \n\n\n";
        cout << "\t\t\t_______________      MENU     ___________________\n\n\n";
        cout << "                                                     \n\n\n";

        cout << "\t|    Press 1 to Search Route                     |" << endl;
        cout << "\t|    Press 2 to Make reservation                 |" << endl;
        cout << "\t|    Press 3 to Return to client menu            |" << endl;
        cout << "\t|    Press 4 to EXIT                             |" << endl;
        cout << "\n\t\t\t Please enter your choice : ";
        cin >> c;
        cout << endl;

        switch (c)
        {
        case 1:
            bus2 = bus.searchRoute(&i);
            break;
        case 2:
            Reservation();
            break;
        case 3:
            menu();
            break;
        case 4:
            cout << "\t\t\t Thank you! \n\n";
            break;
        default:
            system("cls");
            cout << "\t\t\t Please select from the options given above \n"
                 << endl;
        }
    }

    void Reservation()
    {
        vector<Bus> availableRoutes(N);
        Bus bus;
        int nr = 0;
        int choice;

        try
        {
            availableRoutes = bus.searchRoute(&nr);
            bool ok = false;
            ofstream reservations;
            reservations.open("reservations.csv", ios::app);

            while (!ok)
            {
                cout << "\nSelect the route: \n";
                cin >> choice;
                choice--;

                if (choice < 0 || choice > nr - 1)
                {
                    throw out_of_range("Invalid choice. Please select again!");
                }

                // Salvăm rezervarea în fișier
                reservations << userName << ',' << availableRoutes[choice].departureCity << ',' << availableRoutes[choice].destinationCity << ',' << availableRoutes[choice].date << ',' << availableRoutes[choice].time << ',' << availableRoutes[choice].ticketPrice << endl;
                cout << "Reservation was successful!\n\n";
                ok = true;
            }
            reservations.close();
        }
        catch (const exception &e)
        {
            cerr << "Exception: " << e.what() << endl;
            Reservation();
        }
    }
};

void interface()
{
    int choice;
    Operator op;
    Utilizator ut;
    cout << "\t\t\t\t______________________________________\n";
    cout << "\t\t\t\t                                      \n";
    cout << "\t\t\t\t              Welcome!                 \n";
    cout << "\t\t\t\t                                      \n";
    cout << "\t\t\t\t______________________________________\n";
    cout << "\t\t\t\t                                      \n";
    cout << "\t\t\t\t|    1) Operator    |\n";
    cout << "\t\t\t\t|                   |\n";
    cout << "\t\t\t\t|    2) Client      |\n";
    cout << "\t\t\t\t|                   |\n";
    cout << "\t\t\t\t|    3) Exit        |\n";
    cout << "\t\t\t\t|                   |\n";

    cout << "\n\t\t\t Please select ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        op.menu();
        break;
    case 2:
        ut.menu();
        break;
    case 3:
        cout << "\t\t\t Thank you! \n\n";
        break;
    default:
        // system("cls");
        cout << "\t\t\t Please select from the options given above \n"
             << endl;
    }
}

int main()
{
    interface();
    Operator op1;
    Bus b, b2;
    op1.bus = b;
    // op1.menu();
    // op1.login();
    // op1.opAfterLogin();
    // op1.addRoute();
    // op1.removeRoute();

    vector<Bus> availableRoutes(N);
    int numberOfRoutes = 0;
    // availableRoutes = b2.searchRoute(&numberOfRoutes);

    Utilizator u1;
    // u1.menu();
    // u1.registration();
    // u1.login();
    // u1.clientAfterLogin();
    // u1.Reservation();

    return 0;
}