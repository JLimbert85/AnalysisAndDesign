#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

// Struct for holding course data object
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

/**
 * load courses from a csv file and store them as Course objects in a vector
 *
 * @param courses Address of the vector<Course> instance
 * @param filename name of the csv file to load from
 */
bool loadCourses(vector<Course> &courses, string filename) {
    // Open the file
    ifstream file(filename.c_str());

    // Check if file is properly opened before continuing
    if (!file.is_open()) {
        return false;
    }

    // variable to hold the line as a string
    string line;

    // Iterate through each line in the file
    while (getline(file, line)) {
        // vector to hold each value from the line as a string
        vector<string> row;
        // string stream of the line
        stringstream ss(line);
        // variable to store data from split line
        string cell;

        // split the line by the commas into separate values and store in row vector
        while (getline(ss, cell, ',')) {
            if (cell.size() > 1) {
                row.push_back(cell);
            }
        }

        // initialize course object
        Course course;
        // set course number from row vector
        course.courseNumber = row.at(0);
        // set course name from row vector
        course.courseName = row.at(1);

        // any remaining cells in row will be prerequisites, iterate through them
        for (int i = 2; i < row.size(); ++i) {
            // store prerequisite in a string
            string req = row.at(i);
            // remove any spaces or special characters
            req.erase(req.find_last_not_of(" \n\r\t") + 1);
            // store prerequisite in course prerequisite vector
            course.prerequisites.push_back(req);
        }
        // add course to courses vector
        courses.push_back(course);
    }

    // close the file
    file.close();
    // return true to show that the file was loaded successfully
    return true;
}

/**
 * print course information from provided courses vector and course number
 *
 * @param courses Address of the vector<Course> instance
 * @param courseNumber The course number to print
 */
void printCourse(vector<Course> &courses, string courseNumber) {
    // iterate through courses to find the course number
    for (int i = 0; i < courses.size(); ++i) {
        // if the course number is found
        if (courses.at(i).courseNumber == courseNumber) {
            //print the course information
            cout << courses.at(i).courseNumber << ", " << courses.at(i).courseName << endl;
            cout << "Prerequisites: ";
            // iterate through the prerequisites, printing each one
            for (int j = 0; j < courses.at(i).prerequisites.size(); ++j) {
                cout << courses.at(i).prerequisites.at(j);
                if (j != courses.at(i).prerequisites.size() - 1) {
                    cout << ", ";
                }
            }
            // print none if there are no prerequisites
            if (courses.at(i).prerequisites.size() == 0) {
                cout << "None";
            }
            cout << endl;
            return;
        }
    }
    // print if the course was not found after iterating through all courses
    cout << "Course was not found." << endl;
}

/**
 * Partition the vector of courses into two parts, low and high
 *
 * @param courses Address of the vector<Course> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Course> &courses, int begin, int end) {
    //set low and high equal to begin and end
    int low = begin;
    int high = end;

    // Calculate the middle element as middlePoint (int)
    int mid = low + (high - low) / 2;

    // Set Pivot as middlePoint element title to compare (string)
    Course pivot = courses.at(mid);
    // Set done boolean to control while loop condition
    bool done = false;

    // while not done
    while (!done) {
        // keep incrementing low index while courses[low].courseNumber < Pivot
        while (courses.at(low).courseNumber < pivot.courseNumber) {
            low += 1;
        }
        // keep decrementing high index while Pivot < courses[high].courseNumber
        while (pivot.courseNumber < courses.at(high).courseNumber) {
            high -= 1;
        }
        /* If there are zero or one elements remaining,
            all courses are partitioned. Return high */
        if (low >= high) {
            done = true;
        } else {
            // else swap the low and high courses
            swap(courses.at(low), courses.at(high));

            // move low and high closer ++low, --high
            low += 1;
            high -= 1;
        }
    }
    //return high;
    return high;
}

/**
 * Perform a quick sort on course number
 *
 * @param courses address of the vector<Course> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 **/
void quickSort(vector<Course> &courses, int begin, int end) {
    //set mid equal to 0
    int mid = 0;

    /* Base case: If there are 1 or zero courses to sort,
     partition is already sorted otherwise if begin is greater
     than or equal to end then return*/
    if (begin >= end) {
        return;
    }

    /* Partition courses into low and high such that
     midpoint is location of last element in low */
    mid = partition(courses, begin, end);

    // recursively sort low partition (begin to mid)
    quickSort(courses, begin, mid);

    // recursively sort high partition (mid+1 to end)
    quickSort(courses, mid + 1, end);
}

/**
 * Sort all courses alphanumerically by course number then print all the courses information
 *
 * @param courses address of the vector<Course> instance to be sorted
 **/
void printAllCourses(vector<Course> &courses) {
    // call quickSort function to sort all the courses
    quickSort(courses, 0, courses.size() - 1);
    // iterate through all the courses, printing each courses information
    for (int i = 0; i < courses.size(); ++i) {
        cout << courses.at(i).courseNumber << ", " << courses.at(i).courseName << endl;
    }
}

int main() {
    // initialize vector to store all courses
    vector<Course> courses;
    // initialize string to store csv file name
    string filename;
    // initialize string to store course name
    string courseName;
    // initialize int to store menu choice
    int choice = -1;

    // display welcome message
    cout << "Welcome to the course planner." << endl;

    // loop through menu until 9 is pressed to exit the program
    while (choice != 9) {
        // display menu options
        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit." << endl;
        cout << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        // if option entered was not a number, display error and clear input
        if (cin.fail()) {
            cout << "Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(256, '\n');
            continue;
        }

        // switch statement for choice number
        switch (choice) {
            // prompt for filename then load courses
            case 1:
                cout << "Please enter the file name: ";
                cin >> filename;
                // display success message if courses loaded successfully
                if (loadCourses(courses, filename)) {
                    cout << courses.size() << " courses loaded successfully" << endl;
                } else {
                    // display failed message if the courses failed to load
                    cout << "Failed to load courses" << endl;
                }
                break;
            // print all the courses
            case 2:
                cout << "Here is a sample schedule:" << endl << endl;
                printAllCourses(courses);
                break;
            //prompt for the course number, then print the course information
            case 3:
                cout << "What course do you want to know more about? ";
                cin >> courseName;
                printCourse(courses, courseName);
                break;
            // display goodbye message then exit program
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;
            // display error if choice number is not one of the valid options
            default:
                cout << choice << " is not a valid option." << endl;
        }
    }

    // exit the program
    return 0;
}
