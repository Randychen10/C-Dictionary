This project is a command-line dictionary application written in C++. It provides functionalities for users to search for definitions based on keywords and supports additional commands for filtering and organizing the results. Key features include:

Data Loading: The program loads dictionary data from an external file, ensuring the file path is valid before proceeding.

Keyword and Definition Management: It structures entries using a multimap, where each entry comprises a keyword, part of speech, and definition. This allows efficient retrieval of multiple definitions per keyword.

String Processing: Various helper functions trim whitespace, convert strings to lowercase, and handle special formatting, like capitalizing specific terms (e.g., "CSC").

Search Functionality: Users can enter a keyword to view its definitions. Additional commands like distinct (for unique definitions) and reverse (for reversing order) can refine search results. Users can also specify parts of speech to filter relevant entries.

User Guidance: The program offers clear prompts, including an introductory help message. It also handles errors, such as unrecognized commands or duplicate inputs, with detailed feedback.

Interactive Mode: In continuous search mode, users can perform multiple lookups, receive search count feedback, and exit by entering the command !q.

This project is a practical example of using C++ data structures like maps, sets, and vectors to manage and process complex data for a user-friendly console application​ (Dictionary.cpp)
