#include <sstream>
#include <iomanip>
#include "book.h"
#include "util.h"

using namespace std;

Book::Book(const std::string name, double price, int qty,
           const std::string isbn, const std::string author) :
    Product("book", name, price, qty),
    isbn_(isbn),
    author_(author)
{
}

std::set<std::string> Book::keywords() const
{
    std::set<std::string> keys = parseStringToWords(name_);
    std::set<std::string> authorWords = parseStringToWords(author_);
    keys = setUnion(keys, authorWords);
    keys.insert(isbn_);
    return keys;
}

std::string Book::displayString() const
{
    ostringstream oss;
    oss << name_ << "\n"
        << "Author: " << author_ << " ISBN: " << isbn_ << "\n"
        << fixed << setprecision(2) << price_ << " " << qty_ << " left.";
    return oss.str();
}

void Book::dump(std::ostream& os) const
{
    Product::dump(os);
    os << isbn_ << "\n" << author_ << endl;
}
