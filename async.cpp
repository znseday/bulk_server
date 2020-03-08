#include "async.h"

#include "bulk.h"

#include <map>

using namespace std;

namespace async
{

static size_t HandleCounter = 1;

class Context
{
private:
    unique_ptr<CommandHandler> cmds;
    shared_ptr<ConsoleObserver> ConsoleObs;
    shared_ptr<LocalFileObserver> LocalFileObs;

    handle_t Handle;

    string buff;
public:
    Context() = delete;

    Context(size_t block_size, size_t hc)
    {
		MY_DEBUG_ONLY(cout << "Constructor of " << hc << endl;)

        Handle = reinterpret_cast<handle_t>(hc);

        cmds = make_unique<CommandHandler>( block_size );

        ConsoleObs = make_shared<ConsoleObserver>();
        ConsoleObs->Register(cmds);

        LocalFileObs = make_shared<LocalFileObserver>();
        LocalFileObs->Register(cmds);
    }

    handle_t GetHandle() {return Handle;}

    void AddSymbol(const char symbol)
    {
        if (symbol == '\n')
        {
            cmds->AnalyzeCommand(buff);
            buff.clear();
        }
        else
        {
             buff += symbol;
        }
    }

    ~Context()
    {
        cmds->ExecForAllSubs(true); // Требуется ли по заданию вывод оставшихся команд как в обычной bulk ?????

		MY_DEBUG_ONLY(cout << "Destructor of " << Handle << endl;)
    }
};

static map<handle_t,Context> contexts;

handle_t connect(size_t block_size)
{
    handle_t Handle = reinterpret_cast<handle_t>(HandleCounter);
    contexts.try_emplace(Handle, block_size, HandleCounter);

    HandleCounter++;
    return Handle;
}

void receive(handle_t handle, const char *data, size_t size)
{
    auto it = contexts.find(handle);
    if (it == contexts.end())
        throw invalid_argument("Context not found");

    for (size_t i = 0; i < size; i++)
        it->second.AddSymbol(data[i]);
}

void disconnect(handle_t handle)
{
    contexts.erase(handle);
}


} // namespace async
