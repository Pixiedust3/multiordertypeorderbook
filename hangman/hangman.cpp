#include<iostream>
#include<vector>
#include"Random.h"
#include<string_view>

enum class Status {
    WordAccepted,
    WordRejected
};
class Interface{
private:
        std::vector<std::string_view> words{"mystery", "broccoli" , "account", "almost", "spaghetti", "opinion", "beautiful", "distance", "luggage"};
public:
        std::string_view getRandomWord()
    {
        return words[Random::get<std::size_t>(0, words.size()-1)];
    }
        std::string displayunguessed(std::string_view words)
        {
            std::string unsc{};
           for(char c:words)
           {
            unsc=unsc+"_ ";

           }
           return unsc;
        }
        std::string displaylife(int n)
        {
            std::string plus{};
            for(int i=0;i<n;i++)
            {
                plus=plus+"+";
            }
            return plus;

        }
        char getWord( )
        {
            char x;
            std::cin>>x;
            return x;
        }
};

class HandleInput
{
public:
std::string word;
    std::string progress;
    int lives;
    HandleInput(std::string_view w, int l)
        : word(w)
        , progress(w.size(), '_')
        , lives(l)
         {}


    private:
    void checklives() {
        if(lives==0)
        {
            std::cout<<"no more lives left";

        }
    }


};
int main()
{
    std::cout<<"Welcome to C++man (a variant of Hangman)\n ";
    std::cout << "To win: guess the word.  To lose: run out of pluses.\n";
    Interface game;
    std::string plusdisplay{game.displaylife(5)};
    
    std::cout << "Random word: " << game.displayunguessed(game.getRandomWord()) << '\t'<<'\t'<<"Remaining lifes"<<'\t'<<plusdisplay<<'\n';
    char x{game.getWord()};
 
    
    return 0;
}
