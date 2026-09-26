#include <sstream>
#include <iomanip>
#include "movie.h"
#include "util.h"

using namespace std;

Movie::Movie(const std::string name, double price, int qty,
             const std::string genre, const std::string rating) :
    Product("movie", name, price, qty),
    genre_(genre),
    rating_(rating)
{
}

std::set<std::string> Movie::keywords() const
{
    std::set<std::string> keys = parseStringToWords(name_);
    keys.insert(convToLower(genre_));
    return keys;
}

std::string Movie::displayString() const
{
    ostringstream oss;
    oss << name_ << "\n"
        << "Genre: " << genre_ << " Rating: " << rating_ << "\n"
        << fixed << setprecision(2) << price_ << " " << qty_ << " left.";
    return oss.str();
}

void Movie::dump(std::ostream& os) const
{
    Product::dump(os);
    os << genre_ << "\n" << rating_ << endl;
}
