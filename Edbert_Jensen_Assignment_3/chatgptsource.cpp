/*
Author Name: Edbert Jensen
Program Name: EECS 348 Assignment 3
Brief description: Email Priority Program that has an hierarchical order from most to least important (Boss -> Subordinate -> Peer -> ImportantPerson -> OtherPerson)
Inputs: multiple lines of commands (EMAIL, NEXT, READ, COUNT) with their respective parameters
Outputs: Number of emails to read and the next email if NEXT command and COUNT command are called
Collaborators: Chatgpt
Other sources: Chatgpt, Gemini
Creation date: 9/29/2026 5:00PM
Revision date: 10/2/2026 5:00AM
Revisions: create the file
*/

#include <iostream> // Includes standard I/O library
#include <string> // Includes string library
#include <sstream> // Includes string stream library
#include <vector> // Includes vector library

using namespace std; // Uses standard namespace

struct Email { // Defines the Email struct
    string sender; // Stores sender category
    string subject; // Stores email subject
    string date; // Stores email date
    int priority; // Stores numeric priority
}; // Ends struct definition

class MaxHeap { // Defines MaxHeap class
private: // Declares private members
    vector<Email> heap; // Dynamic array to store the heap

    bool higherPriority(const Email& a, const Email& b) { // Defines comparator for emails
        if (a.priority != b.priority) { // Checks if numeric priorities differ
            return a.priority > b.priority; // Returns true if a's priority is greater
        } // Ends priority check

        // For the same sender category, newer email comes first. // Explains date fallback logic
        // MM-DD-YYYY can be compared after converting it to YYYYMMDD. // Explains string conversion
        string dateA = a.date.substr(6, 4) + // Extracts year for email a
                       a.date.substr(0, 2) + // Extracts month for email a
                       a.date.substr(3, 2); // Extracts day for email a

        string dateB = b.date.substr(6, 4) + // Extracts year for email b
                       b.date.substr(0, 2) + // Extracts month for email b
                       b.date.substr(3, 2); // Extracts day for email b

        return dateA > dateB; // Compares dates lexicographically
    } // Ends comparator function

    void heapifyUp(int index) { // Defines function to bubble up elements
        while (index > 0) { // Loops until reaching the root
            int parent = (index - 1) / 2; // Calculates parent index

            if (higherPriority(heap[index], heap[parent])) { // Checks if child is higher priority
                swap(heap[index], heap[parent]); // Swaps child and parent
                index = parent; // Updates index to parent
            } else { // Executes if heap property is met
                break; // Exits loop
            } // Ends conditional
        } // Ends while loop
    } // Ends heapifyUp

    void heapifyDown(int index) { // Defines function to bubble down elements
        int size = heap.size(); // Gets current heap size

        while (true) { // Starts infinite loop
            int left = 2 * index + 1; // Calculates left child index
            int right = 2 * index + 2; // Calculates right child index
            int largest = index; // Assumes current index is largest

            if (left < size && // Checks if left child is in bounds
                higherPriority(heap[left], heap[largest])) { // Checks if left child is larger
                largest = left; // Sets largest to left child
            } // Ends conditional

            if (right < size && // Checks if right child is in bounds
                higherPriority(heap[right], heap[largest])) { // Checks if right child is larger
                largest = right; // Sets largest to right child
            } // Ends conditional

            if (largest != index) { // Checks if a child was larger
                swap(heap[index], heap[largest]); // Swaps current with largest child
                index = largest; // Updates index to continue checking
            } else { // Executes if current is largest
                break; // Exits loop
            } // Ends conditional
        } // Ends while loop
    } // Ends heapifyDown

public: // Declares public members
    void insert(Email email) { // Method to add new email
        heap.push_back(email); // Appends email to vector
        heapifyUp(heap.size() - 1); // Restores heap property upwards
    } // Ends insert

    void removeMax() { // Method to remove highest priority email
        if (heap.empty()) { // Checks if heap is empty
            return; // Exits if empty
        } // Ends conditional

        heap[0] = heap.back(); // Replaces root with last element
        heap.pop_back(); // Removes last element

        if (!heap.empty()) { // Checks if elements remain
            heapifyDown(0); // Restores heap property downwards
        } // Ends conditional
    } // Ends removeMax

    Email getMax() { // Method to return highest priority email
        return heap[0]; // Returns root element
    } // Ends getMax

    bool empty() { // Method to check if heap is empty
        return heap.empty(); // Returns boolean
    } // Ends empty

    int size() { // Method to return heap size
        return heap.size(); // Returns size
    } // Ends size
}; // Ends MaxHeap class

int getPriority(string sender) { // Function to map sender to priority integer
    if (sender == "Boss") { // Checks for Boss
        return 5; // Returns 5
    } // Ends Boss check
    if (sender == "Subordinate") { // Checks for Subordinate
        return 4; // Returns 4
    } // Ends Subordinate check
    if (sender == "Peer") { // Checks for Peer
        return 3; // Returns 3
    } // Ends Peer check
    if (sender == "ImportantPerson") { // Checks for ImportantPerson
        return 2; // Returns 2
    } // Ends ImportantPerson check
    return 1; // OtherPerson // Default return
} // Ends getPriority

Email parseEmail(string line) { // Function to parse input line
    Email email; // Initializes Email struct

    // Remove "EMAIL " // Explains next action
    line = line.substr(6); // Slices first 6 characters off string

    // Find the commas separating the three fields. // Explains next action
    size_t firstComma = line.find(','); // Finds index of first comma
    size_t secondComma = line.find(',', firstComma + 1); // Finds index of second comma

    email.sender = line.substr(0, firstComma); // Extracts sender substring

    email.subject = line.substr( // Extracts subject substring
        firstComma + 1, // Starts after first comma
        secondComma - firstComma - 1 // Calculates length of subject
    ); // Ends subject extraction

    email.date = line.substr(secondComma + 1); // Extracts date after second comma

    // Remove possible spaces after commas. // Explains whitespace handling
    if (!email.subject.empty() && email.subject[0] == ' ') { // Checks if first char is space
        email.subject = email.subject.substr(1); // Removes first character
    } // Ends conditional

    if (!email.date.empty() && email.date[0] == ' ') { // Checks if first char is space
        email.date = email.date.substr(1); // Removes first character
    } // Ends conditional

    email.priority = getPriority(email.sender); // Calculates and assigns priority

    return email; // Returns populated struct
} // Ends parseEmail

int main() { // Main execution block
    MaxHeap inbox; // Instantiates MaxHeap object

    string line; // Declares string for input

    while (getline(cin, line)) { // Loops through standard input lines
        if (line.empty()) { // Checks if line is empty
            continue; // Skips to next loop iteration
        } // Ends conditional

        if (line.substr(0, 5) == "EMAIL") { // Checks for EMAIL command
            Email email = parseEmail(line); // Parses line into struct
            inbox.insert(email); // Inserts into heap
        } // Ends EMAIL check
        else if (line == "NEXT") { // Checks for NEXT command
            if (!inbox.empty()) { // Verifies heap isn't empty
                Email email = inbox.getMax(); // Retrieves root email

                cout << "Sender: " << email.sender << endl; // Prints sender
                cout << "Subject: " << email.subject << endl; // Prints subject
                cout << "Date: " << email.date << endl; // Prints date
            } // Ends inner conditional
        } // Ends NEXT check
        else if (line == "READ") { // Checks for READ command
            if (!inbox.empty()) { // Verifies heap isn't empty
                inbox.removeMax(); // Deletes root email
            } // Ends inner conditional
        } // Ends READ check
        else if (line == "COUNT") { // Checks for COUNT command
            cout << inbox.size() << endl; // Prints size of heap
        } // Ends COUNT check
    } // Ends while loop

    return 0; // Terminates program
} // Ends main