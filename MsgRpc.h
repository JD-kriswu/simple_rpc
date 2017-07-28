#ifndef _MSGRPC_H_
#define _MSGRPC_H_

#include <map>
#include <vector>
#include "syncincl.h"


using namespace std;
class MsgRpc : public CSyncMsg 
{
	public:
	MsgRpc()
	{

	}
	virtual ~MsgRpc()
	{

	}

	virtual int DecodeRequest(char *sBuf,int iLen)=0;

    virtual void HandleError()=0;

	virtual int HandleProcess(); 


};
#endif

