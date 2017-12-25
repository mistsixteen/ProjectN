#pragma once
#include "Stream.h"

class StreamSP
{
private:
	Stream*	stream;
public:
	Stream* operator->(VOID) { return stream; }
	operator Stream*(VOID) { return stream; }
public:
	StreamSP()
	{
		stream = new Stream();
	}
	~StreamSP()
	{
		delete stream;
	}
};

