#include <iostream>
#include <vector>
#include <fstream> // Include the <fstream> header
#include <istream>
#include <string>
#include <sstream>

using namespace std;
ofstream ofile("Execution_Rep.csv");
ifstream ifile("Orders.csv");
vector<Order> orders;

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
        vector<string> status = {"New", "Rejected", "Fill", "PFill"};

        // first check the order is valid, if not write to the output file - (status = Rejected)
        if (order.reason != "")
        {
            ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[1] << order.quantity << "," << order.price << "," << order.reason << endl;
            return;
        }

        // create new item object
        OrderBookItem *item = new OrderBookItem(order.orderID, order.quantity, order.price);

        if (order.side == 1)
        {
            // if the sell vecotr is empty
            if (sell.empty())
            {
                // write to the output file - (status = New)
                ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[0] << "," << order.quantity << "," << order.price << "," << order.reason << endl;

                // place the order in the buy vector
                buy.push_back(*item);
                return;
            }

            // when the order is not matching with any sell orders,

            if (sell.begin()->price > order.price)
            {
                cout << "a";
                // write to the output file - (status = New)
                ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[0] << "," << order.quantity << "," << order.price << "," << order.reason << endl;

                // place corrcetly in buy vector, as buy vector is in the descending order of price
                for (int i = 0; i < buy.size(); i++)
                {
                    if (buy[i].price < order.price)
                    {
                        buy.insert(buy.begin() + i, *item);
                        return;
                    }
                    buy.insert(buy.begin(), *item);
                }
            }

            // when the order is matching with a sell orders,
            while (sell.begin()->price <= order.price && order.quantity > 0)
            {
                if (sell.begin()->quantity < order.quantity)
                {
                    // status of the order changed to PFill
                    ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[3] << sell.begin()->quantity << "," << order.price << "," << order.reason << endl;

                    order.quantity -= sell.begin()->quantity;

                    // status of the sell.begin() change to Fill
                    string b = sell.begin()->orderID;
                    //--auto ord = findOrderByOrdID(orders, b); // Assuming findOrderByOrdID is properly defined and returns the correct type
                    sell.erase(sell.begin());
                }
                else
                {

                    // status of the order changed to Fill
                    ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[2] << order.quantity << "," << order.price << "," << order.reason << endl;

                    sell.begin()->quantity -= order.quantity;
                    order.quantity = 0;

                    if (sell.begin()->quantity == 0)
                    {
                        // status of the sell.begin() change to Fill
                        //---
                        sell.erase(sell.begin());
                    }
                    else
                    {
                        // status of the sell.begin() change to PFill
                        //---
                    }
                    return;
                }
            }

            // place the order in the buy vector
            buy.insert(buy.begin(), *item);
        }
        else
        {
            if (buy.empty())
            {
                // write to the output file - (status = New)
                ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[0] << "," << order.quantity << "," << order.price << "," << order.reason << endl;

                // place the order in the sell vector
                sell.push_back(*item);
                return;
            }

            // when the order is not matching with any buy orders,

            if (buy.begin()->price < order.price)
            {

                // write to the output file - (status = New)
                ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[0] << "," << order.quantity << "," << order.price << "," << order.reason << endl;

                // place corrcetly in sell vector, as sell vector is in the ascending order of price
                for (int i = 0; i < sell.size(); i++)
                {
                    if (sell[i].price > order.price)
                    {
                        sell.insert(sell.begin() + i, *item);
                        return;
                    }
                    sell.push_back(*item);
                }
            }

            // when the order is matching with a buy order,
            while (buy.begin()->price >= order.price && order.quantity > 0)
            {
                if (buy.begin()->quantity < order.quantity)
                {
                    // status of the order changed to PFill
                    ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[3] << "," << buy.begin()->quantity << "," << buy.begin()->price << "," << order.reason << endl;

                    order.quantity -= buy.begin()->quantity;

                    // status of the buy.begin() change to Fill
                    //----
                    buy.erase(buy.begin());
                }
                else
                {

                    // status of the order changed to Fill
                    ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[2] << "," << order.quantity << "," << buy.begin()->price << "," << order.reason << endl;

                    buy.begin()->quantity -= order.quantity;
                    order.quantity = 0;

                    if (buy.begin()->quantity == 0)
                    {
                        // status of the buy.begin() change to Fill
                        //---
                        buy.erase(buy.begin());
                    }
                    else
                    {
                        // status of the buy.begin() change to PFill
                        //---
                    }
                    return;
                }
            }

            // place the order in the sell vector
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

    int ordNumber = 1;
    string line;

    // initially create 5 order books
    OrderBook *rose = new OrderBook("Rose");
    OrderBook *lavender = new OrderBook("Lavender");
    OrderBook *tulip = new OrderBook("Tulip");
    OrderBook *orchid = new OrderBook("Orchid");
    OrderBook *lotus = new OrderBook("Lotus");

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
        orders.push_back(*order);

        if (v[1] == "Rose")
        {
            rose->addOrder(*order);
        }
        else if (v[1] == "Lavender")
        {
            lavender->addOrder(*order);
        }
        else if (v[1] == "Tulip")
        {
            tulip->addOrder(*order);
        }
        else if (v[1] == "Orchid")
        {
            orchid->addOrder(*order);
        }
        else if (v[1] == "Lotus")
        {
            lotus->addOrder(*order);
        }
        else
        {

            ofile << ordNumberString << "," << v[0] << "," << v[1] << "," << v[2] << ","
                  << "Rejected"
                  << "," << v[3] << "," << v[4] << "," << a << endl;
        }

        ordNumber++;
    }
    ifile.close();
    ofile.close();

    cout << "finish" << endl;
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

Order *findOrderByOrdID(vector<Order> orders, string ordID)
{
    for (int i = 0; i < orders.size(); i++)
    {
        if (orders[i].orderID == ordID)
        {
            return &orders[i];
        }
    }
    return NULL;
}