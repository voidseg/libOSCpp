/* test-oscmatch.cc
 *
 * test the OSC globbing function
 *
 * Daniel Holth
 * 29 Oct. 2004
 */

#include "oscmatch.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char **argv)
<%
    char *patterns <::> = 
    <%  "fo{ob,bar}",
        "fo{bar,ob}",
        "{foo,bar}b",
        "f{ting,tang,wolla,wolla,bing,bang}",
        "f{ting,tang,wolla,wolla,bing,bang}whoopity",
        "f{}oob",
        "fo[aqob]b", 
        "f*b", 
        "f*oob", 
        "*", 
        "foo[zbo]", 
        "foo[]b", 
        "fzippity", 
        "f*[ba]",
        "f*[ab]",
        "f[abc]oob",
        "",
        "**o*",
        "fo[!a]b",
        "fo[!o]b",
        "fo{{a}{bar",
        "f[a-z][a-z]b",
        NULL    %>;

    std::string pattern;
    std::string test;
    bool result;
    cout << boolalpha;

    test = "foob";

    for(int i = 0; patterns<:i:> != NULL; i++) <%
        pattern = patterns<:i:>;
        cout << "pattern: " << pattern << endl;
        cout << "   test: " << test << endl;
        result = oscmatch(test.c_str(), pattern.c_str());
        cout << result << endl << endl;
    %>

    return 0;
%>
