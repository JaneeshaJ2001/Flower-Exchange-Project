#include <iostream>
#include <vector>
#include <fstream> // Include the <fstream> header
#include <istream>
#include <string>
#include <sstream>

using namespace std;

string checkValidityOfOrder(vector<string> v);

class Order
{
public:
    string orderID;
    string clientOrderID;
    string instrument;
    int side;
    int status;
    int quantity;
    double price;
    string reason;

    Order(string orderID, string clientOrderID, string instrument, int side, int quantity, double price, string reason)
    {
        this->orderID = orderID;
        this->clientOrderID = clientOrderID;
        this->instrument = instrument;
        this->side = side;
        this->quantity = quantity;
        this->price = price;
        this->reason = reason;
    }
};

class OrderBookItem
{
public:
    string orderID;
    int quantity;
    double price;

    OrderBookItem(string orderID, int quantity, double price)
    {
        this->orderID = orderID;
        this->quantity = quantity;
        this->price = price;
    }
};

class OrderBook
{
private:
    string instrument;
    vector<OrderBookItem> buy;
    vector<OrderBookItem> sell;

public:
    OrderBook(string instrument)
    {
        this->instrument = instrument;
    }

    void addOrder(Order order)
    {
        if (order.side == 1)
        {
            // add to buy
            OrderBookItem *item = new OrderBookItem(order.orderID, order.quantity, order.price);
            buy.push_back(*item);
        }
        else
        {
            // add to sell
            OrderBookItem *item = new OrderBookItem(order.orderID, order.quantity, order.price);
            sell.push_back(*item);
        }
    }

    void printOrderBook()
    {
        cout << "Order Book: " << instrument << endl;
        cout << "Buy:" << endl;
        for (int i = 0; i < buy.size(); i++)
        {
            cout << buy[i].orderID << " " << buy[i].quantity << " " << buy[i].price << endl;
        }
        cout << "Sell:" << endl;
        for (int i = 0; i < sell.size(); i++)
        {
            cout << sell[i].orderID << " " << sell[i].quantity << " " << sell[i].price << endl;
        }
    }
};

int main()
{
    // read the Orders.csv file
    ifstream ifile("Orders.csv");
    int ordNumber = 1;
    string line;

    // read the header of the file
    getline(ifile, line);
    // read the column names of the file
    getline(ifile, line);

    // read all the rows in the input file
    while (getline(ifile, line))
    {

        stringstream ss(line);
        vector<string> v;
        string cell;

        while (getline(ss, cell, ','))
        {
            v.push_back(cell);
            // vector v contains data of 5 fiels of a row
        }

        // check the validity of the order

        string a = checkValidityOfOrder(v);
        string ordNumberString = "ord" + to_string(ordNumber);

        Order *order = new Order(ordNumberString, v[0], v[1], stoi(v[2]), stoi(v[3]), stod(v[4]), a);

        if (a.empty())
        {
            if (v[1] == "Rose")
            {
                OrderBook *rose = new OrderBook("Rose");
                rose->addOrder(*order);
                        }
            else if (v[1] == "Lavender")
            {
                OrderBook *lavender = new OrderBook("Lavender");
                lavender->addOrder(*order);
            }
            else if (v[1] == "Tulip")
            {
                OrderBook *tulip = new OrderBook("Tulip");
                tulip->addOrder(*order);
            }
            else if (v[1] == "Orchid")
            {
                OrderBook *orchid = new OrderBook("Orchid");
                orchid->addOrder(*order);
            }
            else if (v[1] == "Lotus")
            {
                OrderBook *lotus = new OrderBook("Lotus");
                lotus->addOrder(*order);
            }
        }
        ordNumber++;
    }
    ifile.close();
    return 0;
}

string checkValidityOfOrder(vector<string> v)
{

    string reason;

    // check if item is empty
    if (v[0].empty())
    {
        reason = "Invalid client order ID";
        return reason;
    }
    if (v[1].empty())
    {
        reason = "Invalid instrument";
        return reason;
    }
    if (v[2].empty())
    {
        reason = "Invalid side";
        return reason;
    }
    if (v[3].empty())
    {
        reason = "Invalid size";
        return reason;
    }
    if (v[4].empty())
    {
        reason = "Invalid price";
        return reason;
    }

    string clientOrderID = v[0];
    string instrument = v[1];
    int side = stoi(v[2]);
    int quantity = stoi(v[3]);
    double price = stod(v[4]);

    if (instrument != "Rose" && instrument != "Lavender" && instrument != "Tulip" && instrument != "Orchid" && instrument != "Lotus")
    {
        reason = "Invalid instrument";
        return reason;
    }
    if (side != 1 && side != 2)
    {
        reason = "Invalid side";
        return reason;
    }
    if (price < 0)
    {
        reason = "Invalid price";
        return reason;
    }
    if (quantity % 10 != 0 || 10 > quantity || quantity > 1000)
    {
        reason = "Invalid size";
        return reason;
    }

    return reason;
}
