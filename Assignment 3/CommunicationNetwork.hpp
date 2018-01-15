#ifndef COMMUNICATIONNETWORK_HPP
#define COMMUNICATIONNETWORK_HPP

struct City{
    std::string cityName;
    std::string message;
    City *next;
    City *previous;
    //struct constructor (default and parametrized)
    City(std::string initName, City *initNext=nullptr,
      City *initPrevious=nullptr, std::string initMessage=""):
      cityName(initName), message(initMessage), next(initNext),
      previous(initPrevious){}
};

class CommunicationNetwork
{
    public:
        CommunicationNetwork();
        ~CommunicationNetwork();
        void addCity(std::string newCityName, std::string previousCityName);
        void buildNetwork();
        void transmitMsg(char *); //this is like a string
        void transmitMsg(std::string filename); //this is like a string
        void printNetwork();
        void deleteCity(std::string removeCity);
        void deleteNetwork();
    protected:
    private:
        City *head;
        City *tail;
};

#endif // COMMUNICATIONNETWORK_HPP
