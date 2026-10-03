/*
Author Name: Edbert Jensen
Program Name: EECS 348 Assignment 3
Brief description: Email Priority Program that has an hierarchical order from most to least important (Boss -> Subordinate -> Peer -> ImportantPerson -> OtherPerson)
Inputs: multiple lines of commands (EMAIL, NEXT, READ, COUNT) with their respective parameters
Outputs: Number of emails to read and the next email if NEXT command and COUNT command are called
Collaborators: Chatgpt
Other sources: Chatgpt
Creation date: 9/29/2026 5:04PM
Revision date: 10/3/2026 12:00AM
Revisions: refactor code
*/

#include <iostream> // Includes standard I/O library for terminal output
#include <string> // Includes string library for text manipulation
#include <vector> // Includes vector library for dynamic array usage
#include <sstream> // Includes string stream library for parsing inputs

using namespace std; // Allows usage of standard library components without std:: prefix

struct Email { // Defines a structure to hold email data attributes
    string sender; // Stores the sender category string
    string subject; // Stores the email subject line
    string date; // Stores the raw date string in MM-DD-YYYY format
    int senderPriority; // Stores the numeric priority value of the sender category
    string sortableDate; // Stores the date formatted as YYYYMMDD for chronological comparison
}; // Ends the Email structure definition

string trim(const string& str) { // Defines function taking a constant string reference
    size_t first = str.find_first_not_of(' '); // Finds the first non-space character index
    if (string::npos == first) return ""; // Returns empty string if only spaces exist
    size_t last = str.find_last_not_of(' '); // Finds the last non-space character index
    return str.substr(first, (last - first + 1)); // Extracts and returns the trimmed substring
} // Ends the trim function

int getSenderPriority(const string& sender) { // Defines function to calculate priority integer
    if (sender == "Boss") return 5; // Returns highest priority for Boss
    if (sender == "Subordinate") return 4; // Returns second highest priority for Subordinate
    if (sender == "Peer") return 3; // Returns middle priority for Peer
    if (sender == "ImportantPerson") return 2; // Returns lower priority for ImportantPerson
    if (sender == "OtherPerson") return 1; // Returns lowest priority for OtherPerson
    return 0;  // Returns 0 as a fallback for unknown senders
} // Ends getSenderPriority function

string getSortableDate(const string& date) { // Defines function to reformat date strings
    if (date.length() < 10) return date; // Returns original string if length is unexpectedly short
    return date.substr(6, 4) + date.substr(0, 2) + date.substr(3, 2); // Rearranges to YYYYMMDD and returns
} // Ends getSortableDate function

bool comesBefore(const Email& a, const Email& b) { // Defines comparator for two Email objects
    if (a.senderPriority != b.senderPriority) { // Checks if sender priorities are different
        return a.senderPriority > b.senderPriority; // Returns true if a's priority is strictly greater than b's
    } // Ends if statement
    return a.sortableDate > b.sortableDate; // Compares dates lexicographically if priorities match, newest first
} // Ends comesBefore function

class EmailMaxHeap { // Defines the MaxHeap class for the priority queue
private: // Restricts access to internal heap data and helper functions
    vector<Email> heap; // Declares the dynamic array to store the heap elements

    void heapifyUp(int index) { // Defines function to restore max-heap property upwards
        while (index > 0) { // Loops as long as the current node is not the root
            int parent = (index - 1) / 2; // Calculates the parent node index
            if (comesBefore(heap[index], heap[parent])) { // Checks if child has higher priority than parent
                swap(heap[index], heap[parent]); // Swaps the child and parent elements
                index = parent; // Updates index to continue checking upwards
            } else { // Executes if max-heap property is satisfied
                break; // Exits the loop early
            } // Ends if/else block
        } // Ends while loop
    } // Ends heapifyUp function

    void heapifyDown(int index) { // Defines function to restore max-heap property downwards
        int size = heap.size(); // Retrieves the total number of elements in the heap
        while (true) { // Starts an infinite loop to bubble down elements
            int left = 2 * index + 1; // Calculates the left child index
            int right = 2 * index + 2; // Calculates the right child index
            int largest = index; // Assumes the current node is the largest to start

            if (left < size && comesBefore(heap[left], heap[largest])) { // Checks if left child exists and is larger
                largest = left; // Updates largest index to left child
            } // Ends if block
            if (right < size && comesBefore(heap[right], heap[largest])) { // Checks if right child exists and is larger
                largest = right; // Updates largest index to right child
            } // Ends if block
            
            if (largest != index) { // Checks if a child was larger than the current node
                swap(heap[index], heap[largest]); // Swaps current node with the largest child
                index = largest; // Updates index to continue checking downwards
            } else { // Executes if current node is larger than both children
                break; // Exits the downward check loop
            } // Ends if/else block
        } // Ends while loop
    } // Ends heapifyDown function

public: // Exposes public methods for interacting with the priority queue
    void insert(const Email& email) { // Defines method to add a new email
        heap.push_back(email); // Adds the new email to the end of the vector
        heapifyUp(heap.size() - 1); // Restores heap property from the newly added element
    } // Ends insert method

    void read() { // Defines method to mark highest priority email as read
        if (heap.empty()) { // Checks if the heap is empty
            cout << "Error: No emails to read.\n"; // Outputs error message if no emails exist
            return; // Exits the function early
        } // Ends if block
        
        heap[0] = heap.back(); // Overwrites the root (highest priority) with the last element
        heap.pop_back(); // Removes the last element from the vector
        
        if (!heap.empty()) { // Checks if heap is not empty after removal
            heapifyDown(0); // Restores heap property from the root downwards
        } // Ends if block
        
        cout << "email read.\n"; // Outputs confirmation that the email was read
    } // Ends read method

    void next() { // Defines method to peek at the next email to read
        if (heap.empty()) { // Checks if the heap is empty
            cout << "Error: No emails to display.\n"; // Outputs error message if no emails exist
            return; // Exits the function early
        } // Ends if block
        
        Email top = heap[0]; // Retrieves the root element (highest priority email)
        cout << "Sender: " << top.sender << "\n"; // Prints the sender
        cout << "Subject: " << top.subject << "\n"; // Prints the subject
        cout << "Date: " << top.date << "\n"; // Prints the date
    } // Ends next method

    void count() { // Defines method to display unread email count
        cout << heap.size() << "\n"; // Prints the size of the underlying vector
    } // Ends count method
}; // Ends EmailMaxHeap class

int main() { // Main execution entry point
    EmailMaxHeap priorityQueue; // Instantiates the custom max-heap object
    string line; // Declares a string variable to store incoming standard input lines

    while (getline(cin, line)) { // Continuously reads lines from standard input until EOF
        if (line.empty()) continue; // Skips processing for empty blank lines
        
        line = trim(line); // Removes trailing and leading spaces from the command string
        
        if (line.substr(0, 5) == "EMAIL") { // Checks if the command starts with 'EMAIL'
            string payload = line.length() > 5 ? line.substr(5) : ""; // Safely extracts payload or empty string
            stringstream ss(payload); // Creates a stringstream to parse comma-separated values
            string sender = "", subject = "", date = ""; // Initializes string variables for parsed fields
            
            if (getline(ss, sender, ',')) sender = trim(sender); // Extracts and trims sender field
            if (getline(ss, subject, ',')) subject = trim(subject); // Extracts and trims subject field
            if (getline(ss, date, ',')) date = trim(date); // Extracts and trims date field
            
            if (sender.empty() || subject.empty() || date.empty()) { // Checks if any required field is missing
                cout << "Error: Invalid EMAIL command. Missing required parameters.\n"; // Outputs error for malformed command
                continue; // Skips insertion and moves to the next command
            } // Ends validation block
            
            Email newEmail; // Creates a new Email object instance
            newEmail.sender = sender; // Assigns parsed sender to object
            newEmail.subject = subject; // Assigns parsed subject to object
            newEmail.date = date; // Assigns parsed date to object
            newEmail.senderPriority = getSenderPriority(sender); // Calculates and assigns sender priority integer
            newEmail.sortableDate = getSortableDate(date); // Reformats and assigns date for string comparison
            
            priorityQueue.insert(newEmail); // Inserts the populated Email object into the Max Heap
        }  // Ends EMAIL command block
        else if (line == "NEXT") { // Checks if the command is 'NEXT'
            priorityQueue.next(); // Calls the next() method to display the highest priority email
        } // Ends NEXT command block
        else if (line == "READ") { // Checks if the command is 'READ'
            priorityQueue.read(); // Calls the read() method to delete the highest priority email
        } // Ends READ command block
        else if (line == "COUNT") { // Checks if the command is 'COUNT'
            priorityQueue.count(); // Calls the count() method to output the number of emails
        } // Ends COUNT command block
    } // Ends the main file reading loop

    return 0; // Signals successful program termination
} // Ends main function