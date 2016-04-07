#include <iostream>
#include <string>  
#include "common.h"

using namespace std;

int main ()
{
   string html_content;
   load_template("./html_tmpl/login.html", html_content);

   cout << "Content-type:text/html\r\n\r\n";
   cout << html_content;
   
   return 0;
}

