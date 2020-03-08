#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <iterator>
#include <algorithm>
#include <memory>
//#include <chrono>
//#include <ratio>
//#include <thread>
#include <ctime>

#include "common.h"

using namespace std;


//void TestFile(const char *file_name);
//-----------------------------------------------


class Observer
{
private:

public:

    virtual void Do(const std::vector<std::string> &cmds, time_t t) = 0;
    virtual ~Observer() = default;
};
//-----------------------------------------------

class CommandHandler
{
private:

    std::vector<shared_ptr<Observer>> subs;

    size_t N = 3;
    size_t BracketOpenLevel = 0;
    std::vector<std::string> cmds;

    time_t timeFirst;

public:

    CommandHandler(size_t _N) : N(_N) {}

    void subscribe(const shared_ptr<Observer> &obs)
    {
        //subs.push_back(std::move(obs));
        subs.push_back(obs);
    }

    void AnalyzeCommand(const std::string &str)
    {
        if (str != "{" && str != "}")
        {
            if (cmds.empty())
                timeFirst = std::time(nullptr);

            cmds.push_back(str); // emplace_back
        }
        if (cmds.size() == N && !BracketOpenLevel)
        {
            ExecForAllSubs(false);
            //cmds.clear();
        }
        if (str == "{")
        {
            BracketOpenLevel++;

            if (BracketOpenLevel == 1 && !cmds.empty())
                ExecForAllSubs(false);

        }
        if (str == "}")
        {
            BracketOpenLevel--;
            if (BracketOpenLevel == 0 && !cmds.empty())
            {
                ExecForAllSubs(false);
                //cmds.clear();
            }
        }

    }

    void ExecForAllSubs(bool isFinished)
    {
        if ( !cmds.empty() &&  ( BracketOpenLevel == 0 || (BracketOpenLevel == 1 && !isFinished) ) )
        {
            for (auto &s : subs)
            {
                s->Do(cmds, timeFirst);
            }

            cmds.clear();
        }
    }

};
//-----------------------------------------------

class ConsoleObserver : public std::enable_shared_from_this<ConsoleObserver>, public Observer
{
public:

    void Register(const unique_ptr<CommandHandler> &_cmds)
    {
        auto t = shared_from_this();
        _cmds->subscribe(t);
    }

    void Do(const std::vector<std::string> &cmds, [[maybe_unused]] time_t t) override
    {
        //std::cout << MY_P_FUNC << std::endl;
        std::cout << "bulk: ";
        size_t cmds_size = cmds.size();
        for (size_t i = 0; i < cmds_size; i++)
            std::cout << cmds[i] << (  (i<(cmds_size-1)) ? ", " : "\n");
    }
};
//-----------------------------------------------

class LocalFileObserver : public Observer, public std::enable_shared_from_this<LocalFileObserver>
{
public:

    void Register(const unique_ptr<CommandHandler> &_cmds)
    {
        _cmds->subscribe(shared_from_this());
    }

    void Do(const std::vector<std::string> &cmds, time_t t) override
    {
        static int number = 0;
        std::stringstream fn;
        fn << "bulk" << t << "-" << number++ << ".log";

        ofstream f( fn.str() );

        size_t cmds_size = cmds.size();
        for (size_t i = 0; i < cmds_size; i++)
            f << cmds[i] << std::endl;

        f.close();
    }
};
//-----------------------------------------------







