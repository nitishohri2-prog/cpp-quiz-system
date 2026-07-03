#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <cctype>

using namespace std;
using namespace std::chrono;

class Question
{
public:
    string question;
    string options[4];
    char correct;

    void display()
    {
        cout << "\n" << question << "\n";
        for (int i = 0; i < 4; i++)
        {
            cout << options[i] << "\n";
        }
    }
};

class Player
{
    public:
    string name;
    int score = 0;
};

class Quiz
{
    private:
    vector<Question> questions;
    int timeLimit; // in seconds
    
    public:
    Quiz(int t)
    {
        timeLimit = t;
    } 

    // Function to load questions from a file

    vector<Question> loadQuestions()
    {
        questions.clear();
        ifstream file("questions.txt");
        if (!file.is_open())
        {
            cout << "Error opening questions file!" << endl;
            return questions;
        }

        

        while (true)
        {
            Question q;

            // Read the question
            if (!getline(file, q.question))
                break; // End of file

            if (q.question.empty())
                continue; // Skip empty lines
            
                // Read the options
            getline(file, q.options[0]);
            getline(file, q.options[1]);
            getline(file, q.options[2]);
            getline(file, q.options[3]);
            // Read the correct answer
            string correctLine;
            getline(file, correctLine);
            if (!correctLine.empty())
                q.correct = correctLine[0];

            questions.push_back(q);
        }

        file.close();
        return questions;
    }

    void shuffleQuestions()
    {
        srand(time(0));
        random_shuffle(questions.begin(), questions.end());
    }

    void startQuiz(Player &p)
    {
        cout << "\n Starting the quiz for " << p.name << "...\n";

        for(int i = 0; i < questions.size(); i++)
        {
            Question q = questions[i];
            q.display();

            cout << "\n You have " << timeLimit << " seconds to answer(A/B/C/D): ";

            auto start = steady_clock::now();
            char answer;
            cin >> answer;

            auto end = steady_clock::now();
            int timeTaken = duration_cast<seconds>(end - start).count();

            if(timeTaken > timeLimit)
            {
                cout << "Time's up! You took too long to answer.\n";
                continue;
            }

            if(toupper(answer) == toupper(q.correct))
            {
                cout << "Correct answer!\n";
                p.score++;
            }
            else
            {
                cout << "Wrong answer! The correct answer was: " << q.correct << "\n";
            }
        }

        cout << "\n Quiz finished! \n";
        cout << "final score: " << p.score << "/" << questions.size() << "\n";

       saveScore(p);

    }

    void saveScore(Player p)
    {

        ofstream 
        file("leaderboard.txt", ios::app);
        file << p.name << " " << p.score << endl;
        file.close();
    }

    void displayLeaderboard()
    {
        ifstream 
      file("leaderboard.txt");

      cout << "\n Leaderboard: \n";

      string name;
        int score;

        vector<pair<string, int>> leaderboard;

        while(file >> name >> score)
        {
            leaderboard.push_back({name, score});
        }

        file.close();

        sort(leaderboard.begin(), leaderboard.end(), [](auto &a, auto &b) 
        {
            return b.second > a.second;
        });

        for(auto &p : leaderboard)
        {
            cout << p.first << " : " << p.second << "\n";
        }
        
    }
        

};

int main()
{
    int choice;

    do
    {
        cout << "\n Quiz System Menu: \n";
        cout << "1. Start Quiz\n";
        cout << "2. View Leaderboard\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if(!(cin >> choice))
        {
            cin.clear(); // clear the error flag
            cin.ignore(1000, '\n'); // ignore the rest of the line
            continue; // go back to the start of the loop
        }

        if(choice == 1)
        {
            Player p;
            cout << "Enter your name: ";
            cin >> p.name;

            int difficulty;
            cout << "Select difficulty level (1-Easy(no timer), 2-Medium(10 seconds), 3-Hard(5 seconds)): ";
            cout << "Enter your choice: ";
            cin >> difficulty;

            int timeLimit = 9999;

            if(difficulty == 1)
            {
                timeLimit = 9999; // No timer
            }
            else if(difficulty == 2)
            {
                timeLimit = 10; // 10 seconds
            }
            else if(difficulty == 3)
            {
                timeLimit = 5; // 5 seconds
            }
            else
            {
                cout << "Invalid difficulty level! Defaulting to Easy.\n";
                timeLimit = 9999;
            }

            Quiz q(timeLimit);
            q.loadQuestions();

            cout << "Loaded " << q.loadQuestions().size() << endl;
            q.shuffleQuestions();
            q.startQuiz(p);
            
        }

        else if(choice == 2)
        {
            Quiz q(0); // Time limit is irrelevant for leaderboard
            q.displayLeaderboard();
        }
    } while(choice != 3);

    cout << "Exiting the quiz system. Goodbye!\n";
    return 0;
}
      
        
    
    
