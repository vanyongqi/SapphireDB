/*******************************************************************************
)CommandFactory是命令工厂类，可以向命令工厂注册命令，也可以从里面取一个命令对象

*******************************************************************************/
#include "commandFactory.hpp"

CommandFactory::CommandFactory()
{
   addCommand();
}

ICommand * CommandFactory::getCommandProcesser(const char * pCmd)
{
   ICommand * pProcessor = NULL;
   do {
      COMMAND_MAP::iterator iter;
      iter = _cmdMap.find(pCmd);
      if( iter != _cmdMap.end() )
      {
         pProcessor = iter->second;
      }
   }while(0);
   return pProcessor;
}

