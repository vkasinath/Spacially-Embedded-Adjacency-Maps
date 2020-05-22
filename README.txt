HOMEWORK 7: SPATIALLY-EMBEDDED ADJACENCY MAPS


NAME: Vishaal Kasinath


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
https://www.tutorialspoint.com/cpp_standard_library/map.htm
https://en.cppreference.com/w/cpp/language/typedef
https://en.cppreference.com/w/cpp/language/types    [for int, unsigned int limits reference]


Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT: 16



ORDER NOTATION ANALYSIS:
Consider the following variables and give the complexity along with a short
justification for each of the functions below:

n: The total number of users
l: The total number of users with a location
d: The total number of users with degree >= 1
m: The total number of connections
p: The number of users that a function will print out

loadConnections(): O(log n)
the higher order activity is the map insert which is O(log n)

loadLocations(): O(log l)
the higher order activity is the map insert which is O(log l)

printAllUsersWithinDistance(): taking highest order activity = O(l log l)
O(log n) for finding start ID
O(log l) for finding location of start ID
l*O(log l) for map insert for all other users with location (can be a maximum of l users)
O(p) order for printing, not larger than O(l log l)

printDegreesHistogram(): O(n log d)
O(n log d) loop through n users, to find O(log d)
O(p) order for printing, worst case as large as O(d)

printDegreesOfAll():O(n log d)
O(n log d) loop through n users, to find O(log d)
O(p) order for printing, worst case as large as O(d)

printFriendsWithDegree(): O(m log n)
O(log n) for finding start ID
O(m log n) for inserting m connections into temp for n users
O(p) for order for printing, worst cas scenario as large as O(m)

printFriendsWithinDistance(): O(m log m)
O(log n) for finding start ID
O(log l) for finsing start ID's location
O(m log m) for looping through m connections and inserting into tmep map
O(p) for order of printing, worst case scenario as large as O(m)

printUsersWithinIDRange(): O(n)
O(n) for worst case scenario where the start ID is either the very last value
or the very first value.
O(p) for order of printing, can only be as large as O(n)


MISC. COMMENTS TO GRADER:  
(optional, please be concise!)






