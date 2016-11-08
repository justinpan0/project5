// ENGR 151
// Project 5
// Filename: project5.cpp
//
// Authors: Zimeng Pan (lab section: 101)
// Uniqnames: zmpan
//
// Date Created: Nov. 5th
// Date Submitted:
//
// Program Description:
// Based on the inputs, calculate the theoretical prices. Then decide whether to
// buy for less than the theoretical price or sell for higher than the theoretical
// price. Record the price difference as a profit.

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

double max(const double &x){
    if(x >= 0){
        return x;
    } else {
        return 0;
    }
}

double callPrice(double S, const double &K, const double &u, const double &d,
                 const double &r, int timeStep, const int &totalTimeSteps){
    /*
     * S – The stock price of the current time period
     * K – the strike price
     * U – the Up factor in the binomial tree
     * D – the Down factor in the binomial tree
     * R – the interest rate per time period
     * timeStep – the current time step you are looking at, counting backwards (The last time step is 0)
     * totalTimeSteps – the user inputed number of time steps
    */
    if(timeStep == 0){
        //cout << max(S-K) << endl;
        return max(S-K); // End node at T = 0 of the callPrice recursion
    } else {
        double p = (1 + r - d)/(u-d);
        return (p * callPrice(S * u, K, u, d, r, timeStep - 1, totalTimeSteps) +
                (1 - p) * callPrice(S * d, K, u, d, r, timeStep-1, totalTimeSteps))/(1 + r);
    }
}

double putPrice(double S, const double &K, const double &u, const double &d,
                 const double &r, int timeStep, const int &totalTimeSteps){
    if(timeStep == 0){
        //cout << max(K-S) << endl;
        return max(K-S); // End node at T = 0 of the putPrice recursion
    } else {
        double p = (1 + r - d)/(u-d);
        // By mathematical induction, we have f(x_(t) ⊥ x_(t-2) | x_(t-1)),
        // which in general becomes a recursion case
        return (p * putPrice(S * u, K, u, d, r, timeStep - 1, totalTimeSteps) +
                (1 - p) * putPrice(S * d, K, u, d, r, timeStep - 1, totalTimeSteps))/(1 + r);
    }
}

int main() {
    /* Input Format & Meaning:
     * S - Today’s stock price
     * U – the up factor for the binomial tree.
     * r – the interest rate per period
     * T – the number of periods in the binomial model
     * filename - The file name in which “trader information” is located*/

    //cout << callPrice(4, 5, 2, 0.5, 0.25, 1, 1) << endl;
    //cout << putPrice(4, 5, 2, 0.5, 0.25, 2, 2) << endl;
    double S, U, r, strike;
    int T;
    string filename, name, op, transaction;
    double price, profit, total = 0;
    cout << "Enter today's stock price (S): ";
    cin >> S;
    cout << "Enter the up factor (U): ";
    cin >> U;
    cout << "Enter the interest rate (r): ";
    cin >> r; 
    cout << "Enter the number of periods (T): ";
    cin >> T;
    cout << "Enter the trader information filename: ";
    cin >> filename;

    ifstream in(filename.c_str());
    while(in >> name){
        in >> op >> strike >> transaction >> price;
        if(op != "call" && op != "put"){
            cout << name << " has not properly inputted the option type. Transaction "
                    "will be ignored." << endl;
        } else if(transaction != "buy" && transaction != "sell"){
            cout << name << " has not properly inputted whether they are buying or selling. "
                    "Transaction will be ignored." << endl;
        } else {
            if(op == "call"){
                profit = price - callPrice(S, strike, U, 1/U, r, T, T);
                if(transaction == "buy"){
		    //cout << name << profit << endl;
                    if(profit > 0){
                        total += profit;
                        cout << "I will sell to " + name + " in order to record a $" << profit <<" profit." << endl;
                    }
                } else {
                    if(profit < 0){
                        total -= profit;
                        cout << "I will buy from " + name + " in order to record a $" << -profit <<" profit." << endl;
                    }
                }
            } else {
                profit = price - putPrice(S, strike, U, 1/U, r, T, T);
                if(transaction == "buy"){
                    if(profit > 0){
                        total += profit;
                        cout << "I will sell to " + name + " in order to record a $" << profit <<" profit." << endl;
                    }
                } else {
                    if(profit < 0){
                        total -= profit;
                        cout << "I will buy from " + name + " in order to record a $" << -profit <<" profit." << endl;
                    }
                }
            }
        }
    }

    cout << "The total profit is: $" << total << endl;

    return 0;
}
