// Read words and print them in reverse order.
// //   Variation using stack and string.
// // Fred Swartz 2001-11-08, 2001-12-04
//
 #include <iostream>
 #include <stack>
 #include <string>
 using namespace std;

 int main() {

     stack<string> allwords; // stack of all words
     string word;            // input buffer for words.

     // read words/tokens from input stream
     int i = 0;
     while (i < 5 && cin >> word) {
         allwords.push(word);
         i ++;
     }

     cout << "Number of words = " << allwords.size() << endl;

     // write out all the words in reverse order.
     while (!allwords.empty()) {
         cout << allwords.top() << endl;
         allwords.pop();            // remove top element
     }
     return 0;
 }//end main
