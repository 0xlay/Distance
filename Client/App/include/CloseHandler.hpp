#pragma once
#include "DefaultTcpClient.hpp"


namespace Distance
{
	namespace _internal
	{
		/*
		* @brief The HandlerRoutine function is handling query: WM_CLOSE, WM_QUERYENDSESSION
		*/
		LRESULT HandlerRoutine(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	}

	/*
	* @brief The CloseHandler function sets HandlerRoutine for handling 
	* to close application
	*/
	void CloseHandler(
		std::shared_ptr<Distance::Network::DefaultTcpClient> client, 
		std::shared_ptr<bool> isRunClient
	);
}