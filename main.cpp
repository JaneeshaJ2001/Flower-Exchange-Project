#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <sstream>
#include <memory> // Include the <memory> header for smart pointers

using namespace std;

ofstream ofile("Execution_Rep.csv");

ifstream ifile("Orders.csv");

class Order;

class OrderBookItem
{
public:
    string orderID;
    int quantity;
    double price;

    OrderBookItem(string orderID, int quantity, double price)
        : orderID(orderID), quantity(quantity), price(price) {}
};

struct OrderBookItemComparator
{
    bool operator()(const OrderBookItem &a, const OrderBookItem &b) const
    {
        return a.price < b.price;
    }
};

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
        : orderID(orderID), clientOrderID(clientOrderID), instrument(instrument), side(side), quantity(quantity), price(price), reason(reason) {}
};

class OrderBook
{
private:
    string instrument;
    priority_queue<OrderBookItem, vector<OrderBookItem>, OrderBookItemComparator> buy;
    priority_queue<OrderBookItem, vector<OrderBookItem>, greater<OrderBookItem>> sell;

public:
    OrderBook(string instrument)
        : instrument(instrument) {}

    void addOrder(Order order)
    {
        vector<string> status = {"New", "Rejected", "Fill", "PFill"};

        if (!order.reason.empty())
        {
            ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[1] << "," << order.quantity << "," << order.price << "," << order.reason << endl;
            return;
        }

        OrderBookItem item(order.orderID, order.quantity, order.price);

        if (order.side == 1)
        {
            if (sell.empty() || sell.top().price > order.price)
            {
                ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[0] << "," << order.quantity << "," << order.price << "," << order.reason << endl;
                buy.push(item);
                return;
            }

            while (!sell.empty() && sell.top().price <= order.price && order.quantity > 0)
            {
                OrderBookItem sellTop = sell.top();
                sell.pop();

                int filledQuantity = min(order.quantity, sellTop.quantity);
                ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[3] << "," << filledQuantity << "," << order.price << "," << order.reason << endl;

                order.quantity -= filledQuantity;
                if (sellTop.quantity > filledQuantity)
                {
                    sellTop.quantity -= filledQuantity;
                    sell.push(sellTop);
                }
            }

            if (order.quantity > 0)
            {
                buy.push(item);
            }
        }
        else
        {
            if (buy.empty() || buy.top().price < order.price)
            {
                ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[0] << "," << order.quantity << "," << order.price << "," << order.reason << endl;
                sell.push(item);
                return;
            }

            while (!buy.empty() && buy.top().price >= order.price && order.quantity > 0)
            {
                OrderBookItem buyTop = buy.top();
                buy.pop();

                int filledQuantity = min(order.quantity, buyTop.quantity);
                ofile << order.orderID << "," << order.clientOrderID << "," << order.instrument << "," << order.side << "," << status[3] << "," << filledQuantity << "," << order.price << "," << order.reason << endl;

                order.quantity -= filledQuantity;
                if (buyTop.quantity > filledQuantity)
                {
                    buyTop.quantity -= filledQuantity;
                    buy.push(buyTop);
                }
            }

            if (order.quantity > 0)
            {
                sell.push(item);
            }
        }
    }

    void printOrderBook()
    {
        cout << "Order Book: " << instrument << endl;
        cout << "Buy:" << endl;

        priority_queue<OrderBookItem, vector<OrderBookItem>, OrderBookItemComparator> buyCopy = buy;
        while (!buyCopy.empty())
        {
            cout << buyCopy.top().orderID << " " << buyCopy.top().quantity << " " << buyCopy.top().price << endl;
            buyCopy.pop();
        }

        cout << "Sell:" << endl;

        priority_queue<OrderBookItem, vector<OrderBookItem>, greater<OrderBookItem>> sellCopy = sell;
        while (!sellCopy.empty())
        {
            cout << sellCopy.top().orderID << " " << sellCopy.top().quantity << " " << sellCopy.top().price << endl;
            sellCopy.pop();
        }
    }
};

int main()
{
    int ordNumber = 1;
    string line;

    unique_ptr<OrderBook> rose = make_unique<OrderBook>("Rose");
    unique_ptr<OrderBook> lavender = make_unique<OrderBook>("Lavender");
    unique_ptr<OrderBook> tulip = make_unique<OrderBook>("Tulip");
    unique_ptr<OrderBook> orchid = make_unique<OrderBook>("Orchid");
    unique_ptr<OrderBook> lotus = make_unique<OrderBook>("Lotus");

    getline(ifile, line); // Read header
    getline(ifile, line); // Read column names

    while (getline(ifile, line))
    {
        stringstream ss(line);
        vector<string> v;
        string cell;

        while (getline(ss, cell, ','))
        {
            v.push_back(cell);
        }

        string ordNumberString = "ord" + to_string(ordNumber);

        unique_ptr<Order> order = make_unique<Order>(ordNumberString, v[0], v[1], stoi(v[2]), stoi(v[3]), stod(v[4]), checkValidityOfOrder(v));

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
                  << "," << v[3] << "," << v[4] << "," << order->reason << endl;
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
