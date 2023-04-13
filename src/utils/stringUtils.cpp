#include "../../include/stringUtils.h"

/**
 * \brief split string based on specific character.
 * 
 * @return set of splitted sub string. 
 **/
std::vector<std::string> split( std::string const& original, char separator )
{
    std::vector<std::string> results;
    std::string::const_iterator start = original.begin();
    std::string::const_iterator end = original.end();
    std::string::const_iterator next = std::find( start, end, separator );
    while ( next != end ) {
        results.push_back( std::string( start, next ) );
        start = next + 1;
        next = std::find( start, end, separator );
    }
    results.push_back( std::string( start, next ) );
    return results;
}


/**
 * \brief get the precision of a string number.
 * 
 **/
int get_precision_strNum(std::string str)
{
    if (str[0] == '-') str.erase(0,1);
    size_t count = 0;
    
    str = str.erase(0, str.find_first_not_of('0'));
    for ( std::string::size_type pos = 0;
          ( pos = str.find_first_of( "0123456789", pos ) ) != std::string::npos;
          ++pos )
    {
        ++count;
    } 

    return count;
}

/**
 * \brief check if the string is bit-string or not.
 * 
 **/
bool is_bitString(string str)
{
    if (str.find_first_not_of("01")!=std::string::npos) return false;
    return true;
}