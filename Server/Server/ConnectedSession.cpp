#include "stdafx.h"
#include "ConnectedSession.h"


BOOL ConnectedSession::Restart(SOCKET listenSocket)
{
	ThreadSync Sync;

	End();

	return Begin() && Accept(listenSocket);
}

ConnectedSession::ConnectedSession(VOID)
	:Connected(FALSE)
{
}


ConnectedSession::~ConnectedSession(VOID)
{
}
