#ifndef __INT_EVENT_H__
#define __INT_EVENT_H__

#include <sstream>

#include "eventImp.h"

class IntEvent: public EventImp
{
public:
	IntEvent(){}
	IntEvent(int _data): m_data(_data){}
	//virtual ~IntEvent();
	
	virtual std::string SerializePayload() const;

	int GetData() {return m_data;}
	int m_data;
};

inline std::string IntEvent::SerializePayload() const
{
	std::stringstream ss;
	ss << "data" << EventImp::m_keyValueDelim << m_data;
	return ss.str(); 
}

#endif /* __INT_EVENT_H__ */

