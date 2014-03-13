/* newnames.cc 
 *
 * test the new hierarchical OSC namespace
 *
 * Daniel Holth
 * 18 Oct. 2004
 */

#include "OSCNamespaceItem.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool matchPattern(std::string &pattern, std::string &candidate);
void testPathFinder(std::string &address, unsigned int offset);
void Tokenize(const string& str, vector<string>& tokens);

// deprecated
// bool matchPattern(std::string &pattern, std::string &candidate);

// from some website
void Tokenize(const string& str, vector<string>& tokens)
{
    string delimiters = "/";

    string::size_type lastPos = str.find_first_not_of("/", 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of("/", lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void testPathFinder(std::string &address, unsigned int offset)
{
    std::string target;
    size_t next;

    if(offset >= address.size()) {
        if(address[address.size()-1] == '/') { cout << "terminates in slash: list subtree" << endl; }
        else cout << "does not end in slash, and probably a bad address" << endl;
        return;
    }

    // define OSC_PATH_SEPARATOR somewhere
    next = address.find('/', offset);
    target = address.substr(offset, next - offset);

    cout << "looking from " << offset << " component is [" << target << "]" << endl;

    if(next < address.size()) testPathFinder(address, next+1);
}


void printItem(void *cookie, std::vector < std::string > &address, OSCNamespaceItem *item)
{
    std::vector < std::string >::iterator i;
    std::vector < std::string >::iterator end;

    i = address.begin();
    end = address.end();

    cout << "len " << address.size();
    cout << " doing something with address: ";

    for( ; i != end ; i++)
    {
        cout << "/" << *i;
    }
    cout << endl;
}


int main(int argc, char **argv)
{
    std::ios::sync_with_stdio(false); // enhance cout performance for 0, 1, 2
    
    OSCNamespaceItem *item;   
    item = new OSCNamespaceItem();

    char *addresses[] = { "/foo/bar/baz", "/foo/", "/", "#bundle", "/foo/bar/baz/quux/poit/narf/zinggg", "/foo//bar", "/foo/bar/", "/foo/qux/noit", "/foo/zap/kapow", "/foo/zap/babing", "/foo/bar/boot", "/quux/zort", "/bing/crosby", "/lonely/goatherd", "/egg/crate/walking/on/four/legs/", "/foo", NULL };
    std::string current;

    for(int i = 0; addresses[i] != NULL; i++) {
        current = std::string(addresses[i]);
        cout << "looking for " << current << endl;
        testPathFinder(current, 0);
        cout << endl;

        item->add(current);
    }

    std::vector< std::string > tokens;

    /*
    for(int i = 0; addresses[i] != NULL; i++) {
        current = std::string(addresses[i]);
        Tokenize(current, tokens);
    }   

    for(unsigned int i=0; i < tokens.size(); i++)
    {
        cout << "[" << tokens[i] << "]" << endl;
    }
    */

    item->add(string("/foo"));
    item->add(string("/foo/bar/baz/quux"));
    item->list(0);

    /*
    for(int i = 0; addresses[i] != NULL; i++) {
        current = std::string(addresses[i]);
        cout << "search " << current << ":\n";
        cout << item->findOne(current, 0) << endl << endl;
    }
    */
    
    for(int i = 0; addresses[i] != NULL; i++) {
        current = std::string(addresses[i]);
        cout << "traverse " << current << ":\n";
        cout << item->traverse(current, &printItem) << endl << endl;
    }

    char *search[] = { "/foo/*", "/foo/*/*", NULL };

    for(int i = 0; search[i] != NULL; i++) {
        current = std::string(search[i]);
        cout << "traverse " << current << ":\n";
        cout << item->traverse(current, &printItem) << endl << endl;
    }
    
    delete item;

    return 0;
}
