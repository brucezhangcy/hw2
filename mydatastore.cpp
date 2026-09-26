#include <iostream>
#include "mydatastore.h"
#include "util.h"

using namespace std;

MyDataStore::MyDataStore()
{
}

MyDataStore::~MyDataStore()
{
    for (size_t i = 0; i < products_.size(); ++i) {
        delete products_[i];
    }
    for (map<string, User*>::iterator it = users_.begin(); it != users_.end(); ++it) {
        delete it->second;
    }
}

void MyDataStore::addProduct(Product* p)
{
    products_.push_back(p);
    set<string> keys = p->keywords();
    for (set<string>::iterator it = keys.begin(); it != keys.end(); ++it) {
        keywordMap_[*it].insert(p);
    }
}

void MyDataStore::addUser(User* u)
{
    string name = convToLower(u->getName());
    users_[name] = u;
    carts_[name];
}

vector<Product*> MyDataStore::search(vector<string>& terms, int type)
{
    set<Product*> result;
    for (size_t i = 0; i < terms.size(); ++i) {
        set<Product*> matches;
        map<string, set<Product*> >::iterator it = keywordMap_.find(terms[i]);
        if (it != keywordMap_.end()) {
            matches = it->second;
        }
        if (i == 0) {
            result = matches;
        }
        else if (type == 0) {
            result = setIntersection(result, matches);
        }
        else {
            result = setUnion(result, matches);
        }
    }
    return vector<Product*>(result.begin(), result.end());
}

bool MyDataStore::addToCart(const string& username, Product* p)
{
    map<string, deque<Product*> >::iterator it = carts_.find(convToLower(username));
    if (it == carts_.end()) {
        return false;
    }
    it->second.push_back(p);
    return true;
}

bool MyDataStore::viewCart(const string& username)
{
    map<string, deque<Product*> >::iterator it = carts_.find(convToLower(username));
    if (it == carts_.end()) {
        return false;
    }
    deque<Product*>& cart = it->second;
    for (size_t i = 0; i < cart.size(); ++i) {
        cout << "Item " << i + 1 << endl;
        cout << cart[i]->displayString() << endl;
        cout << endl;
    }
    return true;
}

bool MyDataStore::buyCart(const string& username)
{
    string name = convToLower(username);
    map<string, deque<Product*> >::iterator it = carts_.find(name);
    if (it == carts_.end()) {
        return false;
    }
    User* u = users_[name];
    deque<Product*>& cart = it->second;
    deque<Product*> remaining;
    for (size_t i = 0; i < cart.size(); ++i) {
        Product* p = cart[i];
        if (p->getQty() > 0 && u->getBalance() >= p->getPrice()) {
            p->subtractQty(1);
            u->deductAmount(p->getPrice());
        }
        else {
            remaining.push_back(p);
        }
    }
    cart = remaining;
    return true;
}

void MyDataStore::dump(ostream& ofile)
{
    ofile << "<products>" << endl;
    for (size_t i = 0; i < products_.size(); ++i) {
        products_[i]->dump(ofile);
    }
    ofile << "</products>" << endl;
    ofile << "<users>" << endl;
    for (map<string, User*>::iterator it = users_.begin(); it != users_.end(); ++it) {
        it->second->dump(ofile);
    }
    ofile << "</users>" << endl;
}
