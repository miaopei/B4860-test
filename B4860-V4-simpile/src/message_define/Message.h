#ifndef MESSAGE_H_4358943
#define MESSAGE_H_4358943

enum 
{
	SERVER_CENTER_TYPE = 1000,

};

enum 
{
	MSG = BASE_MSG_BEGIN + 1000
};

class CMsg
{
public:
	enum
	{
		MSG_ID = MSG
	};

	CMsg()
		: m_nSessionID(0)
		, m_nTimes(0)
	{

	}

	int m_nSessionID;
	int m_nTimes;
};
#endif

